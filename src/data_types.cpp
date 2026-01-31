#include "data_types.h"
#include <iomanip>
#include <sstream>

std::string TempReading::toString() const {
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        timestamp.time_since_epoch());
    std::stringstream ss;
    auto tt = std::chrono::system_clock::to_time_t(timestamp);
    ss << std::put_time(std::localtime(&tt), "%Y-%m-%d %H:%M:%S.");
    ss << ms.count() % 1000 << "] " << std::fixed << std::setprecision(2)
       << temperature << "°C";
    return ss.str();
}
