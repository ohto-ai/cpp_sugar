#include <sugar/string_pool.hh>
#include <catch2/catch.hpp>

TEST_CASE("Test string pool") {
    ai::sugar::string_pool pool;

    SECTION("Test intern const char*") {
        auto sv1 = pool.intern("Hello");
        auto sv2 = pool.intern("Hello");
        REQUIRE(sv1 == sv2);
    }

    SECTION("Test intern std::string") {
        auto sv1 = pool.intern(std::string("Hello"));
        auto sv2 = pool.intern(std::string("Hello"));
        REQUIRE(sv1 == sv2);
    }

    SECTION("Test intern std::string_view") {
        auto sv1 = pool.intern(std::string_view("Hello"));
        auto sv2 = pool.intern(std::string_view("Hello"));
        REQUIRE(sv1 == sv2);
    }
}
