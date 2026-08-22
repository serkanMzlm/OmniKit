#include "omnikit/log/log_level.hpp"

namespace omnikit::log {

const char* toString(LogLevel level) {
    switch (level) {
    case LogLevel::DEBUG:
        return "DEBUG";
    case LogLevel::INFO:
        return "INFO";
    case LogLevel::WARN:
        return "WARN";
    case LogLevel::ERROR:
        return "ERROR";
    }
    return "UNKNOWN";
}

} // namespace omnikit::log
