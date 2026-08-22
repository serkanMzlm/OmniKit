#include "omnikit/log/logger.hpp"

#include <utility>

namespace omnikit::log {

Logger::Logger(std::shared_ptr<Sink> sink) : sink_(std::move(sink)) {}

void Logger::log(LogLevel level, const std::string& message) {
    if (sink_) {
        sink_->write(level, message);
    }
}

void Logger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warn(const std::string& message) {
    log(LogLevel::WARN, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void Logger::setSink(std::shared_ptr<Sink> sink) {
    sink_ = std::move(sink);
}

} // namespace omnikit::log
