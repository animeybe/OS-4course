#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <chrono>
#include <string>
#include <iomanip>
#include <sstream>

struct TempReading {
    std::chrono::system_clock::time_point timestamp;
    double temperature;  // Temperature in C
    
    std::string toString() const {
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            timestamp.time_since_epoch());
        std::stringstream ss;
        auto tt = std::chrono::system_clock::to_time_t(timestamp);
        ss << std::put_time(std::localtime(&tt), "[%Y-%m-%d %H:%M:%S.");
        ss << ms.count() % 1000 << "] " 
           << std::fixed << std::setprecision(2) << temperature << " C";
        return ss.str();
    }
};

struct HourlyAvg {
    std::chrono::system_clock::time_point hour_start;
    double average_temp;
    int readings_count;
    
    std::string toString() const {
        std::stringstream ss;
        auto tt = std::chrono::system_clock::to_time_t(hour_start);
        ss << std::put_time(std::localtime(&tt), "%Y-%m-%d %H:00:00") 
           << " " << std::fixed << std::setprecision(2) << average_temp 
           << " C (" << readings_count << " readings)";
        return ss.str();
    }
};

struct DailyAvg {
    std::chrono::system_clock::time_point day_start;
    double average_temp;
    int hours_count;
    
    std::string toString() const {
        std::stringstream ss;
        auto tt = std::chrono::system_clock::to_time_t(day_start);
        ss << std::put_time(std::localtime(&tt), "%Y-%m-%d") 
           << " " << std::fixed << std::setprecision(2) << average_temp 
           << " C (" << hours_count << " hours)";
        return ss.str();
    }
};
