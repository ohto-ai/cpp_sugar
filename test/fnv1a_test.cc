#include <sugar/fnv1a.hh>
#include <catch2/catch.hpp>

TEST_CASE("Test fnv1a") {
    SECTION("Test fnv1a hash") {
        using namespace ai::sugar::hash::fnv1a;

        REQUIRE(fnv1a_32("hello") == 0x4f9f2cab);
        REQUIRE(fnv1a_32("world") == 0x37a3e893);
        REQUIRE(fnv1a_32("hello, world!") == 0x8a0c8a74);
    }
}
