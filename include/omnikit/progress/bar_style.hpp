#pragma once

#include <string>

namespace omnikit::progress {

class BarStyle {
public:
    BarStyle() = default;

    BarStyle(const BarStyle&) = delete;
    BarStyle& operator=(const BarStyle&) = delete;
    BarStyle(BarStyle&&) = delete;
    BarStyle& operator=(BarStyle&&) = delete;

    virtual ~BarStyle() = default;
    virtual std::string render(double ratio, int width) const = 0;
};

class ClassicStyle : public BarStyle {
public:
    std::string render(double ratio, int width) const override;
};

class BlockStyle : public BarStyle {
public:
    std::string render(double ratio, int width) const override;
};

class AsciiStyle : public BarStyle {
public:
    std::string render(double ratio, int width) const override;
};
} // namespace omnikit::progress
