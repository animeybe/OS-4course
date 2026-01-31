#pragma once
#include "data_types.h"

class TempSimulator {
public:
    TempSimulator(double base_temp = 23.0, double variation = 2.0);
    TempReading getNextReading();  // Симуляция каждые 10с
private:
    double base_temp, variation;
    std::chrono::system_clock::time_point last_reading;
};