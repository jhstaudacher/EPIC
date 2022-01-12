#define CATCH_CONFIG_MAIN
#include "../include/FastUpperBoundApproximation.h"
#include "../include/Game.h"
#include "../include/ItfUpperBoundApproximation.h"
#include "../include/index/AbsoluteBanzhaf.h"
#include "../include/index/AbsolutePowerIndexG.h"
#include "../include/index/AbsolutePublicGood.h"
#include "../include/index/Banzhaf.h"
#include "../include/index/BanzhafBelow.h"
#include "../include/index/BanzhafOwen.h"
#include "../include/index/ColemanCollective.h"
#include "../include/index/ColemanInitiative.h"
#include "../include/index/ColemanPreventive.h"
#include "../include/index/DeeganPackel.h"
#include "../include/index/FelsenthalIndex.h"
#include "../include/index/HarsanyiNashIndex.h"
#include "../include/index/Johnston.h"
#include "../include/index/KoenigBraeuninger.h"
#include "../include/index/KoenigBraeuningerPH.h"
#include "../include/index/Nevison.h"
#include "../include/index/NevisonPH.h"
#include "../include/index/Owen.h"
#include "../include/index/PowerIndexF.h"
#include "../include/index/PowerIndexG.h"
#include "../include/index/PowerIndexGPH.h"
#include "../include/index/PublicGood.h"
#include "../include/index/PublicHelpTheta.h"
#include "../include/index/PublicHelpThetaPH.h"
#include "../include/index/PublicHelpXi.h"
#include "../include/index/Rae.h"
#include "../include/index/RawBanzhaf.h"
#include "../include/index/RawBanzhafBelow.h"
#include "../include/index/RawDeeganPackel.h"
#include "../include/index/RawFelsenthal.h"
#include "../include/index/RawJohnston.h"
#include "../include/index/RawPowerIndexF.h"
#include "../include/index/RawPowerIndexG.h"
#include "../include/index/RawPublicGood.h"
#include "../include/index/RawPublicHelpTheta.h"
#include "../include/index/RawPublicHelpThetaSD.h"
#include "../include/index/RawShapleyShubik.h"
#include "../include/index/RawShapleyShubikBelow.h"
#include "../include/index/ShapleyShubik.h"
#include "../include/index/ShapleyShubikBelow.h"
#include "../include/index/SingleValueW.h"
#include "../include/index/SingleValueWM.h"
#include "../include/index/SingleValueWS.h"
#include "Catch2/catch.hpp"
#include <gmpxx.h>

/**
 * Compares the actual result and the expected result.
 *
 * @param actual_result The actual result.
 * @param expected_result The expected result.
 * @param tolerance Defines the tolerance which is accepted (necessary due to epic::bigFloat).
*/
bool isResultIdentical(std::vector<epic::bigFloat> actual_result, std::vector<epic::bigFloat> expected_result, epic::bigFloat tolerance) {
	bool success = true;
	//gmp_printf("tol      %.50Ff\n", tolerance);
	for (long long unsigned int i = 0; i < expected_result.size(); i++) {
		if (abs(expected_result[i] - actual_result[i]) > tolerance) {
			std::cout << "False at following values: " << std::endl;
			gmp_printf("Expected value %.50Ff\n", expected_result[i]);
			gmp_printf("Actual value   %.50Ff\n", actual_result[i]);
			epic::bigFloat diff = expected_result[i] - actual_result[i];
			gmp_printf("Diff           %.50Ff\n", diff);
			success = false;
		}
	}

	return success;
}

TEST_CASE("Game: Quota: 11; Weights: 9, 5, 3, 3; Precoalitions = {{0}, {1}, {2, 3}};", "[index]") {
	epic::longUInt quota = 11;
	std::vector<epic::longUInt> weights = {9, 5, 3, 3};
	std::vector<std::vector<int>> precoalitions = {{0}, {1}, {2, 3}};
	const auto game = new epic::Game(quota, weights, false, precoalitions);
	const auto approx = new epic::FastUpperBoundApproximation(*game);
	epic::bigFloat tolerance = 0.000001;

	SECTION("AbsoluteBanzhaf") {
		auto index = epic::index::AbsoluteBanzhaf(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.75, 0.25, 0.25, 0.25};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("AbsolutePowerIndexG") {
		auto index = epic::index::AbsolutePowerIndexG(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.875, 0.625, 0.625, 0.625};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("AbsolutePublicGood") {
		auto index = epic::index::AbsolutePublicGood(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.75, 0.5, 0.5, 0.5};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("Banzhaf") {
		auto index = epic::index::Banzhaf(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.5, 0.166666666666666666666666666666667, 0.166666666666666666666666666666667, 0.166666666666666666666666666666667};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("BanzhafBelow") {
		auto index = epic::index::BanzhafBelow(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.5, 0.166666666666666666666666666666667, 0.166666666666666666666666666666667, 0.166666666666666666666666666666667};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("BanzhafOwen") {
		auto index = epic::index::BanzhafOwen(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.25, 0.25};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("ColemanCollective") {
		auto index = epic::index::ColemanCollective(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.5, 0.5};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("ColemanInitiative") {
		auto index = epic::index::ColemanInitiative(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.75, 0.25, 0.25, 0.25};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("ColemanPreventive") {
		auto index = epic::index::ColemanPreventive(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.75, 0.25, 0.25, 0.25};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("DeeganPackel") {
		auto index = epic::index::DeeganPackel(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.375, 0.208333, 0.208333, 0.208333};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("FelsenthalIndex") {
		auto index = epic::index::FelsenthalIndex(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.5, 0.166667, 0.166667, 0.166667};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("HarsanyiNashIndex") {
		auto index = epic::index::HarsanyiNashIndex(*game);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.25, 0.25, 0.25, 0.25};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("Johnston") {
		auto index = epic::index::Johnston(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.642857, 0.119048, 0.119048, 0.119048};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("KoenigBraeuninger") {
		auto index = epic::index::KoenigBraeuninger(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.875, 0.625, 0.625, 0.625};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("KoenigBraeuningerPH") {
		auto index = epic::index::KoenigBraeuningerPH(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.875, 0.625, 0.625, 0.625};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("Nevison") {
		auto index = epic::index::Nevison(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.875, 0.625, 0.625, 0.625};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("NevisonPH") {
		auto index = epic::index::NevisonPH(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.875, 0.625, 0.625, 0.625};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("Owen") {
		auto index = epic::index::Owen(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.333333, 0.166667, 0.166667};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("PowerIndexF") {
		auto index = epic::index::PowerIndexF(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.34375, 0.21875, 0.21875, 0.21875};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("PowerIndexG") {
		auto index = epic::index::PowerIndexG(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.318182, 0.227273, 0.227273, 0.227273};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("PowerIndexGPH") {
		auto index = epic::index::PowerIndexGPH(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.318182, 0.227273, 0.227273, 0.227273};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("PublicGood") {
		auto index = epic::index::PublicGood(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.222222, 0.222222, 0.222222};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("PublicHelpTheta") {
		auto index = epic::index::PublicHelpTheta(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.318182, 0.227273, 0.227273, 0.227273};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("PublicHelpThetaPH") {
		auto index = epic::index::PublicHelpThetaPH(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.318182, 0.227273, 0.227273, 0.227273};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("PublicHelpXi") {
		auto index = epic::index::PublicHelpXi(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.371622, 0.209459, 0.209459, 0.209459};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("Rae") {
		auto index = epic::index::Rae(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.875, 0.625, 0.625, 0.625};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("RawBanzhaf") {
		auto index = epic::index::RawBanzhaf(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {6, 2, 2, 2};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("RawBanzhafBelow") {
		auto index = epic::index::RawBanzhafBelow(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {6, 2, 2, 2};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("RawDeeganPackel") {
		auto index = epic::index::RawDeeganPackel(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {1.5, 0.833333, 0.833333, 0.833333};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("RawFelsenthal") {
		auto index = epic::index::RawFelsenthal(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {3, 1, 1, 1};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("RawJohnston") {
		auto index = epic::index::RawJohnston(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {4.5, 0.833333, 0.833333, 0.833333};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("RawPowerIndexF") {
		auto index = epic::index::RawPowerIndexF(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {2.75, 1.75, 1.75, 1.75};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("RawPowerIndexG") {
		auto index = epic::index::RawPowerIndexG(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {7, 5, 5, 5};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("RawPublicGood") {
		auto index = epic::index::RawPublicGood(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {3, 2, 2, 2};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("RawPublicHelpTheta") {
		auto index = epic::index::RawPublicHelpTheta(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {7, 5, 5, 5};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("RawPublicHelpThetaSD") {
		auto index = epic::index::RawPublicHelpThetaSD(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {7, 5, 5, 5};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("RawShapleyShubik") {
		auto index = epic::index::RawShapleyShubik(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {12, 4, 4, 4};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("RawShapleyShubikBelow") {
		auto index = epic::index::RawShapleyShubikBelow(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {12, 4, 4, 4};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("ShapleyShubik") {
		auto index = epic::index::ShapleyShubik(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.5, 0.166667, 0.166667, 0.166667};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("ShapleyShubikBelow") {
		auto index = epic::index::ShapleyShubikBelow(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {0.5, 0.166667, 0.166667, 0.166667};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("SingleValueW") {
		auto index = epic::index::SingleValueW(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {8};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("SingleValueWM") {
		auto index = epic::index::SingleValueWM(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {4};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
	SECTION("SingleValueWS") {
		auto index = epic::index::SingleValueWS(*game, approx);
		auto actual_result = index.calculate();
		std::vector<epic::bigFloat> expected_result = {6};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	}
}