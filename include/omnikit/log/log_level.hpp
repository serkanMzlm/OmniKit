#pragma once

namespace omnikit::log {

enum class LogLevel { DEBUG, INFO, WARN, ERROR };

const char* toString(LogLevel level);

} // namespace omnikit::log
