#include "omnikit/progress/progress_bar.hpp"

#include <cstdio>
#include <iostream>
#include <utility>

namespace omnikit::progress {

ProgressBar::ProgressBar(double total, std::shared_ptr<BarStyle> style, int width)
    : total_(total > 0.0 ? total : 1.0), // guard against divide-by-zero
      width_(width), style_(std::move(style)) {}

void ProgressBar::update(double current) {
    const double ratio = current / total_;
    draw(ratio);
}

void ProgressBar::finish() {
    draw(1.0);
    std::cout << '\n';
}

void ProgressBar::setStyle(std::shared_ptr<BarStyle> style) {
    style_ = std::move(style);
}

void ProgressBar::draw(double ratio) const {
    if (!style_) {
        return; // no style, nothing to draw
    }

    if (ratio < 0.0) {
        ratio = 0.0;
    }
    if (ratio > 1.0) {
        ratio = 1.0;
    }

    const std::string body = style_->render(ratio, width_);
    const int percent = static_cast<int>(ratio * 100.0);

    // '\r' returns to line start so we overwrite the same line.
    std::cout << '\r' << '|' << body << "| " << percent << '%';
    std::cout.flush();
}

} // namespace omnikit::progress
