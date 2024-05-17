#include <sugar/brain_fuck.hh>
#include <catch2/catch.hpp>

TEST_CASE("BrainFuck") {
    using namespace ohtoai::sugar::funny::bf;
    constexpr auto code = ">+++++++++[<++++++++>-]<.>+++++++[<++++>-]<+.+++++++..+++.>>>++++++++[<++++>-]"
        "<.>>>++++++++++[<+++++++++>-]<---.<<<<.+++.------.--------.>>+.>++++++++++.";
    const char* bf = brain_fuck<brain_fuck_output_size(code)>(code);
    SECTION("Test Hello World") {
        REQUIRE(std::string(bf) == "Hello World!\n");
    }
}
