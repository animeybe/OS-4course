#pragma once
#include "data_types.h"

class TempSimulator {
public:
    TempSimulator(double base_temp = 23.0, double variation = 2.0);  // Default: 23C ±2C
    TempReading getNextReading();  // Simulate device reading every 10s
    
private:
    double base_temp_;     // Base temperature
    double variation_;     // Random variation
    std::chrono::system_clock::time_point last_reading_;  // Last reading time
};
