#pragma once

namespace omnikit::test {

inline int g_checks = 0;
inline int g_failures = 0;

} // namespace omnikit::test

#define CHECK(cond)                                                                                \
    do {                                                                                           \
        ++omnikit::test::g_checks;                                                                 \
        if (!(cond)) {                                                                             \
            ++omnikit::test::g_failures;                                                           \
            std::printf("  [FAIL] %s:%d  CHECK(%s)\n", __FILE__, __LINE__, #cond);                 \
        }                                                                                          \
    } while (0)

#define REQUIRE(cond)                                                                              \
    do {                                                                                           \
        ++omnikit::test::g_checks;                                                                 \
        if (!(cond)) {                                                                             \
            ++omnikit::test::g_failures;                                                           \
            std::printf("  [FAIL] %s:%d  REQUIRE(%s)\n", __FILE__, __LINE__, #cond);               \
            return;                                                                                \
        }                                                                                          \
    } while (0)
