#include  "bus_interface.hpp"

void BusInterface::setTimeout(uint32_t timeout) {
    _timeout_ms = std::min(std::max(timeout, MIN_TIMEOUT_MS), MAX_TIMEOUT_MS);
}

void BusInterface::setOpenPort(bool is_open) {
    _is_open = is_open;
}
