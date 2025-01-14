#include <sugar/brain_fuck.hh>
#include <catch2/catch.hpp>

TEST_CASE("BrainFuck") {
    using namespace ai::sugar::funny::bf;
    constexpr auto code = ">+++++++++[<++++++++>-]<.>+++++++[<++++>-]<+.+++++++..+++.>>>++++++++[<++++>-]"
        "<.>>>++++++++++[<+++++++++>-]<---.<<<<.+++.------.--------.>>+.>++++++++++.";
    std::string bf(brain_fuck<brain_fuck_output_size(code)>(code));
    SECTION("Test Hello World") {
        REQUIRE(bf == "Hello World!\n");
    }
}
