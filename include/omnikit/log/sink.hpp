#pragma once

#include "omnikit/log/log_level.hpp"

#include <string>

namespace omnikit::log {

class Sink {
public:
    Sink() = default;
    virtual ~Sink() = default;

    Sink(const Sink&) = delete;
    Sink& operator=(const Sink&) = delete;
    Sink(Sink&&) = delete;
    Sink& operator=(Sink&&) = delete;

    virtual void write(LogLevel level, const std::string& message) = 0;
};
} // namespace omnikit::log
