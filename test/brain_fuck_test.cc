#include <suger/brain_fuck.hh>
#include <catch2/catch.hpp>

TEST_CASE("BrainFuck") {
    using namespace ohtoai::sugar::funny::bf;
    constexpr auto code = ">+++++++++[<++++++++>-]<.>+++++++[<++++>-]<+.+++++++..+++.>>>++++++++[<++++>-]"
        "<.>>>++++++++++[<+++++++++>-]<---.<<<<.+++.------.--------.>>+.>++++++++++.";
    std::string str = brain_fuck<brain_fuck_output_size(code)>(code);
    SECTION("Test Hello World") {
        REQUIRE(str == "Hello World!\n");
    }
}