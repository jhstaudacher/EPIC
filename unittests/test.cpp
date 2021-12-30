// DO not repeat this #define - you only need one main()
#define CATCH_CONFIG_MAIN
#include "Catch2/catch.hpp"

TEST_CASE("TestName", "[TestTag][Second Tag]"){
    std::vector<int> vec = {1, 2, 3};
    REQUIRE(6 == 6);
    REQUIRE_FALSE(false);
}