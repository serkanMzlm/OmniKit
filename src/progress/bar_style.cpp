#include "omnikit/progress/bar_style.hpp"

namespace {
constexpr const char* FULL_BLOCK = "\u2588";
}

namespace omnikit::progress {

std::string ClassicStyle::render(double ratio, int width) const {
    if (ratio < 0.0) {
        ratio = 0.0;
    }
    if (ratio > 1.0) {
        ratio = 1.0;
    }

    const int filled = static_cast<int>(ratio * width);

    std::string bar;
    bar.reserve(static_cast<std::size_t>(width));

    for (int i = 0; i < width; ++i) {
        if (i < filled) {
            bar += '=';
        } else if (i == filled) {
            bar += '>';
        } else {
            bar += ' ';
        }
    }
    return bar;
}

std::string BlockStyle::render(double ratio, int width) const {
    if (ratio < 0.0) {
        ratio = 0.0;
    }
    if (ratio > 1.0) {
        ratio = 1.0;
    }

    const int filled = static_cast<int>(ratio * width);

    std::string bar;
    bar.reserve(static_cast<std::size_t>(width) * 3);

    for (int i = 0; i < width; ++i) {
        if (i < filled) {
            bar += FULL_BLOCK;
        } else {
            bar += ' ';
        }
    }
    return bar;
}

std::string AsciiStyle::render(double ratio, int width) const {
    if (ratio < 0.0) {
        ratio = 0.0;
    }
    if (ratio > 1.0) {
        ratio = 1.0;
    }

    const int filled = static_cast<int>(ratio * width);

    std::string bar;
    bar.reserve(static_cast<std::size_t>(width));

    for (int i = 0; i < width; ++i) {
        bar += (i < filled) ? '#' : '.';
    }
    return bar;
}
} // namespace omnikit::progress
