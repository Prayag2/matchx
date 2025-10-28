#pragma once

#include <string>
#include <chrono>
#include <common/types.hpp>

namespace Utils {
    /**
     * @brief Generates a timestamp string from a given timestamp
     * @param timestamp This is of type `Time` and stores epoch duration in microseconds
     * @return std::string
     */
    inline std::string getTimeString(Time timestamp) {
        std::chrono::microseconds duration{timestamp};
        auto timePoint{std::chrono::system_clock::time_point{duration}};
        std::string timeString{std::format("{:%Y-%m-%dT%H:%M:%S}", timePoint)};

        return timeString;
    };

    /**
     * @brief Returns the unix epoch duration in microseconds
     * @return `Time`
     */
    inline Time now() {
        auto now{std::chrono::high_resolution_clock::now()};
        Time duration{std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count()};

        return duration;
    }
}
