#pragma once
#include "data_types.h"
#include <string>
#include <vector>

class TempLogger {
public:
    TempLogger(const std::string& logs_dir);
    
    // Основной цикл
    void logTemperature(const TempReading& reading);
    void cleanupLogs();  // Автоочистка по расписанию
    
private:
    // 3 лог-файла
    void appendRaw(const TempReading& reading);   // 24ч
    void appendHourly(const HourlyAvg& avg);      // 1мес
    void appendDaily(const DailyAvg& avg);        // 1год
    
    // Очистка по времени
    void cleanupRaw24h();
    void cleanupHourlyMonth();
    void cleanupDailyYear();
    
    std::string logs_dir_;
    std::vector<TempReading> hourly_buffer_;  // Буфер часа
    std::vector<HourlyAvg> daily_buffer_;     // Буфер дня
};
