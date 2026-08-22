#include "omnikit/log/logger.hpp"
#include "omnikit/log/sink.hpp"
#include "test_framework.hpp"

#include <memory>
#include <string>
#include <vector>

namespace omnikit::log {

class MockSink : public Sink {
public:
    struct Record {
        LogLevel level;
        std::string message;
    };

    void write(LogLevel level, const std::string& message) override {
        records_.push_back({level, message});
    }

    const std::vector<Record>& records() const { return records_; }

private:
    std::vector<Record> records_;
};

} // namespace omnikit::log

using omnikit::log::Logger;
using omnikit::log::LogLevel;
using omnikit::log::MockSink;

void testForwardsEachLevel() {
    std::printf("testForwardsEachLevel\n");

    auto sink = std::make_shared<MockSink>();
    Logger logger(sink);

    logger.debug("d");
    logger.info("i");
    logger.warn("w");
    logger.error("e");

    const auto& records = sink->records();
    REQUIRE(records.size() == 4);

    CHECK(records[0].level == LogLevel::DEBUG);
    CHECK(records[0].message == "d");
    CHECK(records[1].level == LogLevel::INFO);
    CHECK(records[2].level == LogLevel::WARN);
    CHECK(records[3].level == LogLevel::ERROR);
    CHECK(records[3].message == "e");
}

// ── Test: sink can be swapped at runtime ────────────────────────
void testSwapSink() {
    std::printf("testSwapSink\n");

    auto first = std::make_shared<MockSink>();
    auto second = std::make_shared<MockSink>();

    Logger logger(first);
    logger.info("to first");

    logger.setSink(second);
    logger.info("to second");

    REQUIRE(first->records().size() == 1);
    REQUIRE(second->records().size() == 1);
    CHECK(first->records()[0].message == "to first");
    CHECK(second->records()[0].message == "to second");
}

// ── Test: a null sink is ignored, not crashed on ────────────────
void testNullSinkIsSafe() {
    std::printf("testNullSinkIsSafe\n");

    Logger logger(nullptr);
    logger.info("safely ignored");
    CHECK(true);
}

int main() {
    testForwardsEachLevel();
    testSwapSink();
    testNullSinkIsSafe();

    using namespace omnikit::test;
    std::printf("\n%d checks, %d failed\n", g_checks, g_failures);
    return g_failures == 0 ? 0 : 1;
}
