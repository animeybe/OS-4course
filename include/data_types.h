#pragma once
#include <chrono>
#include <string>

struct TempReading {
    std::chrono::system_clock::time_point timestamp;
    double temperature;  // °C
    std::string toString() const;
};

struct HourlyAvg {
    std::chrono::system_clock::time_point hour_start;
    double average_temp;
    int readings_count;
};

struct DailyAvg {
    std::chrono::system_clock::time_point day_start;
    double average_temp;
    int hours_count;
};