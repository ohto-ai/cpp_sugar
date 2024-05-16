#include <sugar/factory.hh>
#include <catch2/catch.hpp>


// Define class
class Base {
public:
    virtual ~Base() = default;
    virtual std::string name() const = 0;
};

class Derived1 : public Base {
public:
    std::string name() const override {
        return "Derived1";
    }
};

class Derived2 : public Base {
public:
    std::string name() const override {
        return "Derived2";
    }
};

template <typename Type>
Type example{};


TEST_CASE("Test Factory method") {
    // Register
    ohtoai::sugar::ProductRegistrar<Base, Derived1> reg1("d1_id");
    ohtoai::sugar::ProductRegistrar<Base, Derived2> reg2("d2_id");

    // Create
    auto derived1 = ohtoai::sugar::ProductFactory<Base>::instance().product("d1_id");
    auto derived2 = ohtoai::sugar::ProductFactory<Base>::instance().product("d2_id");
    auto illegal = ohtoai::sugar::ProductFactory<Base>::instance().product("illegal_id");

    // Check
    REQUIRE(derived1->name() == example<Derived1>.name());
    REQUIRE(derived2->name() == example<Derived2>.name());
    REQUIRE(illegal == nullptr);
}