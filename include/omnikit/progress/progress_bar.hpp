#pragma once

#include "omnikit/progress/bar_style.hpp"

#include <memory>

namespace omnikit::progress {

class ProgressBar {
public:
    /// @param total  the value that represents 100% (must be > 0)
    /// @param style  how the bar looks (Strategy); if null, nothing draws
    /// @param width  bar body width in character cells
    ProgressBar(double total, std::shared_ptr<BarStyle> style, int width = 50);

    /// Report the current progress value (0..total) and redraw the bar.
    void update(double current);

    /// Finish the bar (jump to 100%) and move to a fresh line.
    void finish();

    /// Swap the look at runtime.
    void setStyle(std::shared_ptr<BarStyle> style);

private:
    void draw(double ratio) const;

    double total_;
    int width_;
    std::shared_ptr<BarStyle> style_;
};

} // namespace omnikit::progress
