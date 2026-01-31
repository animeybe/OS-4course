#include "temp_logger.h"
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <iomanip>
#include <sstream>

TempLogger::TempLogger(const std::string& logs_dir) : logs_dir_(logs_dir) {
    std::filesystem::create_directories(logs_dir_);
}

void TempLogger::logTemperature(const TempReading& reading) {
    appendRaw(reading);  // Всегда пишем сырые данные

    // Буферизация по часу
    auto hour_start = std::chrono::time_point_cast<std::chrono::hours>(
        reading.timestamp);
    
    if (hourly_buffer_.empty() || 
        hour_start != std::chrono::time_point_cast<std::chrono::hours>(
            hourly_buffer_.back().hour_start)) {
        // Новый час - вычисляем предыдущий
        if (!hourly_buffer_.empty()) {
            double sum = 0;
            for (const auto& r : hourly_buffer_) sum += r.temperature;
            HourlyAvg avg{hourly_buffer_[0].hour_start, 
                         sum / hourly_buffer_.size(), 
                         static_cast<int>(hourly_buffer_.size())};
            appendHourly(avg);
            daily_buffer_.push_back(avg);
        }
        hourly_buffer_.clear();
    }
    hourly_buffer_.push_back(reading);

    cleanupLogs();  // Проверяем очистку
}

void TempLogger::appendRaw(const TempReading& reading) {
    std::ofstream file(logs_dir_ + "/raw_temps.log", std::ios::app);
    file << reading.toString() << "\n";
}

void TempLogger::appendHourly(const HourlyAvg& avg) {
    std::ofstream file(logs_dir_ + "/hourly_avg.log", std::ios::app);
    auto tt = std::chrono::system_clock::to_time_t(avg.hour_start);
    file << std::put_time(std::localtime(&tt), "%Y-%m-%d %H:00:00") 
         << " " << std::fixed << std::setprecision(2) << avg.average_temp 
         << "°C (" << avg.readings_count << " измерений)\n";
}

void TempLogger::cleanupLogs() {
    cleanupRaw24h();
    cleanupHourlyMonth();
    cleanupDailyYear();
}

void TempLogger::cleanupRaw24h() {
    auto cutoff = std::chrono::system_clock::now() - std::chrono::hours(24);
    // Логика очистки: переписать файл без старых записей
    // (упрощено - полная реализация требует парсинга лога)
}

void TempLogger::cleanupHourlyMonth() {
    auto cutoff = std::chrono::system_clock::now() - std::chrono::hours(24*30);
    // Аналогично
}

void TempLogger::cleanupDailyYear() {
    auto cutoff = std::chrono::system_clock::now() - std::chrono::hours(24*365);
    // Аналогично
}
