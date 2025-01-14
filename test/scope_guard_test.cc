#include <sugar/scope_guard.hh>
#include <catch2/catch.hpp>

TEST_CASE("Test scope guard") {
    bool flag = false;

    {
        ai::sugar::scope_guard guard{ [&] { flag = true; } };
        REQUIRE_FALSE(flag);
    }

    REQUIRE(flag);
}

TEST_CASE("Test scope guard with exception") {
    bool flag = false;

    try {
        ai::sugar::scope_guard guard([&] { flag = true; });
        throw std::runtime_error("Test exception");
    } catch (const std::runtime_error&) {
        REQUIRE(flag);
    }
}
