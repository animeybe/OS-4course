#include "temp_logger.h"
#include <fstream>
#include <numeric>
#include <algorithm>
#include <ctime>

TempLogger::TempLogger(const std::string& logs_dir) : logs_dir_(logs_dir) {
    std::filesystem::create_directories(logs_dir_);  // Create logs directory
    current_hour_start_ = std::chrono::system_clock::now();
    current_day_start_ = std::chrono::system_clock::now();
}

// Auto-finalize buffers on program exit
TempLogger::~TempLogger() {
    if (!hourly_buffer_.empty()) {
        // Write incomplete hour
        double sum = std::accumulate(hourly_buffer_.begin(), hourly_buffer_.end(), 0.0,
            [](double acc, const TempReading& r) { return acc + r.temperature; });
        HourlyAvg avg{current_hour_start_, sum / hourly_buffer_.size(), 
                     static_cast<int>(hourly_buffer_.size())};
        appendHourly(avg);
        daily_buffer_.push_back(avg);
    }
    
    if (!daily_buffer_.empty()) {
        // Write incomplete day
        double sum = std::accumulate(daily_buffer_.begin(), daily_buffer_.end(), 0.0,
            [](double acc, const HourlyAvg& h) { return acc + h.average_temp; });
        DailyAvg day_avg{current_day_start_, sum / daily_buffer_.size(), 
                        static_cast<int>(daily_buffer_.size())};
        appendDaily(day_avg);
    }
    
    cleanupLogs();
}

void TempLogger::logTemperature(const TempReading& reading) {
    appendRaw(reading);  // Always log raw data

    // Detect hour change
    auto reading_hour = std::chrono::time_point_cast<std::chrono::hours>(reading.timestamp);
    if (reading_hour > current_hour_start_) {
        if (!hourly_buffer_.empty()) {
            // Calculate and save hourly average
            double sum = std::accumulate(hourly_buffer_.begin(), hourly_buffer_.end(), 0.0,
                [](double acc, const TempReading& r) { return acc + r.temperature; });
            HourlyAvg avg{current_hour_start_, sum / hourly_buffer_.size(), 
                         static_cast<int>(hourly_buffer_.size())};
            appendHourly(avg);
            daily_buffer_.push_back(avg);
        }
        hourly_buffer_.clear();
        current_hour_start_ = reading_hour;
    }
    hourly_buffer_.push_back(reading);

    // Detect day change (C++17 compatible)
    std::time_t now_t = std::chrono::system_clock::to_time_t(reading.timestamp);
    struct std::tm timeinfo = *std::localtime(&now_t);
    timeinfo.tm_hour = 0; timeinfo.tm_min = 0; timeinfo.tm_sec = 0; timeinfo.tm_isdst = -1;
    std::time_t day_start_t = std::mktime(&timeinfo);
    auto reading_day_start = std::chrono::system_clock::from_time_t(day_start_t);

    if (reading_day_start > current_day_start_ && !daily_buffer_.empty()) {
        // Calculate and save daily average
        double sum = std::accumulate(daily_buffer_.begin(), daily_buffer_.end(), 0.0,
            [](double acc, const HourlyAvg& h) { return acc + h.average_temp; });
        DailyAvg day_avg{current_day_start_, sum / daily_buffer_.size(), 
                        static_cast<int>(daily_buffer_.size())};
        appendDaily(day_avg);
        daily_buffer_.clear();
        current_day_start_ = reading_day_start;
    }

    // Periodic cleanup
    static int cleanup_counter = 0;
    if (++cleanup_counter % 100 == 0) {
        cleanupLogs();
    }
}

void TempLogger::appendRaw(const TempReading& reading) {
    std::ofstream file(logs_dir_ + "/raw_temps.log", std::ios::app);
    file << reading.toString() << "\n";
}

void TempLogger::appendHourly(const HourlyAvg& avg) {
    std::ofstream file(logs_dir_ + "/hourly_avg.log", std::ios::app);
    file << avg.toString() << "\n";
}

void TempLogger::appendDaily(const DailyAvg& avg) {
    std::ofstream file(logs_dir_ + "/daily_avg.log", std::ios::app);
    file << avg.toString() << "\n";
}

void TempLogger::cleanupLogs() {
    cleanupRaw24h();
    cleanupHourlyMonth();
    cleanupDailyYear();
}

void TempLogger::cleanupRaw24h() {
    // Size-based cleanup: 10MB limit for 24h raw data
    std::filesystem::path file_path = logs_dir_ + "/raw_temps.log";
    if (std::filesystem::exists(file_path) && 
        std::filesystem::file_size(file_path) > 10 * 1024 * 1024) {
        std::filesystem::remove(file_path);
    }
}

void TempLogger::cleanupHourlyMonth() {
    // Size-based cleanup: 5MB limit for 1 month hourly data
    std::filesystem::path file_path = logs_dir_ + "/hourly_avg.log";
    if (std::filesystem::exists(file_path) && 
        std::filesystem::file_size(file_path) > 5 * 1024 * 1024) {
        std::filesystem::remove(file_path);
    }
}

void TempLogger::cleanupDailyYear() {
    // Size-based cleanup: 1MB limit for 1 year daily data
    std::filesystem::path file_path = logs_dir_ + "/daily_avg.log";
    if (std::filesystem::exists(file_path) && 
        std::filesystem::file_size(file_path) > 1 * 1024 * 1024) {
        std::filesystem::remove(file_path);
    }
}
