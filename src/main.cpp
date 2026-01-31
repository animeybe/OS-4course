#include "temp_logger.h"
#include "temp_simulator.h"
#include <iostream>
#include <csignal>

volatile bool running = true;
void signalHandler(int) { running = false; }  // Graceful shutdown

int main() {
    std::signal(SIGINT, signalHandler);
    
    TempLogger logger("logs");  // RAII: auto-finalizes on exit
    TempSimulator sim(23.0, 2.0);  // Device simulator: 23C ±2C

    std::cout << "Temperature Logger started\n";
    std::cout << "Raw log:     logs/raw_temps.log (24h)\n";
    std::cout << "Hourly log:  logs/hourly_avg.log (1 month)\n";
    std::cout << "Daily log:   logs/daily_avg.log (1 year)\n";
    std::cout << "Simulating device... Press Ctrl+C to stop\n\n";

    while (running) {
        auto reading = sim.getNextReading();  // Read every 10s
        logger.logTemperature(reading);
        std::cout << reading.toString() << std::endl;
    }

    std::cout << "\nTemperature Logger stopped. All buffers finalized by destructor.\n";
    return 0;  // ~TempLogger() automatically writes incomplete buffers
}
