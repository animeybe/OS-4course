#pragma once
#include "data_types.h"
#include <string>
#include <vector>
#include <chrono>
#include <filesystem>

class TempLogger {
public:
    TempLogger(const std::string& logs_dir);  // Initialize with logs directory
    ~TempLogger();  // Auto-finalize buffers on exit
    
    void logTemperature(const TempReading& reading);  // Main logging entry
    void cleanupLogs();  // Manual cleanup
    
private:
    // Log append methods
    void appendRaw(const TempReading& reading);      // Raw data log (24h)
    void appendHourly(const HourlyAvg& avg);         // Hourly averages (1 month)
    void appendDaily(const DailyAvg& avg);           // Daily averages (1 year)
    
    // Cleanup old data
    void cleanupRaw24h();       // Remove raw data >24h (size-based)
    void cleanupHourlyMonth();  // Remove hourly data >1 month
    void cleanupDailyYear();    // Remove daily data >1 year
    
    // Internal state
    std::chrono::system_clock::time_point current_hour_start_;
    std::vector<TempReading> hourly_buffer_;     // Current hour buffer
    std::chrono::system_clock::time_point current_day_start_;
    std::vector<HourlyAvg> daily_buffer_;        // Current day buffer
    std::string logs_dir_;
};
