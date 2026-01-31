#pragma once
#include <string>
#include <cstdint>
#include <limits>

// Кроссплатформенный тип PID
#ifdef _WIN32
    typedef uint32_t pid_t;
#else
    #include <sys/types.h>
#endif

// Класс для работы с таймерами и процессами (Win/Linux)
class CrossPlatformTimer {
public:
    // Получение текущего времени с миллисекундами
    static std::string getCurrentTimeStr();
    
    // PID текущего процесса как строка
    static std::string getPIDStr();
    
    // Потокобезопасная запись в лог-файл
    static void log(const std::string& message);
    
    // Выбор главного процесса (минимальный PID)
    static bool isMainProcess();
    
    // Запуск дочерних процессов Copy1 (+10) и Copy2 (*2 -> /2)
    static int spawnCopy1();
    static int spawnCopy2();
    
    // Проверка активности процесса по PID
    static bool isProcessRunning(pid_t pid);
    
private:
    // Путь к лог-файлу build/logs/timer.log
    static std::string getLogPath();
};
