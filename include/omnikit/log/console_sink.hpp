#pragma once

#include "omnikit/log/sink.hpp"

#include <string>

namespace omnikit::log {

class ConsoleSink : public Sink {
public:
    void write(LogLevel level, const std::string& message) override;
};

} // namespace omnikit::log
