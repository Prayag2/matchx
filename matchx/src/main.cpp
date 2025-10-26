#include <cstdlib>
#include <iostream>
#include <sw/redis++/redis++.h>
#include <engine/matching_engine.hpp>
#include <plog/Log.h>
#include <plog/Initializers/ConsoleInitializer.h>
#include <plog/Initializers/RollingFileInitializer.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include <plog/Formatters/TxtFormatter.h>

int main() {
    plog::init(plog::debug, "debug-log.txt");
    plog::init<plog::TxtFormatter>(plog::info, plog::streamStdOut);
    plog::ColorConsoleAppender<plog::TxtFormatter>(streamStdOut);

    PLOGI << "Starting Engine";
    MatchingEngine engine{};
    engine.run();

    return 0;
}
