#include "omnikit/log/file_sink.hpp"

#include <stdexcept>

namespace omnikit::log {

FileSink::FileSink(const std::string& path) : stream_(path, std::ios::out | std::ios::app) {
    if (!stream_.is_open()) {
        throw std::runtime_error("FileSink: cannot open file: " + path);
    }
}

void FileSink::write(LogLevel level, const std::string& message) {
    stream_ << '[' << toString(level) << "] " << message << '\n';
    stream_.flush();
}

} // namespace omnikit::log
