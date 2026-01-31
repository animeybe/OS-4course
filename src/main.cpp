#include "temp_logger.h"
#include "temp_simulator.h"
#include <iostream>
#include <csignal>
#include <thread>

volatile bool running = true;
void signalHandler(int) { running = false; }

int main() {
    signal(SIGINT, signalHandler);

    TempLogger logger("logs");
    TempSimulator sim(23.0, 2.0);

    std::cout << "Temperature Logger started\n";
    std::cout << "Raw log:     logs/raw_temps.log (24ч)\n";
    std::cout << "Hourly log:  logs/hourly_avg.log (1мес)\n";
    std::cout << "Daily log:   logs/daily_avg.log (1год)\n";
    std::cout << "Simulating device... Press Ctrl+C to stop\n";

    while (running) {
        auto reading = sim.getNextReading();
        logger.logTemperature(reading);
        std::cout << reading.toString() << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Небольшая пауза
    }

    std::cout << "\nTemperature Logger stopped\n";
    return 0;
}
