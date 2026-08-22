#include "omnikit/log/console_sink.hpp"

#include <iostream>

namespace omnikit::log {
namespace {
constexpr const char* COLOR_RESET = "\033[0m";
constexpr const char* COLOR_GREEN = "\033[32m";  // Debug
constexpr const char* COLOR_YELLOW = "\033[33m"; // Warn
constexpr const char* COLOR_RED = "\033[31m";    // Error

const char* colorFor(LogLevel level) {
    switch (level) {
    case LogLevel::DEBUG:
        return COLOR_GREEN;
    case LogLevel::WARN:
        return COLOR_YELLOW;
    case LogLevel::ERROR:
        return COLOR_RED;
    case LogLevel::INFO:
    default:
        return ""; // no color: use terminal default
    }
}
} // namespace

void ConsoleSink::write(LogLevel level, const std::string& message) {
    const char* color = colorFor(level);

    // Errors and warnings conventionally go to stderr; the rest to stdout.
    std::ostream& out =
        (level == LogLevel::ERROR || level == LogLevel::WARN) ? std::cerr : std::cout;

    out << color << '[' << toString(level) << "] " << message << COLOR_RESET << '\n';
}

} // namespace omnikit::log
