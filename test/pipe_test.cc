#include <sugar/pipe.hh>
#include <catch2/catch.hpp>
#include <iostream>
#include <vector>
#include <array>

TEST_CASE("Pipe operator tests", "pipe") {
    using ohtoai::sugar::pipe::operator|;

    SECTION("Test pipe for basic types") {
        auto add_one = [](int x) { return x + 1; };
        auto square = [](int x) { return x * x; };

        REQUIRE((1 | add_one | square) == 4);
        REQUIRE((2 | add_one | square) == 9);
        REQUIRE((3 | add_one | square) == 16);
    }

    SECTION("Test pipe for ostream") {
        std::ostringstream oss;
        oss << "Hello, ";
        oss | std::endl;
        oss << "world!";
        REQUIRE(oss.str() == "Hello, \nworld!");
    }
}

TEST_CASE("Pipe functor tests", "functor") {
    using namespace ohtoai::sugar::pipe;

    SECTION("Test filter") {
        auto is_even = [](int x) { return x % 2 == 0; };

        std::vector<int> v{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        auto result = v | filter(is_even);
        REQUIRE(result == std::vector<int>{2, 4, 6, 8, 10});
    }

    SECTION("Test for_each") {
        std::vector<int> v{ 1, 2, 3, 4, 5 };
        std::vector<int> result;
        auto print_and_store = [&](int x) { result.push_back(x); };

        v | for_each(print_and_store);

        REQUIRE(result == std::vector<int>{1, 2, 3, 4, 5});

        std::string s = "Hello, world!";
        auto result_str = s | for_each([](char& c) { c = std::toupper(c); });
        REQUIRE(result_str == "HELLO, WORLD!");
        REQUIRE(s == "Hello, world!");
    }

    SECTION("Test transform") {
        std::vector<int> v{ 1, 2, 3, 4, 5 };
        auto square = [](int x) { return x * x; };

        auto result = v | transform(v.begin(), square);

        REQUIRE(result == std::vector<int>{1, 4, 9, 16, 25});
        REQUIRE(v == std::vector<int>{1, 4, 9, 16, 25});
    }

    SECTION("Test all_of") {
        auto is_positive = [](int x) { return x > 0; };

        std::vector<int> v{ 1, 2, 3, 4, 5 };
        REQUIRE((v | all_of(is_positive)));

        std::vector<int> v2{ 1, -2, 3, -4, 5 };
        REQUIRE_FALSE((v2 | all_of(is_positive)));
    }

    SECTION("Test any_of") {
        auto is_negative = [](int x) { return x < 0; };

        std::vector<int> v{ 1, 2, 3, 4, 5 };
        REQUIRE_FALSE((v | any_of(is_negative)));

        std::vector<int> v2{ 1, -2, 3, -4, 5 };
        REQUIRE((v2 | any_of(is_negative)));
    }

    SECTION("Test none_of") {
        auto is_negative = [](int x) { return x < 0; };

        std::vector<int> v{ 1, 2, 3, 4, 5 };
        REQUIRE((v | none_of(is_negative)));

        std::vector<int> v2{ 1, -2, 3, -4, 5 };
        REQUIRE_FALSE((v2 | none_of(is_negative)));
    }

    SECTION("Test reverse") {
        const std::vector<int> v{ 1, 2, 3, 4, 5 };
        REQUIRE((v | reverse) == std::vector<int>{5, 4, 3, 2, 1});

        const std::string s = "Hello, world!";
        REQUIRE((s | reverse) == "!dlrow ,olleH");
    }

    SECTION("Test sort") {
        std::vector<int> v{ 3, 1, 4, 1, 5, 9, 2, 6 };
        REQUIRE((v | sort) == std::vector<int>{1, 1, 2, 3, 4, 5, 6, 9});
    }

    SECTION("Test unique") {
        std::vector<int> v{ 1, 2, 2, 3, 3, 3, 4, 5, 5 };
        REQUIRE((v | unique) == std::vector<int>{1, 2, 3, 4, 5});
    }

    SECTION("Test copy") {
        std::vector<int> v{ 1, 2, 3 };
        std::vector<int> dest;
        v | copy(std::back_inserter(dest));
        REQUIRE(dest == v);
    }

    SECTION("Test nop") {
        std::vector<int> v{ 1, 2, 3 };
        bool nop_called = false;
        auto result = v | nop([&nop_called](const auto& v) { nop_called = true; });
        REQUIRE(result == v);
    }

    SECTION("Test read_vector") {
        auto str_vec = "Hello, world!" | read_vector<std::string>;
        REQUIRE(str_vec == std::vector<std::string>{ "Hello,", "world!" });

        auto int_vec = "01 2 3 4 5 abc" | read_vector<int>;
        REQUIRE(int_vec == std::vector<int>{ 1, 2, 3, 4, 5 });

        auto double_vec = "1.1 2.2 3.3 4.4 5.5 xyz" | read_vector<double>;
        REQUIRE(double_vec == std::vector<double>{ 1.1, 2.2, 3.3, 4.4, 5.5 });

        auto char_vec = "Hello, world!" | read_vector<char>;
        REQUIRE(char_vec == std::vector<char>{ 'H', 'e', 'l', 'l', 'o', ',', 'w', 'o', 'r', 'l', 'd', '!' });
    }

    SECTION("Test join") {
        std::vector<std::string> v{ "Hello", "world", "!" };
        REQUIRE((v | join()) == "Helloworld!");
        REQUIRE((v | join(" ")) == "Hello world !");
        REQUIRE((v | join(", ")) == "Hello, world, !");

    }
}

TEST_CASE("Pipe tuple call tests", "tuple") {
    using namespace ohtoai::sugar::pipe;

    SECTION("Test tuple call") {
        auto add = [](int x, int y) { return x + y; };
        auto square = [](int x) { return x * x; };

        auto result = std::make_tuple(1, 2) | tuple_as_args(add) | square;
        REQUIRE(result == 9);
    }

    SECTION("Test tuple call with array") {
        std::array<int, 10> arr{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        auto sum = [](const auto&... args) { return (args + ...); };
        auto result = arr | tuple_as_args(sum);
        REQUIRE(result == 55);
    }

    SECTION("Test tuple call with multiple arguments") {
        auto add = [](int x, int y, int z) { return x + y + z; };
        auto square = [](int x) { return x * x; };

        auto result = std::make_tuple(1, 2, 3) | tuple_as_args(add) | square;
        REQUIRE(result == 36);
    }

    SECTION("Test tuple call with variadic arguments") {
        std::stringstream ss;
        auto print = [&ss](const auto&... args) {
            (ss << ... << args);
            };

        std::make_tuple("Hello, ", "world!") | tuple_as_args(print);
        REQUIRE(ss.str() == "Hello, world!");
    }
}

TEST_CASE("Pipe make_packet tests", "make_packet") {
    using ohtoai::sugar::pipe::make_packet;
    using ohtoai::sugar::pipe::tuple_as_args;

    SECTION("Test make_packet") {
        auto add_one = [](int x) { return x + 1; };
        auto square = [](int x) { return x * x; };

        auto packet = ohtoai::sugar::pipe::make_packet(add_one, square);
        REQUIRE((1 | packet) == 4);
        REQUIRE((2 | packet) == 9);
        REQUIRE((3 | packet) == 16);
    }

    SECTION("Test make_packet with multiple operations") {
        auto add = [](int x, int y) { return x + y; };
        auto square = [](int x) { return x * x; };

        auto packet = make_packet(tuple_as_args(add), square);
        REQUIRE((std::make_tuple(1, 2) | packet) == 9);
        REQUIRE((std::make_tuple(2, 3) | packet) == 25);
        REQUIRE((std::make_tuple(3, 4) | packet) == 49);
    }

    SECTION("Test make_packet with variadic arguments") {
        std::stringstream ss;
        auto print = [&ss](const auto&... args) {
            (ss << ... << args);
            };

        auto packet = make_packet(tuple_as_args(print));
        packet(std::make_tuple("Hello, ", "world!"));
        REQUIRE(ss.str() == "Hello, world!");
    }
}
