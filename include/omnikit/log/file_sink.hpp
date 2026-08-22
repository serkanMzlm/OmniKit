#pragma once

#include "omnikit/log/sink.hpp"

#include <fstream>
#include <string>

namespace omnikit::log {

class FileSink : public Sink {
public:
    explicit FileSink(const std::string& path);
    void write(LogLevel level, const std::string& message) override;

private:
    std::ofstream stream_;
};

} // namespace omnikit::log
