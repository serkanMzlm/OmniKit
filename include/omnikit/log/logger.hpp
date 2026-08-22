#pragma once

#include "omnikit/log/log_level.hpp"
#include "omnikit/log/sink.hpp"

#include <memory>
#include <string>

namespace omnikit::log {

class Logger {
public:
    explicit Logger(std::shared_ptr<Sink> sink);

    void debug(const std::string& message);
    void info(const std::string& message);
    void warn(const std::string& message);
    void error(const std::string& message);

    void log(LogLevel level, const std::string& message);

    void setSink(std::shared_ptr<Sink> sink);

private:
    std::shared_ptr<Sink> sink_;
};

} // namespace omnikit::log
