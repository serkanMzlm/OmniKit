#include "framework.hpp"
#include "omnikit/progress/progress_bar.hpp"

#include <memory>
#include <string>

using omnikit::progress::AsciiStyle;
using omnikit::progress::BlockStyle;
using omnikit::progress::ClassicStyle;
using omnikit::progress::ProgressBar;

void testClassicStyle() {
    ClassicStyle style;

    CHECK(style.render(0.0, 10) == ">         ");
    CHECK(style.render(1.0, 10) == "==========");
    CHECK(style.render(0.5, 10) == "=====>    ");
}

void testAsciiStyle() {
    AsciiStyle style;

    CHECK(style.render(-0.5, 10) == "..........");
    CHECK(style.render(0.0, 10) == "..........");
    CHECK(style.render(1.0, 10) == "##########");
    CHECK(style.render(0.5, 10) == "#####.....");
}

void testBlockStyleFillCount() {
    BlockStyle style;

    CHECK(style.render(0.0, 10).size() == 10);
    CHECK(style.render(1.0, 10).size() == 30);
}

void testProgressBarNullStyleIsSafe() {
    ProgressBar bar(100.0, nullptr);
    bar.update(50.0); // should be safely ignored, no crash
    CHECK(true);
}

int main() {
    testClassicStyle();
    testAsciiStyle();
    testBlockStyleFillCount();
    testProgressBarNullStyleIsSafe();

    using namespace omnikit::test;
    std::printf("\n%d checks, %d failed\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}
