#define CATCH_CONFIG_MAIN
#include "Catch2/catch.hpp"
#include "../include/Game.h"
//#include "../include/index/PublicGood.h"


TEST_CASE("CreateGame", "[creategame]") 
{
	epic::longUInt quota = 12;
    epic::longUInt a = 9;
    epic::longUInt b = 5;
    epic::longUInt c = 3;
    std::vector<epic::longUInt> weights = {a, b, c, c};
	std::vector<std::vector<int>> precoalitions;



	auto g = new epic::Game(quota, weights, false, precoalitions);

    std::vector<epic::bigFloat> solution;//	= new auto;

	//epic::index::PublicGood* pgi = new epic::index::PublicGood(g, &solution);
}

TEST_CASE("Game X", "[index]")
{
    // Create Game here (acts like setup)
    
    SECTION("Index A") 
    {
        // Test Index A in here
    }
    SECTION("Index B") 
    {
        // Test Index B in here
    }
    SECTION("Index C") 
    {
        // Test Index C in here
    }
}


TEST_CASE("TestName", "[firsttag][secondtag]") 
{
	std::vector<int> vec = {1, 2, 3};
	REQUIRE(6 == 6);
	REQUIRE_FALSE(false);
}