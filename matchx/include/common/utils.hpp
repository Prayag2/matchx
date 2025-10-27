#pragma once

#include <ratio>
#include <string>
#include <chrono>
#include <common/types.hpp>

namespace Utils {
    inline std::string getTimeString(Time timestamp) {
        std::chrono::microseconds duration{timestamp};
        auto timePoint{std::chrono::system_clock::time_point{duration}};
        std::string timeString{std::format("{:%Y-%m-%dT%H:%M:%S}", timePoint)};

        return timeString;
    };

    inline Time now() {
        auto now{std::chrono::high_resolution_clock::now()};
        Time duration{std::chrono::duration_cast<std::chrono::microseconds>(now.time_since_epoch()).count()};

        return duration;
    }
}
