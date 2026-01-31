#include "temp_simulator.h"
#include <random>
#include <thread>
#include <chrono>

TempSimulator::TempSimulator(double base_temp, double variation)
    : base_temp(base_temp), variation(variation), last_reading(std::chrono::system_clock::now()) {}

TempReading TempSimulator::getNextReading() {
    // Ждем 10 секунд между измерениями
    auto now = std::chrono::system_clock::now();
    if (now - last_reading < std::chrono::seconds(10)) {
        std::this_thread::sleep_for(std::chrono::seconds(10) - (now - last_reading));
    }
    last_reading = std::chrono::system_clock::now();

    // Генерируем температуру: base ± variation
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-variation, variation);
    double temp = base_temp + dis(gen);

    TempReading reading{last_reading, temp};
    return reading;
}
