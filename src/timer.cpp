#include "../include/timer.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <chrono>
#include <mutex>
#include <filesystem>

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <processthreadsapi.h>
#include <psapi.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>
#include <pthread.h>
#include <sys/stat.h>
#include <atomic>
#endif

extern std::mutex logMutex;

// Путь к лог-файлу: build/logs/timer.log
std::string CrossPlatformTimer::getLogPath() {
#ifdef _WIN32
    char path[MAX_PATH];
    if (GetModuleFileNameA(NULL, path, MAX_PATH)) {
        std::string exeDir = std::string(path);
        size_t pos = exeDir.find_last_of("\\");
        if (pos != std::string::npos) {
            exeDir = exeDir.substr(0, pos);
        }
        std::string logDir = exeDir + "\\logs";
        CreateDirectoryA(logDir.c_str(), NULL);
        return logDir + "\\timer.log";
    }
    return "timer.log";
#else
    std::string logDir = std::string(getenv("PWD")) + "/logs";
    mkdir(logDir.c_str(), 0755);
    return logDir + "/timer.log";
#endif
}

// Форматированное время с миллисекундами
std::string CrossPlatformTimer::getCurrentTimeStr() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;
    
    std::stringstream ss;
#ifdef _WIN32
    struct tm timeinfo;
    localtime_s(&timeinfo, &time_t);
    ss << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
#else
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S");
#endif
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
    return ss.str();
}

// PID текущего процесса как строка
std::string CrossPlatformTimer::getPIDStr() {
#ifdef _WIN32
    return std::to_string(GetCurrentProcessId());
#else
    return std::to_string(getpid());
#endif
}

// Потокобезопасная запись в лог-файл
void CrossPlatformTimer::log(const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);
    std::string logPath = getLogPath();
    std::ofstream file(logPath, std::ios::app);
    if (file.is_open()) {
        file << "[" << getCurrentTimeStr() << "] PID:" 
             << getPIDStr() << " " << message << std::endl;
        file.close();
    }
}

// Выбор главного процесса (минимальный PID, атомарно)
bool CrossPlatformTimer::isMainProcess() {
    static std::atomic<pid_t> minPID{std::numeric_limits<pid_t>::max()};
    pid_t currentPID = 0;
    
#ifdef _WIN32
    currentPID = GetCurrentProcessId();
#else
    currentPID = getpid();
#endif
    
    pid_t oldMin = minPID.load();
    while (oldMin == std::numeric_limits<pid_t>::max() || currentPID < oldMin) {
        if (minPID.compare_exchange_weak(oldMin, currentPID)) {
            CrossPlatformTimer::log("MAIN PROCESS selected (lowest PID)");
            return true;
        }
    }
    return false;
}

// Запуск Copy1: +10 к счетчику (~100мс)
int CrossPlatformTimer::spawnCopy1() {
#ifdef _WIN32
    STARTUPINFOA si = {sizeof(si)};
    PROCESS_INFORMATION pi = {};
    
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    
    std::string cmdline = std::string(exePath) + " copy1";
    
    if (CreateProcessA(NULL, const_cast<char*>(cmdline.c_str()), 
                      NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return static_cast<int>(pi.dwProcessId);
    }
    return -1;
#else
    pid_t pid = fork();
    if (pid == 0) {
        execlp("./timer_lab", "timer_lab", "copy1", (char*)NULL);
        exit(1);
    } else if (pid > 0) {
        return static_cast<int>(pid);
    }
    return -1;
#endif
}

// Запуск Copy2: *2 -> 2с пауза -> /2 (~2.1с)
int CrossPlatformTimer::spawnCopy2() {
#ifdef _WIN32
    STARTUPINFOA si = {sizeof(si)};
    PROCESS_INFORMATION pi = {};
    
    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);
    
    std::string cmdline = std::string(exePath) + " copy2";
    
    if (CreateProcessA(NULL, const_cast<char*>(cmdline.c_str()), 
                      NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        return static_cast<int>(pi.dwProcessId);
    }
    return -1;
#else
    pid_t pid = fork();
    if (pid == 0) {
        execlp("./timer_lab", "timer_lab", "copy2", (char*)NULL);
        exit(1);
    } else if (pid > 0) {
        return static_cast<int>(pid);
    }
    return -1;
#endif
}

// Проверка активности процесса по PID
bool CrossPlatformTimer::isProcessRunning(pid_t pid) {
#ifdef _WIN32
    if (pid == 0) return false;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, static_cast<DWORD>(pid));
    if (hProcess == NULL) return false;
    
    DWORD exitCode = 0;
    bool active = GetExitCodeProcess(hProcess, &exitCode) && exitCode == STILL_ACTIVE;
    CloseHandle(hProcess);
    return active;
#else
    if (pid <= 0) return false;
    int status;
    pid_t result = waitpid(static_cast<pid_t>(pid), &status, WNOHANG);
    return result == 0;
#endif
}
