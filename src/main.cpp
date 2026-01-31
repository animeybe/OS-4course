#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <atomic>
#include <mutex>
#include <thread>
#include <chrono>
#include <string>
#include "../include/timer.h"

#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <signal.h>
#endif

// Глобальные переменные (shared memory)
std::atomic<long long> counter{0};  // Общий счетчик
std::mutex logMutex;                // Синхронизация логов
std::atomic<bool> running{true};    // Флаг завершения
pid_t mainPID = 0;
bool isMainProcessFlag = false;

// Таймер 300мс: +1 к счетчику во всех процессах
void timer300ms() {
    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
        counter.fetch_add(1, std::memory_order_relaxed);
    }
}

// Таймер 1с: лог счетчика только в главном процессе
void timer1s() {
    auto lastLog = std::chrono::steady_clock::now();
    while (running) {
        if (isMainProcessFlag) {
            auto now = std::chrono::steady_clock::now();
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastLog).count();
            
            if (elapsed >= 1000) {
                CrossPlatformTimer::log("COUNTER: " + std::to_string(counter.load()));
                lastLog = now;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}

// Таймер 3с: спавн Copy1/Copy2 только в главном процессе
void timer3s() {
    pid_t copy1PID = 0, copy2PID = 0;
    auto lastSpawn = std::chrono::steady_clock::now();
    
    while (running && isMainProcessFlag) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastSpawn).count();
        
        if (elapsed >= 3000) {
            CrossPlatformTimer::log("3s timer - checking child processes");
            
            // Спавн Copy1 если предыдущая завершилась
            if (copy1PID == 0 || !CrossPlatformTimer::isProcessRunning(copy1PID)) {
                copy1PID = CrossPlatformTimer::spawnCopy1();
                if (copy1PID > 0) {
                    CrossPlatformTimer::log("Copy1 spawned successfully (PID: " + std::to_string(copy1PID) + ")");
                } else {
                    CrossPlatformTimer::log("Failed to spawn Copy1");
                }
            } else {
                CrossPlatformTimer::log("Copy1 still running (PID: " + std::to_string(copy1PID) + "), skipping spawn");
            }
            
            // Спавн Copy2 если предыдущая завершилась
            if (copy2PID == 0 || !CrossPlatformTimer::isProcessRunning(copy2PID)) {
                copy2PID = CrossPlatformTimer::spawnCopy2();
                if (copy2PID > 0) {
                    CrossPlatformTimer::log("Copy2 spawned successfully (PID: " + std::to_string(copy2PID) + ")");
                } else {
                    CrossPlatformTimer::log("Failed to spawn Copy2");
                }
            } else {
                CrossPlatformTimer::log("Copy2 still running (PID: " + std::to_string(copy2PID) + "), skipping spawn");
            }
            
            lastSpawn = now;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

// Windows: обработчик Ctrl+C
#ifdef _WIN32
BOOL WINAPI ConsoleHandler(DWORD signal) {
    if (signal == CTRL_C_EVENT) {
        running = false;
        CrossPlatformTimer::log("Main process terminating (Ctrl+C)");
        return TRUE;
    }
    return FALSE;
}
#endif

// Copy1: +10 к счетчику за 100мс
void handleCopy1() {
    CrossPlatformTimer::log("Copy1 START");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    long long oldValue = counter.load();
    counter.fetch_add(10, std::memory_order_relaxed);
    CrossPlatformTimer::log("Copy1 EXIT (counter +10: " + std::to_string(oldValue) + " -> " + std::to_string(counter.load()) + ")");
}

// Copy2: *2 счетчик -> 2с пауза -> /2 счетчик
void handleCopy2() {
    CrossPlatformTimer::log("Copy2 START");
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    long long current = counter.load();
    counter.fetch_add(current, std::memory_order_relaxed);
    CrossPlatformTimer::log("Copy2 doubled: " + std::to_string(current) + " -> " + std::to_string(counter.load()));
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    current = counter.load();
    long long half = current / 2;
    counter.fetch_sub(half, std::memory_order_relaxed);
    CrossPlatformTimer::log("Copy2 EXIT: restored " + std::to_string(half) + " -> " + std::to_string(counter.load()));
}

int main(int argc, char* argv[]) {
    // Получение PID текущего процесса
#ifdef _WIN32
    mainPID = GetCurrentProcessId();
#else
    mainPID = getpid();
#endif
    
    // Определение главного процесса (минимальный PID)
    isMainProcessFlag = CrossPlatformTimer::isMainProcess();
    CrossPlatformTimer::log("PROGRAM START");

    // Обработка аргументов командной строки (Copy1/Copy2)
    if (argc > 1) {
        if (std::string(argv[1]) == "copy1") {
            handleCopy1();
            return 0;
        } else if (std::string(argv[1]) == "copy2") {
            handleCopy2();
            return 0;
        }
    }

    // Регистрация обработчиков Ctrl+C
#ifdef _WIN32
    SetConsoleCtrlHandler(ConsoleHandler, TRUE);
#else
    signal(SIGINT, [](int) { running = false; });
#endif

    // Запуск фоновых таймеров
    std::thread t300ms(timer300ms);  // 300мс: +1 во всех процессах
    std::thread t1s(timer1s);        // 1с: лог счетчика (только main)
    std::thread t3s;
    
    if (isMainProcessFlag) {
        t3s = std::thread(timer3s);  // 3с: спавн копий (только main)
    }

    // CLI: команды "set X"
    std::cout << "Timer started. PID: " << mainPID 
              << (isMainProcessFlag ? " (MAIN)" : " (slave)") 
              << "\nCommands: 'set X' or Ctrl+C\n";

    std::string line;
    while (running && std::getline(std::cin, line)) {
        if (line.find("set ") == 0) {
            try {
                size_t pos = 4;
                while (pos < line.size() && isspace(line[pos])) pos++;
                long long value = std::stoll(line.substr(pos));
                counter.store(value, std::memory_order_relaxed);
                std::cout << "Counter set to " << value << std::endl;
                CrossPlatformTimer::log("CLI: counter set to " + std::to_string(value));
            } catch (...) {
                std::cout << "Invalid number after 'set '" << std::endl;
            }
        }
    }

    // Завершение потоков
    running = false;
    
    if (t300ms.joinable()) t300ms.join();
    if (t1s.joinable()) t1s.join();
    if (t3s.joinable()) t3s.join();
    
    CrossPlatformTimer::log("PROGRAM EXIT");
    return 0;
}
