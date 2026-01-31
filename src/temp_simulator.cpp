#include "temp_simulator.h"
#include <random>
#include <thread>
#include <chrono>

TempSimulator::TempSimulator(double base_temp, double variation)
    : base_temp_(base_temp), variation_(variation), 
      last_reading_(std::chrono::system_clock::now()) {}

TempReading TempSimulator::getNextReading() {
    // Enforce 10s interval
    auto now = std::chrono::system_clock::now();
    auto interval = std::chrono::seconds(10);
    if (now - last_reading_ < interval) {
        std::this_thread::sleep_for(interval - (now - last_reading_));
    }
    last_reading_ = std::chrono::system_clock::now();

    // Generate realistic temperature: base ± variation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-variation_, variation_);
    double temp = base_temp_ + dis(gen);

    return {last_reading_, temp};
}
