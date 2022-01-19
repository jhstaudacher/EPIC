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
#include "../include/lint/GlobalCalculator.h"
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

TEST_CASE("TestCase #01: Game: Quota: 11; Weights: 9, 5, 3, 3; Precoalitions = {{0}, {1}, {2, 3}};", "[index]") {
	epic::longUInt quota = 11;
	std::vector<epic::longUInt> weights = {9, 5, 3, 3};
	std::vector<std::vector<int>> precoalitions = {{0}, {1}, {2, 3}};

	const auto game = new epic::Game(quota, weights, false);
	const auto precoalition_game = new epic::PrecoalitionGame(quota, weights, false, precoalitions);
	const auto approx = new epic::FastUpperBoundApproximation(*game);
	epic::bigFloat tolerance = 0.00001;

	SECTION("AbsoluteBanzhaf") {
		const auto index = new epic::index::AbsoluteBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.25, 0.25, 0.25};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePowerIndexG") {
		const auto index = new epic::index::AbsolutePowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.625, 0.625, 0.625};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePublicGood") {
		const auto index = new epic::index::AbsolutePublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.5, 0.5, 0.5};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Banzhaf") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.166666666666666666666666666666667, 0.166666666666666666666666666666667, 0.166666666666666666666666666666667};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafBelow") {
		const auto index = new epic::index::BanzhafBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.166666666666666666666666666666667, 0.166666666666666666666666666666667, 0.166666666666666666666666666666667};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafOwen") {
		const auto index = new epic::index::BanzhafOwen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.25, 0.25};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanCollective") {
		const auto index = new epic::index::ColemanCollective();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.5, 0.5};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanInitiative") {
		const auto index = new epic::index::ColemanInitiative();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.25, 0.25, 0.25};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanPreventive") {
		const auto index = new epic::index::ColemanPreventive();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.25, 0.25, 0.25};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("DeeganPackel") {
		const auto index = new epic::index::DeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.375, 0.208333, 0.208333, 0.208333};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("FelsenthalIndex") {
		const auto index = new epic::index::FelsenthalIndex(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.166667, 0.166667, 0.166667};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("HarsanyiNashIndex") {
		const auto index = new epic::index::HarsanyiNashIndex();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.25, 0.25, 0.25, 0.25};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Johnston") {
		const auto index = new epic::index::Johnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.642857, 0.119048, 0.119048, 0.119048};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuninger") {
		const auto index = new epic::index::KoenigBraeuninger(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.625, 0.625, 0.625};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuningerPH") {
		const auto index = new epic::index::KoenigBraeuningerPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.625, 0.625, 0.625};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Nevison") {
		const auto index = new epic::index::Nevison(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.625, 0.625, 0.625};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("NevisonPH") {
		const auto index = new epic::index::NevisonPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.625, 0.625, 0.625};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Owen") {
		const auto index = new epic::index::Owen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.333333, 0.166667, 0.166667};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexF") {
		const auto index = new epic::index::PowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.34375, 0.21875, 0.21875, 0.21875};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexG") {
		const auto index = new epic::index::PowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.318182, 0.227273, 0.227273, 0.227273};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexGPH") {
		const auto index = new epic::index::PowerIndexGPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.318182, 0.227273, 0.227273, 0.227273};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicGood") {
		const auto index = new epic::index::PublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.222222, 0.222222, 0.222222};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpTheta") {
		const auto index = new epic::index::PublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.318182, 0.227273, 0.227273, 0.227273};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpThetaPH") {
		const auto index = new epic::index::PublicHelpThetaPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.318182, 0.227273, 0.227273, 0.227273};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpXi") {
		const auto index = new epic::index::PublicHelpXi(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.371622, 0.209459, 0.209459, 0.209459};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Rae") {
		const auto index = new epic::index::Rae(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.625, 0.625, 0.625};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhaf") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {6, 2, 2, 2};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhafBelow") {
		const auto index = new epic::index::RawBanzhafBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {6, 2, 2, 2};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawDeeganPackel") {
		const auto index = new epic::index::RawDeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {1.5, 0.833333, 0.833333, 0.833333};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawFelsenthal") {
		const auto index = new epic::index::RawFelsenthal(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {3, 1, 1, 1};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawJohnston") {
		const auto index = new epic::index::RawJohnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4.5, 0.833333, 0.833333, 0.833333};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexF") {
		const auto index = new epic::index::RawPowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2.75, 1.75, 1.75, 1.75};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexG") {
		const auto index = new epic::index::RawPowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7, 5, 5, 5};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicGood") {
		const auto index = new epic::index::RawPublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {3, 2, 2, 2};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpTheta") {
		const auto index = new epic::index::RawPublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7, 5, 5, 5};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpThetaSD") {
		const auto index = new epic::index::RawPublicHelpThetaSD(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7, 5, 5, 5};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubik") {
		const auto index = new epic::index::RawShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {12, 4, 4, 4};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubikBelow") {
		const auto index = new epic::index::RawShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {12, 4, 4, 4};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubik") {
		const auto index = new epic::index::ShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.166667, 0.166667, 0.166667};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubikBelow") {
		const auto index = new epic::index::ShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.166667, 0.166667, 0.166667};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueW") {
		const auto index = new epic::index::SingleValueW();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {8};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWM") {
		const auto index = new epic::index::SingleValueWM();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWS") {
		const auto index = new epic::index::SingleValueWS(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {6};
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
}

TEST_CASE("TestCase #02: Game: Quota: 68; Weights: 60, 34, 13, 11, 17; Precoalitions = {{0}, {1, 2, 3}, {4}};", "[index]") {
	epic::longUInt quota = 68;
	std::vector<epic::longUInt> weights = {60, 34, 13, 11, 17};
	std::vector<std::vector<int>> precoalitions = {{0}, {1, 2, 3}, {4}};

	const auto game = new epic::Game(quota, weights, false);
	const auto precoalition_game = new epic::PrecoalitionGame(quota, weights, false, precoalitions);
	const auto approx = new epic::FastUpperBoundApproximation(*game);
	epic::bigFloat tolerance = 0.00001;

	SECTION("AbsoluteBanzhaf") {
		const auto index = new epic::index::AbsoluteBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.125, 0.125, 0.125, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePowerIndexG") {
		const auto index = new epic::index::AbsolutePowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePublicGood") {
		const auto index = new epic::index::AbsolutePublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.8, 0.4, 0.4, 0.4, 0.4};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Banzhaf") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.636364, 0.0909091, 0.0909091, 0.0909091, 0.0909091};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafBelow") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.636364, 0.0909091, 0.0909091, 0.0909091, 0.0909091};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafOwen") {
		const auto index = new epic::index::BanzhafOwen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.125, 0.5, 0.125, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanCollective") {
		const auto index = new epic::index::ColemanCollective();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.5, 0.5, 0.5};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanInitiative") {
		const auto index = new epic::index::ColemanInitiative();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.125, 0.125, 0.125, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanPreventive") {
		const auto index = new epic::index::ColemanPreventive();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.125, 0.125, 0.125, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("DeeganPackel") {
		const auto index = new epic::index::DeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.4, 0.15, 0.15, 0.15, 0.15};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("FelsenthalIndex") {
		const auto index = new epic::index::FelsenthalIndex(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.125, 0.125, 0.125, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("HarsanyiNashIndex") {
		const auto index = new epic::index::HarsanyiNashIndex();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.2, 0.2, 0.2, 0.2, 0.2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Johnston") {
		const auto index = new epic::index::Johnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.8, 0.05, 0.05, 0.05, 0.05};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuninger") {
		const auto index = new epic::index::KoenigBraeuninger(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuningerPH") {
		const auto index = new epic::index::KoenigBraeuningerPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Nevison") {
		const auto index = new epic::index::Nevison(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("NevisonPH") {
		const auto index = new epic::index::NevisonPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Owen") {
		const auto index = new epic::index::Owen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.111111, 0.333333, 0.111111, 0.111111};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexF") {
		const auto index = new epic::index::PowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.325, 0.16875, 0.16875, 0.16875, 0.16875};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexG") {
		const auto index = new epic::index::PowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.294118, 0.176471, 0.176471, 0.176471, 0.176471};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexGPH") {
		const auto index = new epic::index::PowerIndexGPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.294118, 0.176471, 0.176471, 0.176471, 0.176471};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicGood") {
		const auto index = new epic::index::PublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.166667, 0.166667, 0.166667, 0.166667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpTheta") {
		const auto index = new epic::index::PublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.294118, 0.176471, 0.176471, 0.176471, 0.176471};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpThetaPH") {
		const auto index = new epic::index::PublicHelpThetaPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.294118, 0.176471, 0.176471, 0.176471, 0.176471};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpXi") {
		const auto index = new epic::index::PublicHelpXi(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.359021, 0.160245, 0.160245, 0.160245, 0.160245};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Rae") {
		const auto index = new epic::index::Rae(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhaf") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {14, 2, 2, 2, 2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhafBelow") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {14, 2, 2, 2, 2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawDeeganPackel") {
		const auto index = new epic::index::RawDeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2, 0.75, 0.75, 0.75, 0.75};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawFelsenthal") {
		const auto index = new epic::index::RawFelsenthal(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4, 1, 1, 1, 1};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawJohnston") {
		const auto index = new epic::index::RawJohnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {12, 0.75, 0.75, 0.75, 0.75};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexF") {
		const auto index = new epic::index::RawPowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {5.2, 2.7, 2.7, 2.7, 2.7};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexG") {
		const auto index = new epic::index::RawPowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15, 9, 9, 9, 9};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicGood") {
		const auto index = new epic::index::RawPublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4, 2, 2, 2, 2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpTheta") {
		const auto index = new epic::index::RawPublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15, 9, 9, 9, 9};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpThetaSD") {
		const auto index = new epic::index::RawPublicHelpThetaSD(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15, 9, 9, 9, 9};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubik") {
		const auto index = new epic::index::RawShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {72, 12, 12, 12, 12};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubikBelow") {
		const auto index = new epic::index::RawShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {72, 12, 12, 12, 12};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubik") {
		const auto index = new epic::index::ShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.6, 0.1, 0.1, 0.1, 0.1};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubikBelow") {
		const auto index = new epic::index::ShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.6, 0.1, 0.1, 0.1, 0.1};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueW") {
		const auto index = new epic::index::SingleValueW();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {16};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWM") {
		const auto index = new epic::index::SingleValueWM();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {5};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWS") {
		const auto index = new epic::index::SingleValueWS(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {8};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
}

TEST_CASE("TestCase #03: Game: Quota: 5; Weights: 3, 1, 1, 1, 3; Precoalitions = {{0}, {1, 2, 3}, {4}};", "[index]") {
	epic::longUInt quota = 5;
	std::vector<epic::longUInt> weights = {3, 1, 1, 1, 3};
	std::vector<std::vector<int>> precoalitions = {{0}, {1, 2, 3}, {4}};

	const auto game = new epic::Game(quota, weights, false);
	const auto precoalition_game = new epic::PrecoalitionGame(quota, weights, false, precoalitions);
	const auto approx = new epic::FastUpperBoundApproximation(*game);
	epic::bigFloat tolerance = 0.00001;

	SECTION("AbsoluteBanzhaf") {
		const auto index = new epic::index::AbsoluteBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.25, 0.25, 0.25};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePowerIndexG") {
		const auto index = new epic::index::AbsolutePowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.625, 0.625, 0.625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePublicGood") {
		const auto index = new epic::index::AbsolutePublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.571429, 0.571429, 0.571429, 0.571429, 0.571429};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Banzhaf") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.285714, 0.285714, 0.142857, 0.142857, 0.142857};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafBelow") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.285714, 0.285714, 0.142857, 0.142857, 0.142857};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafOwen") {
		const auto index = new epic::index::BanzhafOwen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.25, 0.25, 0.25};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanCollective") {
		const auto index = new epic::index::ColemanCollective();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.5, 0.5, 0.5};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanInitiative") {
		const auto index = new epic::index::ColemanInitiative();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.25, 0.25, 0.25};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanPreventive") {
		const auto index = new epic::index::ColemanPreventive();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.25, 0.25, 0.25};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("DeeganPackel") {
		const auto index = new epic::index::DeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.214286, 0.214286, 0.190476, 0.190476, 0.190476};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("FelsenthalIndex") {
		const auto index = new epic::index::FelsenthalIndex(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0, 0, 0};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("HarsanyiNashIndex") {
		const auto index = new epic::index::HarsanyiNashIndex();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.2, 0.2, 0.2, 0.2, 0.2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Johnston") {
		const auto index = new epic::index::Johnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.333333, 0.111111, 0.111111, 0.111111};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuninger") {
		const auto index = new epic::index::KoenigBraeuninger(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.625, 0.625, 0.625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuningerPH") {
		const auto index = new epic::index::KoenigBraeuningerPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.625, 0.625, 0.625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Nevison") {
		const auto index = new epic::index::Nevison(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.625, 0.625, 0.625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("NevisonPH") {
		const auto index = new epic::index::NevisonPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.625, 0.625, 0.625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Owen") {
		const auto index = new epic::index::Owen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.333333, 0.111111, 0.111111, 0.111111};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexF") {
		const auto index = new epic::index::PowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.23125, 0.23125, 0.179167, 0.179167, 0.179167};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexG") {
		const auto index = new epic::index::PowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.222222, 0.222222, 0.185185, 0.185185, 0.185185};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexGPH") {
		const auto index = new epic::index::PowerIndexGPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.222222, 0.222222, 0.185185, 0.185185, 0.185185};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicGood") {
		const auto index = new epic::index::PublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.2, 0.2, 0.2, 0.2, 0.2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpTheta") {
		const auto index = new epic::index::PublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.222222, 0.222222, 0.185185, 0.185185, 0.185185};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpThetaPH") {
		const auto index = new epic::index::PublicHelpThetaPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.222222, 0.222222, 0.185185, 0.185185, 0.185185};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpXi") {
		const auto index = new epic::index::PublicHelpXi(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.243771, 0.243771, 0.170819, 0.170819, 0.170819};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Rae") {
		const auto index = new epic::index::Rae(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.625, 0.625, 0.625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhaf") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {8, 8, 4, 4, 4};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhafBelow") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {8, 8, 4, 4, 4};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawDeeganPackel") {
		const auto index = new epic::index::RawDeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {1.5, 1.5, 1.33333, 1.33333, 1.33333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawFelsenthal") {
		const auto index = new epic::index::RawFelsenthal(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {1, 1, 0, 0, 0};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawJohnston") {
		const auto index = new epic::index::RawJohnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4, 4, 1.33333, 1.33333, 1.33333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexF") {
		const auto index = new epic::index::RawPowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {3.7, 3.7, 2.86667, 2.86667, 2.86667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexG") {
		const auto index = new epic::index::RawPowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {12, 12, 10, 10, 10};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicGood") {
		const auto index = new epic::index::RawPublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4, 4, 4, 4, 4};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpTheta") {
		const auto index = new epic::index::RawPublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {12, 12, 10, 10, 10};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpThetaSD") {
		const auto index = new epic::index::RawPublicHelpThetaSD(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {12, 12, 10, 10, 10};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubik") {
		const auto index = new epic::index::RawShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {36, 36, 16, 16, 16};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubikBelow") {
		const auto index = new epic::index::RawShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {36, 36, 16, 16, 16};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubik") {
		const auto index = new epic::index::ShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.3, 0.3, 0.133333, 0.133333, 0.133333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubikBelow") {
		const auto index = new epic::index::ShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.3, 0.3, 0.133333, 0.133333, 0.133333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueW") {
		const auto index = new epic::index::SingleValueW();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {16};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWM") {
		const auto index = new epic::index::SingleValueWM();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWS") {
		const auto index = new epic::index::SingleValueWS(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
}

TEST_CASE("TestCase #04: Game: Quota: 7; Weights: 3, 3, 2, 2, 2; Precoalitions = {{0}, {1}, {2}, {3, 4}};", "[index]") {
	epic::longUInt quota = 7;
	std::vector<epic::longUInt> weights = {3, 3, 2, 2, 2};
	std::vector<std::vector<int>> precoalitions = {{0}, {1}, {2}, {3, 4}};

	const auto game = new epic::Game(quota, weights, false);
	const auto precoalition_game = new epic::PrecoalitionGame(quota, weights, false, precoalitions);
	const auto approx = new epic::FastUpperBoundApproximation(*game);
	epic::bigFloat tolerance = 0.00001;

	SECTION("AbsoluteBanzhaf") {
		const auto index = new epic::index::AbsoluteBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.4375, 0.4375, 0.3125, 0.3125, 0.3125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePowerIndexG") {
		const auto index = new epic::index::AbsolutePowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.733333, 0.733333, 0.666667, 0.666667, 0.666667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePublicGood") {
		const auto index = new epic::index::AbsolutePublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.666667, 0.666667, 0.555556, 0.555556, 0.555556};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Banzhaf") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.241379, 0.241379, 0.172414, 0.172414, 0.172414};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafBelow") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.241379, 0.241379, 0.172414, 0.172414, 0.172414};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafOwen") {
		const auto index = new epic::index::BanzhafOwen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.375, 0.375, 0.3125, 0.3125, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanCollective") {
		const auto index = new epic::index::ColemanCollective();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.46875, 0.46875, 0.46875, 0.46875, 0.46875};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanInitiative") {
		const auto index = new epic::index::ColemanInitiative();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.411765, 0.411765, 0.294118, 0.294118, 0.294118};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanPreventive") {
		const auto index = new epic::index::ColemanPreventive();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.466667, 0.466667, 0.333333, 0.333333, 0.333333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("DeeganPackel") {
		const auto index = new epic::index::DeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.222222, 0.222222, 0.185185, 0.185185, 0.185185};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("FelsenthalIndex") {
		const auto index = new epic::index::FelsenthalIndex(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.222222, 0.222222, 0.185185, 0.185185, 0.185185};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("HarsanyiNashIndex") {
		const auto index = new epic::index::HarsanyiNashIndex();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.2, 0.2, 0.2, 0.2, 0.2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Johnston") {
		const auto index = new epic::index::Johnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.272727, 0.272727, 0.151515, 0.151515, 0.151515};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuninger") {
		const auto index = new epic::index::KoenigBraeuninger(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.733333, 0.733333, 0.666667, 0.666667, 0.666667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuningerPH") {
		const auto index = new epic::index::KoenigBraeuningerPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.733333, 0.733333, 0.666667, 0.666667, 0.666667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Nevison") {
		const auto index = new epic::index::Nevison(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.6875, 0.6875, 0.625, 0.625, 0.625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("NevisonPH") {
		const auto index = new epic::index::NevisonPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.6875, 0.6875, 0.625, 0.625, 0.625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Owen") {
		const auto index = new epic::index::Owen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.25, 0.25, 0.208333, 0.208333, 0.0833333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexF") {
		const auto index = new epic::index::PowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.213333, 0.213333, 0.191111, 0.191111, 0.191111};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexG") {
		const auto index = new epic::index::PowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.211538, 0.211538, 0.192308, 0.192308, 0.192308};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexGPH") {
		const auto index = new epic::index::PowerIndexGPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.211538, 0.211538, 0.192308, 0.192308, 0.192308};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicGood") {
		const auto index = new epic::index::PublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.222222, 0.222222, 0.185185, 0.185185, 0.185185};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpTheta") {
		const auto index = new epic::index::PublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.211538, 0.211538, 0.192308, 0.192308, 0.192308};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpThetaPH") {
		const auto index = new epic::index::PublicHelpThetaPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.211538, 0.211538, 0.192308, 0.192308, 0.192308};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpXi") {
		const auto index = new epic::index::PublicHelpXi(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.214981, 0.214981, 0.190012, 0.190012, 0.190012};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Rae") {
		const auto index = new epic::index::Rae(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.71875, 0.71875, 0.65625, 0.65625, 0.65625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhaf") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7, 7, 5, 5, 5};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhafBelow") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7, 7, 5, 5, 5};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawDeeganPackel") {
		const auto index = new epic::index::RawDeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2, 2, 1.66667, 1.66667, 1.66667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawFelsenthal") {
		const auto index = new epic::index::RawFelsenthal(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {6, 6, 5, 5, 5};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawJohnston") {
		const auto index = new epic::index::RawJohnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {3, 3, 1.66667, 1.66667, 1.66667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexF") {
		const auto index = new epic::index::RawPowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {3.2, 3.2, 2.86667, 2.86667, 2.86667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexG") {
		const auto index = new epic::index::RawPowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {11, 11, 10, 10, 10};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicGood") {
		const auto index = new epic::index::RawPublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {6, 6, 5, 5, 5};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpTheta") {
		const auto index = new epic::index::RawPublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {11, 11, 10, 10, 10};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpThetaSD") {
		const auto index = new epic::index::RawPublicHelpThetaSD(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {11, 11, 10, 10, 10};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubik") {
		const auto index = new epic::index::RawShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {30, 30, 20, 20, 20};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubikBelow") {
		const auto index = new epic::index::RawShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {30, 30, 20, 20, 20};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubik") {
		const auto index = new epic::index::ShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.25, 0.25, 0.166667, 0.166667, 0.166667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubikBelow") {
		const auto index = new epic::index::ShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.25, 0.25, 0.166667, 0.166667, 0.166667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueW") {
		const auto index = new epic::index::SingleValueW();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWM") {
		const auto index = new epic::index::SingleValueWM();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {9};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWS") {
		const auto index = new epic::index::SingleValueWS(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {27};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
}

TEST_CASE("TestCase #05: Game: Quota: 5; Weights: 4, 3, 1, 1, 1; Precoalitions = {{0}, {1}, {2}, {3, 4}};", "[index]") {
	epic::longUInt quota = 5;
	std::vector<epic::longUInt> weights = {4, 3, 1, 1, 1};
	std::vector<std::vector<int>> precoalitions = {{0}, {1}, {2}, {3, 4}};

	const auto game = new epic::Game(quota, weights, false);
	const auto precoalition_game = new epic::PrecoalitionGame(quota, weights, false, precoalitions);
	const auto approx = new epic::FastUpperBoundApproximation(*game);
	epic::bigFloat tolerance = 0.00001;

	SECTION("AbsoluteBanzhaf") {
		const auto index = new epic::index::AbsoluteBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.6875, 0.3125, 0.1875, 0.1875, 0.1875};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePowerIndexG") {
		const auto index = new epic::index::AbsolutePowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.789474, 0.631579, 0.578947, 0.578947, 0.578947};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePublicGood") {
		const auto index = new epic::index::AbsolutePublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.571429, 0.571429, 0.428571, 0.428571, 0.428571};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Banzhaf") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.44, 0.2, 0.12, 0.12, 0.12};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafBelow") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.44, 0.2, 0.12, 0.12, 0.12};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafOwen") {
		const auto index = new epic::index::BanzhafOwen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.625, 0.375, 0.1875, 0.1875, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanCollective") {
		const auto index = new epic::index::ColemanCollective();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.59375, 0.59375, 0.59375, 0.59375, 0.59375};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanInitiative") {
		const auto index = new epic::index::ColemanInitiative();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.846154, 0.384615, 0.230769, 0.230769, 0.230769};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanPreventive") {
		const auto index = new epic::index::ColemanPreventive();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.578947, 0.263158, 0.157895, 0.157895, 0.157895};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("DeeganPackel") {
		const auto index = new epic::index::DeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.285714, 0.214286, 0.166667, 0.166667, 0.166667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("FelsenthalIndex") {
		const auto index = new epic::index::FelsenthalIndex(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.125, 0.125, 0.125, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("HarsanyiNashIndex") {
		const auto index = new epic::index::HarsanyiNashIndex();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.2, 0.2, 0.2, 0.2, 0.2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Johnston") {
		const auto index = new epic::index::Johnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.6, 0.166667, 0.0777778, 0.0777778, 0.0777778};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuninger") {
		const auto index = new epic::index::KoenigBraeuninger(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.789474, 0.631579, 0.578947, 0.578947, 0.578947};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuningerPH") {
		const auto index = new epic::index::KoenigBraeuningerPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.789474, 0.631579, 0.578947, 0.578947, 0.578947};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Nevison") {
		const auto index = new epic::index::Nevison(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.75, 0.6875, 0.6875, 0.6875};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("NevisonPH") {
		const auto index = new epic::index::NevisonPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.75, 0.6875, 0.6875, 0.6875};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Owen") {
		const auto index = new epic::index::Owen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.416667, 0.25, 0.125, 0.125, 0.0833333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexF") {
		const auto index = new epic::index::PowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.273684, 0.194737, 0.177193, 0.177193, 0.177193};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexG") {
		const auto index = new epic::index::PowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.25, 0.2, 0.183333, 0.183333, 0.183333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexGPH") {
		const auto index = new epic::index::PowerIndexGPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.25, 0.2, 0.183333, 0.183333, 0.183333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicGood") {
		const auto index = new epic::index::PublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.235294, 0.235294, 0.176471, 0.176471, 0.176471};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpTheta") {
		const auto index = new epic::index::PublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.25, 0.2, 0.183333, 0.183333, 0.183333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpThetaPH") {
		const auto index = new epic::index::PublicHelpThetaPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.25, 0.2, 0.183333, 0.183333, 0.183333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpXi") {
		const auto index = new epic::index::PublicHelpXi(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.303359, 0.18708, 0.169854, 0.169854, 0.169854};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Rae") {
		const auto index = new epic::index::Rae(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.84375, 0.65625, 0.59375, 0.59375, 0.59375};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhaf") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {11, 5, 3, 3, 3};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhafBelow") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {11, 5, 3, 3, 3};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawDeeganPackel") {
		const auto index = new epic::index::RawDeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2, 1.5, 1.16667, 1.16667, 1.16667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawFelsenthal") {
		const auto index = new epic::index::RawFelsenthal(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4, 1, 1, 1, 1};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawJohnston") {
		const auto index = new epic::index::RawJohnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {9, 2.5, 1.16667, 1.16667, 1.16667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexF") {
		const auto index = new epic::index::RawPowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {5.2, 3.7, 3.36667, 3.36667, 3.36667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexG") {
		const auto index = new epic::index::RawPowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15, 12, 11, 11, 11};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicGood") {
		const auto index = new epic::index::RawPublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4, 4, 3, 3, 3};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpTheta") {
		const auto index = new epic::index::RawPublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15, 12, 11, 11, 11};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpThetaSD") {
		const auto index = new epic::index::RawPublicHelpThetaSD(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15, 12, 11, 11, 11};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubik") {
		const auto index = new epic::index::RawShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {54, 24, 14, 14, 14};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubikBelow") {
		const auto index = new epic::index::RawShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {54, 24, 14, 14, 14};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubik") {
		const auto index = new epic::index::ShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.45, 0.2, 0.116667, 0.116667, 0.116667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubikBelow") {
		const auto index = new epic::index::ShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.45, 0.2, 0.116667, 0.116667, 0.116667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueW") {
		const auto index = new epic::index::SingleValueW();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {19};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWM") {
		const auto index = new epic::index::SingleValueWM();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWS") {
		const auto index = new epic::index::SingleValueWS(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {8};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
}

TEST_CASE("TestCase #06: Game: Quota: 11; Weights: 6, 5, 4, 4, 2; Precoalitions = {{0}, {1}, {2}, {3, 4}};", "[index]") {
	epic::longUInt quota = 11;
	std::vector<epic::longUInt> weights = {6, 5, 4, 4, 2};
	std::vector<std::vector<int>> precoalitions = {{0}, {1}, {2}, {3, 4}};

	const auto game = new epic::Game(quota, weights, false);
	const auto precoalition_game = new epic::PrecoalitionGame(quota, weights, false, precoalitions);
	const auto approx = new epic::FastUpperBoundApproximation(*game);
	epic::bigFloat tolerance = 0.00001;

	SECTION("AbsoluteBanzhaf") {
		const auto index = new epic::index::AbsoluteBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.25, 0.25, 0.25};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePowerIndexG") {
		const auto index = new epic::index::AbsolutePowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.625, 0.625, 0.625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePublicGood") {
		const auto index = new epic::index::AbsolutePublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.571429, 0.571429, 0.571429, 0.571429, 0.571429};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Banzhaf") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.285714, 0.285714, 0.142857, 0.142857, 0.142857};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafBelow") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.285714, 0.285714, 0.142857, 0.142857, 0.142857};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafOwen") {
		const auto index = new epic::index::BanzhafOwen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.25, 0, 0.25};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanCollective") {
		const auto index = new epic::index::ColemanCollective();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.5, 0.5, 0.5};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanInitiative") {
		const auto index = new epic::index::ColemanInitiative();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.25, 0.25, 0.25};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanPreventive") {
		const auto index = new epic::index::ColemanPreventive();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.25, 0.25, 0.25};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("DeeganPackel") {
		const auto index = new epic::index::DeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.214286, 0.214286, 0.190476, 0.190476, 0.190476};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("FelsenthalIndex") {
		const auto index = new epic::index::FelsenthalIndex(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0, 0, 0};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("HarsanyiNashIndex") {
		const auto index = new epic::index::HarsanyiNashIndex();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.2, 0.2, 0.2, 0.2, 0.2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Johnston") {
		const auto index = new epic::index::Johnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.333333, 0.111111, 0.111111, 0.111111};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuninger") {
		const auto index = new epic::index::KoenigBraeuninger(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.625, 0.625, 0.625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuningerPH") {
		const auto index = new epic::index::KoenigBraeuningerPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.625, 0.625, 0.625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Nevison") {
		const auto index = new epic::index::Nevison(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.625, 0.625, 0.625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("NevisonPH") {
		const auto index = new epic::index::NevisonPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.625, 0.625, 0.625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Owen") {
		const auto index = new epic::index::Owen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.333333, 0.166667, 0, 0.166667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexF") {
		const auto index = new epic::index::PowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.23125, 0.23125, 0.179167, 0.179167, 0.179167};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexG") {
		const auto index = new epic::index::PowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.222222, 0.222222, 0.185185, 0.185185, 0.185185};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexGPH") {
		const auto index = new epic::index::PowerIndexGPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.222222, 0.222222, 0.185185, 0.185185, 0.185185};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicGood") {
		const auto index = new epic::index::PublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.2, 0.2, 0.2, 0.2, 0.2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpTheta") {
		const auto index = new epic::index::PublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.222222, 0.222222, 0.185185, 0.185185, 0.185185};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpThetaPH") {
		const auto index = new epic::index::PublicHelpThetaPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.222222, 0.222222, 0.185185, 0.185185, 0.185185};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpXi") {
		const auto index = new epic::index::PublicHelpXi(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.243771, 0.243771, 0.170819, 0.170819, 0.170819};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Rae") {
		const auto index = new epic::index::Rae(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.625, 0.625, 0.625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhaf") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {8, 8, 4, 4, 4};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhafBelow") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {8, 8, 4, 4, 4};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawDeeganPackel") {
		const auto index = new epic::index::RawDeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {1.5, 1.5, 1.33333, 1.33333, 1.33333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawFelsenthal") {
		const auto index = new epic::index::RawFelsenthal(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {1, 1, 0, 0, 0};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawJohnston") {
		const auto index = new epic::index::RawJohnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4, 4, 1.33333, 1.33333, 1.33333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexF") {
		const auto index = new epic::index::RawPowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {3.7, 3.7, 2.86667, 2.86667, 2.86667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexG") {
		const auto index = new epic::index::RawPowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {12, 12, 10, 10, 10};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicGood") {
		const auto index = new epic::index::RawPublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4, 4, 4, 4, 4};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpTheta") {
		const auto index = new epic::index::RawPublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {12, 12, 10, 10, 10};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpThetaSD") {
		const auto index = new epic::index::RawPublicHelpThetaSD(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {12, 12, 10, 10, 10};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubik") {
		const auto index = new epic::index::RawShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {36, 36, 16, 16, 16};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubikBelow") {
		const auto index = new epic::index::RawShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {36, 36, 16, 16, 16};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubik") {
		const auto index = new epic::index::ShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.3, 0.3, 0.133333, 0.133333, 0.133333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubikBelow") {
		const auto index = new epic::index::ShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.3, 0.3, 0.133333, 0.133333, 0.133333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueW") {
		const auto index = new epic::index::SingleValueW();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {16};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWM") {
		const auto index = new epic::index::SingleValueWM();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWS") {
		const auto index = new epic::index::SingleValueWS(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
}

TEST_CASE("TestCase #07: Game: Quota: 6; Weights: 5, 2, 2, 1, 1; Precoalitions = {{0}, {1}, {2}, {3, 4}};", "[index]") {
	epic::longUInt quota = 6;
	std::vector<epic::longUInt> weights = {5, 2, 2, 1, 1};
	std::vector<std::vector<int>> precoalitions = {{0}, {1}, {2}, {3, 4}};

	const auto game = new epic::Game(quota, weights, false);
	const auto precoalition_game = new epic::PrecoalitionGame(quota, weights, false, precoalitions);
	const auto approx = new epic::FastUpperBoundApproximation(*game);
	epic::bigFloat tolerance = 0.00001;

	SECTION("AbsoluteBanzhaf") {
		const auto index = new epic::index::AbsoluteBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.125, 0.125, 0.125, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePowerIndexG") {
		const auto index = new epic::index::AbsolutePowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePublicGood") {
		const auto index = new epic::index::AbsolutePublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.8, 0.4, 0.4, 0.4, 0.4};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Banzhaf") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.636364, 0.0909091, 0.0909091, 0.0909091, 0.0909091};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafBelow") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.636364, 0.0909091, 0.0909091, 0.0909091, 0.0909091};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafOwen") {
		const auto index = new epic::index::BanzhafOwen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.25, 0.25, 0.125, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanCollective") {
		const auto index = new epic::index::ColemanCollective();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.5, 0.5, 0.5};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanInitiative") {
		const auto index = new epic::index::ColemanInitiative();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.125, 0.125, 0.125, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanPreventive") {
		const auto index = new epic::index::ColemanPreventive();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.125, 0.125, 0.125, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("DeeganPackel") {
		const auto index = new epic::index::DeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.4, 0.15, 0.15, 0.15, 0.15};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("FelsenthalIndex") {
		const auto index = new epic::index::FelsenthalIndex(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.125, 0.125, 0.125, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("HarsanyiNashIndex") {
		const auto index = new epic::index::HarsanyiNashIndex();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.2, 0.2, 0.2, 0.2, 0.2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Johnston") {
		const auto index = new epic::index::Johnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.8, 0.05, 0.05, 0.05, 0.05};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuninger") {
		const auto index = new epic::index::KoenigBraeuninger(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuningerPH") {
		const auto index = new epic::index::KoenigBraeuningerPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Nevison") {
		const auto index = new epic::index::Nevison(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("NevisonPH") {
		const auto index = new epic::index::NevisonPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Owen") {
		const auto index = new epic::index::Owen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.166667, 0.166667, 0.0833333, 0.0833333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexF") {
		const auto index = new epic::index::PowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.325, 0.16875, 0.16875, 0.16875, 0.16875};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexG") {
		const auto index = new epic::index::PowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.294118, 0.176471, 0.176471, 0.176471, 0.176471};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexGPH") {
		const auto index = new epic::index::PowerIndexGPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.294118, 0.176471, 0.176471, 0.176471, 0.176471};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicGood") {
		const auto index = new epic::index::PublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.166667, 0.166667, 0.166667, 0.166667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpTheta") {
		const auto index = new epic::index::PublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.294118, 0.176471, 0.176471, 0.176471, 0.176471};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpThetaPH") {
		const auto index = new epic::index::PublicHelpThetaPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.294118, 0.176471, 0.176471, 0.176471, 0.176471};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpXi") {
		const auto index = new epic::index::PublicHelpXi(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.359021, 0.160245, 0.160245, 0.160245, 0.160245};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Rae") {
		const auto index = new epic::index::Rae(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhaf") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {14, 2, 2, 2, 2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhafBelow") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {14, 2, 2, 2, 2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawDeeganPackel") {
		const auto index = new epic::index::RawDeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2, 0.75, 0.75, 0.75, 0.75};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawFelsenthal") {
		const auto index = new epic::index::RawFelsenthal(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4, 1, 1, 1, 1};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawJohnston") {
		const auto index = new epic::index::RawJohnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {12, 0.75, 0.75, 0.75, 0.75};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexF") {
		const auto index = new epic::index::RawPowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {5.2, 2.7, 2.7, 2.7, 2.7};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexG") {
		const auto index = new epic::index::RawPowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15, 9, 9, 9, 9};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicGood") {
		const auto index = new epic::index::RawPublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4, 2, 2, 2, 2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpTheta") {
		const auto index = new epic::index::RawPublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15, 9, 9, 9, 9};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpThetaSD") {
		const auto index = new epic::index::RawPublicHelpThetaSD(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15, 9, 9, 9, 9};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubik") {
		const auto index = new epic::index::RawShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {72, 12, 12, 12, 12};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubikBelow") {
		const auto index = new epic::index::RawShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {72, 12, 12, 12, 12};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubik") {
		const auto index = new epic::index::ShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.6, 0.1, 0.1, 0.1, 0.1};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubikBelow") {
		const auto index = new epic::index::ShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.6, 0.1, 0.1, 0.1, 0.1};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueW") {
		const auto index = new epic::index::SingleValueW();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {16};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWM") {
		const auto index = new epic::index::SingleValueWM();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {5};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWS") {
		const auto index = new epic::index::SingleValueWS(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {8};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
}

TEST_CASE("TestCase #08: Game: Quota: 40; Weights: 32, 15, 10, 10, 9; Precoalitions = {{0}, {1}, {2}, {3, 4}};", "[index]") {
	epic::longUInt quota = 40;
	std::vector<epic::longUInt> weights = {32, 15, 10, 10, 9};
	std::vector<std::vector<int>> precoalitions = {{0}, {1}, {2}, {3, 4}};

	const auto game = new epic::Game(quota, weights, false);
	const auto precoalition_game = new epic::PrecoalitionGame(quota, weights, false, precoalitions);
	const auto approx = new epic::FastUpperBoundApproximation(*game);
	epic::bigFloat tolerance = 0.00001;

	SECTION("AbsoluteBanzhaf") {
		const auto index = new epic::index::AbsoluteBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.125, 0.125, 0.125, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePowerIndexG") {
		const auto index = new epic::index::AbsolutePowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePublicGood") {
		const auto index = new epic::index::AbsolutePublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.8, 0.4, 0.4, 0.4, 0.4};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Banzhaf") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.636364, 0.0909091, 0.0909091, 0.0909091, 0.0909091};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafBelow") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.636364, 0.0909091, 0.0909091, 0.0909091, 0.0909091};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafOwen") {
		const auto index = new epic::index::BanzhafOwen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.25, 0.125, 0.25, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanCollective") {
		const auto index = new epic::index::ColemanCollective();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.5, 0.5, 0.5};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanInitiative") {
		const auto index = new epic::index::ColemanInitiative();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.125, 0.125, 0.125, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanPreventive") {
		const auto index = new epic::index::ColemanPreventive();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.125, 0.125, 0.125, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("DeeganPackel") {
		const auto index = new epic::index::DeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.4, 0.15, 0.15, 0.15, 0.15};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("FelsenthalIndex") {
		const auto index = new epic::index::FelsenthalIndex(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.125, 0.125, 0.125, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("HarsanyiNashIndex") {
		const auto index = new epic::index::HarsanyiNashIndex();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.2, 0.2, 0.2, 0.2, 0.2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Johnston") {
		const auto index = new epic::index::Johnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.8, 0.05, 0.05, 0.05, 0.05};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuninger") {
		const auto index = new epic::index::KoenigBraeuninger(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuningerPH") {
		const auto index = new epic::index::KoenigBraeuningerPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Nevison") {
		const auto index = new epic::index::Nevison(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("NevisonPH") {
		const auto index = new epic::index::NevisonPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Owen") {
		const auto index = new epic::index::Owen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.166667, 0.0833333, 0.166667, 0.0833333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexF") {
		const auto index = new epic::index::PowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.325, 0.16875, 0.16875, 0.16875, 0.16875};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexG") {
		const auto index = new epic::index::PowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.294118, 0.176471, 0.176471, 0.176471, 0.176471};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexGPH") {
		const auto index = new epic::index::PowerIndexGPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.294118, 0.176471, 0.176471, 0.176471, 0.176471};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicGood") {
		const auto index = new epic::index::PublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.166667, 0.166667, 0.166667, 0.166667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpTheta") {
		const auto index = new epic::index::PublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.294118, 0.176471, 0.176471, 0.176471, 0.176471};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpThetaPH") {
		const auto index = new epic::index::PublicHelpThetaPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.294118, 0.176471, 0.176471, 0.176471, 0.176471};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpXi") {
		const auto index = new epic::index::PublicHelpXi(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.359021, 0.160245, 0.160245, 0.160245, 0.160245};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Rae") {
		const auto index = new epic::index::Rae(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhaf") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {14, 2, 2, 2, 2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhafBelow") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {14, 2, 2, 2, 2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawDeeganPackel") {
		const auto index = new epic::index::RawDeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2, 0.75, 0.75, 0.75, 0.75};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawFelsenthal") {
		const auto index = new epic::index::RawFelsenthal(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4, 1, 1, 1, 1};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawJohnston") {
		const auto index = new epic::index::RawJohnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {12, 0.75, 0.75, 0.75, 0.75};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexF") {
		const auto index = new epic::index::RawPowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {5.2, 2.7, 2.7, 2.7, 2.7};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexG") {
		const auto index = new epic::index::RawPowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15, 9, 9, 9, 9};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicGood") {
		const auto index = new epic::index::RawPublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4, 2, 2, 2, 2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpTheta") {
		const auto index = new epic::index::RawPublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15, 9, 9, 9, 9};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpThetaSD") {
		const auto index = new epic::index::RawPublicHelpThetaSD(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15, 9, 9, 9, 9};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubik") {
		const auto index = new epic::index::RawShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {72, 12, 12, 12, 12};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubikBelow") {
		const auto index = new epic::index::RawShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {72, 12, 12, 12, 12};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubik") {
		const auto index = new epic::index::ShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.6, 0.1, 0.1, 0.1, 0.1};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubikBelow") {
		const auto index = new epic::index::ShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.6, 0.1, 0.1, 0.1, 0.1};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueW") {
		const auto index = new epic::index::SingleValueW();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {16};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWM") {
		const auto index = new epic::index::SingleValueWM();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {5};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWS") {
		const auto index = new epic::index::SingleValueWS(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {8};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
}

TEST_CASE("TestCase #09: Game: Quota: 11; Weights: 5, 5, 3, 3, 3; Precoalitions = {{0}, {1}, {2}, {3, 4}};", "[index]") {
	epic::longUInt quota = 11;
	std::vector<epic::longUInt> weights = {5, 5, 3, 3, 3};
	std::vector<std::vector<int>> precoalitions = {{0}, {1}, {2}, {3, 4}};

	const auto game = new epic::Game(quota, weights, false);
	const auto precoalition_game = new epic::PrecoalitionGame(quota, weights, false, precoalitions);
	const auto approx = new epic::FastUpperBoundApproximation(*game);
	epic::bigFloat tolerance = 0.00001;

	SECTION("AbsoluteBanzhaf") {
		const auto index = new epic::index::AbsoluteBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.4375, 0.4375, 0.3125, 0.3125, 0.3125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePowerIndexG") {
		const auto index = new epic::index::AbsolutePowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.733333, 0.733333, 0.666667, 0.666667, 0.666667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePublicGood") {
		const auto index = new epic::index::AbsolutePublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.666667, 0.666667, 0.555556, 0.555556, 0.555556};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Banzhaf") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.241379, 0.241379, 0.172414, 0.172414, 0.172414};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafBelow") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.241379, 0.241379, 0.172414, 0.172414, 0.172414};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafOwen") {
		const auto index = new epic::index::BanzhafOwen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.375, 0.375, 0.3125, 0.3125, 0.125};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanCollective") {
		const auto index = new epic::index::ColemanCollective();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.46875, 0.46875, 0.46875, 0.46875, 0.46875};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanInitiative") {
		const auto index = new epic::index::ColemanInitiative();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.411765, 0.411765, 0.294118, 0.294118, 0.294118};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanPreventive") {
		const auto index = new epic::index::ColemanPreventive();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.466667, 0.466667, 0.333333, 0.333333, 0.333333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("DeeganPackel") {
		const auto index = new epic::index::DeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.222222, 0.222222, 0.185185, 0.185185, 0.185185};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("FelsenthalIndex") {
		const auto index = new epic::index::FelsenthalIndex(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.222222, 0.222222, 0.185185, 0.185185, 0.185185};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("HarsanyiNashIndex") {
		const auto index = new epic::index::HarsanyiNashIndex();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.2, 0.2, 0.2, 0.2, 0.2};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Johnston") {
		const auto index = new epic::index::Johnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.272727, 0.272727, 0.151515, 0.151515, 0.151515};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuninger") {
		const auto index = new epic::index::KoenigBraeuninger(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.733333, 0.733333, 0.666667, 0.666667, 0.666667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuningerPH") {
		const auto index = new epic::index::KoenigBraeuningerPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.733333, 0.733333, 0.666667, 0.666667, 0.666667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Nevison") {
		const auto index = new epic::index::Nevison(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.6875, 0.6875, 0.625, 0.625, 0.625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("NevisonPH") {
		const auto index = new epic::index::NevisonPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.6875, 0.6875, 0.625, 0.625, 0.625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Owen") {
		const auto index = new epic::index::Owen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.25, 0.25, 0.208333, 0.208333, 0.0833333};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexF") {
		const auto index = new epic::index::PowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.213333, 0.213333, 0.191111, 0.191111, 0.191111};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexG") {
		const auto index = new epic::index::PowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.211538, 0.211538, 0.192308, 0.192308, 0.192308};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexGPH") {
		const auto index = new epic::index::PowerIndexGPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.211538, 0.211538, 0.192308, 0.192308, 0.192308};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicGood") {
		const auto index = new epic::index::PublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.222222, 0.222222, 0.185185, 0.185185, 0.185185};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpTheta") {
		const auto index = new epic::index::PublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.211538, 0.211538, 0.192308, 0.192308, 0.192308};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpThetaPH") {
		const auto index = new epic::index::PublicHelpThetaPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.211538, 0.211538, 0.192308, 0.192308, 0.192308};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpXi") {
		const auto index = new epic::index::PublicHelpXi(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.214981, 0.214981, 0.190012, 0.190012, 0.190012};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Rae") {
		const auto index = new epic::index::Rae(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.71875, 0.71875, 0.65625, 0.65625, 0.65625};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhaf") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7, 7, 5, 5, 5};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhafBelow") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7, 7, 5, 5, 5};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawDeeganPackel") {
		const auto index = new epic::index::RawDeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2, 2, 1.66667, 1.66667, 1.66667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawFelsenthal") {
		const auto index = new epic::index::RawFelsenthal(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {6, 6, 5, 5, 5};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawJohnston") {
		const auto index = new epic::index::RawJohnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {3, 3, 1.66667, 1.66667, 1.66667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexF") {
		const auto index = new epic::index::RawPowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {3.2, 3.2, 2.86667, 2.86667, 2.86667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexG") {
		const auto index = new epic::index::RawPowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {11, 11, 10, 10, 10};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicGood") {
		const auto index = new epic::index::RawPublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {6, 6, 5, 5, 5};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpTheta") {
		const auto index = new epic::index::RawPublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {11, 11, 10, 10, 10};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpThetaSD") {
		const auto index = new epic::index::RawPublicHelpThetaSD(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {11, 11, 10, 10, 10};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubik") {
		const auto index = new epic::index::RawShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {30, 30, 20, 20, 20};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubikBelow") {
		const auto index = new epic::index::RawShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {30, 30, 20, 20, 20};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubik") {
		const auto index = new epic::index::ShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.25, 0.25, 0.166667, 0.166667, 0.166667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubikBelow") {
		const auto index = new epic::index::ShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.25, 0.25, 0.166667, 0.166667, 0.166667};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueW") {
		const auto index = new epic::index::SingleValueW();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWM") {
		const auto index = new epic::index::SingleValueWM();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {9};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWS") {
		const auto index = new epic::index::SingleValueWS(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {27};

		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
}

TEST_CASE("TestCase #10: Game: Quota: 8; Weights: 5, 5, 2, 1, 1; Precoalitions = {{0,2}, {1,3}, {4}};", "[index]") {
	epic::longUInt quota = 8;
	std::vector<epic::longUInt> weights = {5, 5, 2, 1, 1};
	std::vector<std::vector<int>> precoalitions = {{0,2}, {1,3},{4}};
	//{0.125, 0.375, 0.375, 0.25, 0.125};

	const auto game = new epic::Game(quota, weights, false);
	const auto precoalition_game = new epic::PrecoalitionGame(quota, weights, false, precoalitions);
	const auto approx = new epic::FastUpperBoundApproximation(*game);
	epic::bigFloat tolerance = 0.00001;

SECTION("AbsoluteBanzhaf") {
	const auto index = new epic::index::AbsoluteBanzhaf();
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.375, 0.125, 0.125};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("AbsolutePowerIndexG") {
	const auto index = new epic::index::AbsolutePowerIndexG(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.785714, 0.785714, 0.714286, 0.571429, 0.571429};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("AbsolutePublicGood") {
	const auto index = new epic::index::AbsolutePublicGood();
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.6, 0.6, 0.8, 0.4, 0.4};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("Banzhaf") {
	const auto index = new epic::index::Banzhaf();
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.307692, 0.307692, 0.230769, 0.0769231, 0.0769231};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("BanzhafBelow") {
	const auto index = new epic::index::BanzhafBelow();
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.307692, 0.307692, 0.230769, 0.0769231, 0.0769231};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("BanzhafOwen") {
	const auto index = new epic::index::BanzhafOwen();
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(precoalition_game);
	std::vector<epic::bigFloat> expected_result = {0.125, 0.375, 0.375, 0.25, 0.125};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("ColemanCollective") {
	const auto index = new epic::index::ColemanCollective();
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.4375, 0.4375, 0.4375, 0.4375, 0.4375};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("ColemanInitiative") {
	const auto index = new epic::index::ColemanInitiative();
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.444444, 0.444444, 0.333333, 0.111111, 0.111111};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("ColemanPreventive") {
	const auto index = new epic::index::ColemanPreventive();
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.571429, 0.571429, 0.428571, 0.142857, 0.142857};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("DeeganPackel") {
	const auto index = new epic::index::DeeganPackel();
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.233333, 0.233333, 0.266667, 0.133333, 0.133333};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("FelsenthalIndex") {
	const auto index = new epic::index::FelsenthalIndex(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0, 0, 0};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("HarsanyiNashIndex") {
	const auto index = new epic::index::HarsanyiNashIndex();
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.2, 0.2, 0.2, 0.2, 0.2};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("Johnston") {
	const auto index = new epic::index::Johnston(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.333333, 0.333333, 0.212121, 0.0606061, 0.0606061};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("KoenigBraeuninger") {
	const auto index = new epic::index::KoenigBraeuninger(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.785714, 0.785714, 0.714286, 0.571429, 0.571429};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("KoenigBraeuningerPH") {
	const auto index = new epic::index::KoenigBraeuningerPH(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.785714, 0.785714, 0.714286, 0.571429, 0.571429};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("Nevison") {
	const auto index = new epic::index::Nevison(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.6875, 0.6875, 0.625, 0.5, 0.5};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("NevisonPH") {
	const auto index = new epic::index::NevisonPH(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.6875, 0.6875, 0.625, 0.5, 0.5};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("Owen") {
	const auto index = new epic::index::Owen();
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(precoalition_game);
	std::vector<epic::bigFloat> expected_result = {0.0833333, 0.333333, 0.333333, 0.166667, 0.0833333};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("PowerIndexF") {
	const auto index = new epic::index::PowerIndexF(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.240476, 0.240476, 0.204762, 0.157143, 0.157143};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("PowerIndexG") {
	const auto index = new epic::index::PowerIndexG(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.229167, 0.229167, 0.208333, 0.166667, 0.166667};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("PowerIndexGPH") {
	const auto index = new epic::index::PowerIndexGPH(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.229167, 0.229167, 0.208333, 0.166667, 0.166667};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("PublicGood") {
	const auto index = new epic::index::PublicGood();
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.214286, 0.214286, 0.285714, 0.142857, 0.142857};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("PublicHelpTheta") {
	const auto index = new epic::index::PublicHelpTheta(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.229167, 0.229167, 0.208333, 0.166667, 0.166667};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("PublicHelpThetaPH") {
	const auto index = new epic::index::PublicHelpThetaPH(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.229167, 0.229167, 0.208333, 0.166667, 0.166667};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("PublicHelpXi") {
	const auto index = new epic::index::PublicHelpXi(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.255772, 0.255772, 0.197406, 0.145525, 0.145525};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("Rae") {
	const auto index = new epic::index::Rae(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.6875, 0.5625, 0.5625};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("RawBanzhaf") {
	const auto index = new epic::index::RawBanzhaf();
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {8, 8, 6, 2, 2};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("RawBanzhafBelow") {
	const auto index = new epic::index::RawBanzhafBelow();
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {8, 8, 6, 2, 2};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("RawDeeganPackel") {
	const auto index = new epic::index::RawDeeganPackel();
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {1.16667, 1.16667, 1.33333, 0.666667, 0.666667};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("RawFelsenthal") {
	const auto index = new epic::index::RawFelsenthal(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {1, 1, 0, 0, 0};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("RawJohnston") {
	const auto index = new epic::index::RawJohnston(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {3.66667, 3.66667, 2.33333, 0.666667, 0.666667};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("RawPowerIndexF") {
	const auto index = new epic::index::RawPowerIndexF(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {3.36667, 3.36667, 2.86667, 2.2, 2.2};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("RawPowerIndexG") {
	const auto index = new epic::index::RawPowerIndexG(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {11, 11, 10, 8, 8};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("RawPublicGood") {
	const auto index = new epic::index::RawPublicGood();
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {3, 3, 4, 2, 2};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("RawPublicHelpTheta") {
	const auto index = new epic::index::RawPublicHelpTheta(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {11, 11, 10, 8, 8};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("RawPublicHelpThetaSD") {
	const auto index = new epic::index::RawPublicHelpThetaSD(game);
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {11, 11, 10, 8, 8};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
SECTION("RawShapleyShubik") {
	const auto index = new epic::index::RawShapleyShubik();
	epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																	index->getOperationRequirement(),
																	epic::IntRepresentation::DEFAULT));
	auto actual_result = index->calculate(game);
	std::vector<epic::bigFloat> expected_result = {38, 38, 28, 8, 8};
	//REQUIRE(actual_result == expected_result);
	REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
	epic::lint::GlobalCalculator::remove();
}
	SECTION("RawShapleyShubikBelow") {
		const auto index = new epic::index::RawShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {38, 38, 28, 8, 8};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubik") {
		const auto index = new epic::index::ShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.316667, 0.316667, 0.233333, 0.0666667, 0.0666667};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubikBelow") {
		const auto index = new epic::index::ShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.316667, 0.316667, 0.233333, 0.0666667, 0.0666667};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueW") {
		const auto index = new epic::index::SingleValueW();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {14};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWM") {
		const auto index = new epic::index::SingleValueWM();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWS") {
		const auto index = new epic::index::SingleValueWS(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2};
		REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
}

TEST_CASE("TestCase #11: Game: Quota: 9; Weights: 6, 1, 2, 1, 1, 3, 1; Precoalitions = {{0, 1}, {2, 3, 4}, {5, 6}};", "[index]") {
	epic::longUInt quota = 9;
	std::vector<epic::longUInt> weights = {6, 1, 2, 1, 1, 3, 1};
	std::vector<std::vector<int>> precoalitions = {{0, 1}, {2, 3, 4}, {5, 6}};

	const auto game = new epic::Game(quota, weights, false);
	const auto precoalition_game = new epic::PrecoalitionGame(quota, weights, false, precoalitions);
	const auto approx = new epic::FastUpperBoundApproximation(*game);
	epic::bigFloat tolerance = 0.0001;

	SECTION("AbsoluteBanzhaf") {
		const auto index = new epic::index::AbsoluteBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.796875, 0.203125, 0.171875, 0.078125, 0.078125, 0.078125, 0.078125};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePowerIndexG") {
		const auto index = new epic::index::AbsolutePowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.981132, 0.622642, 0.603774, 0.54717, 0.54717, 0.54717, 0.54717};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePublicGood") {
		const auto index = new epic::index::AbsolutePublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9, 0.2, 0.5, 0.5, 0.5, 0.5, 0.5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Banzhaf") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.536842, 0.136842, 0.115789, 0.0526316, 0.0526316, 0.0526316, 0.0526316};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafBelow") {
		const auto index = new epic::index::BanzhafBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.536842, 0.136842, 0.115789, 0.0526316, 0.0526316, 0.0526316, 0.0526316};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafOwen") {
		const auto index = new epic::index::BanzhafOwen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.625, 0.25, 0.1875, 0, 0.0625, 0.0625, 0.125};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanCollective") {
		const auto index = new epic::index::ColemanCollective();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.414063, 0.414063, 0.414063, 0.414063, 0.414063, 0.414063, 0.414063};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanInitiative") {
		const auto index = new epic::index::ColemanInitiative();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.68, 0.173333, 0.146667, 0.0666667, 0.0666667, 0.0666667, 0.0666667};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanPreventive") {
		const auto index = new epic::index::ColemanPreventive();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.962264, 0.245283, 0.207547, 0.0943396, 0.0943396, 0.0943396, 0.0943396};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("DeeganPackel") {
		const auto index = new epic::index::DeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.283333, 0.0666667, 0.15, 0.125, 0.125, 0.125, 0.125};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("FelsenthalIndex") {
		const auto index = new epic::index::FelsenthalIndex(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0, 0, 0, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("HarsanyiNashIndex") {
		const auto index = new epic::index::HarsanyiNashIndex();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.142857, 0.142857, 0.142857, 0.142857, 0.142857, 0.142857, 0.142857};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Johnston") {
		const auto index = new epic::index::Johnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.698718, 0.11859, 0.0865385, 0.0240385, 0.0240385, 0.0240385, 0.0240385};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuninger") {
		const auto index = new epic::index::KoenigBraeuninger(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.981132, 0.622642, 0.603774, 0.54717, 0.54717, 0.54717, 0.54717};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuningerPH") {
		const auto index = new epic::index::KoenigBraeuningerPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.981132, 0.622642, 0.603774, 0.54717, 0.54717, 0.54717, 0.54717};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Nevison") {
		const auto index = new epic::index::Nevison(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.8125, 0.515625, 0.5, 0.453125, 0.453125, 0.453125, 0.453125};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("NevisonPH") {
		const auto index = new epic::index::NevisonPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.8125, 0.515625, 0.5, 0.453125, 0.453125, 0.453125, 0.453125};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Owen") {
		const auto index = new epic::index::Owen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.166667, 0.111111, 0, 0.0277778, 0.0277778, 0.166667};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexF") {
		const auto index = new epic::index::PowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.238544, 0.14735, 0.137916, 0.119048, 0.119048, 0.119048, 0.119048};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexG") {
		const auto index = new epic::index::PowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.223176, 0.141631, 0.137339, 0.124464, 0.124464, 0.124464, 0.124464};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexGPH") {
		const auto index = new epic::index::PowerIndexGPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.223176, 0.141631, 0.137339, 0.124464, 0.124464, 0.124464, 0.124464};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicGood") {
		const auto index = new epic::index::PublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.25, 0.0555556, 0.138889, 0.138889, 0.138889, 0.138889, 0.138889};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpTheta") {
		const auto index = new epic::index::PublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.223176, 0.141631, 0.137339, 0.124464, 0.124464, 0.124464, 0.124464};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpThetaPH") {
		const auto index = new epic::index::PublicHelpThetaPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.223176, 0.141631, 0.137339, 0.124464, 0.124464, 0.124464, 0.124464};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpXi") {
		const auto index = new epic::index::PublicHelpXi(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.256612, 0.15751, 0.137993, 0.111971, 0.111971, 0.111971, 0.111971};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Rae") {
		const auto index = new epic::index::Rae(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.898438, 0.601563, 0.585938, 0.539063, 0.539063, 0.539063, 0.539063};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhaf") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {51, 13, 11, 5, 5, 5, 5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhafBelow") {
		const auto index = new epic::index::RawBanzhafBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {51, 13, 11, 5, 5, 5, 5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawDeeganPackel") {
		const auto index = new epic::index::RawDeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2.83333, 0.666667, 1.5, 1.25, 1.25, 1.25, 1.25};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawFelsenthal") {
		const auto index = new epic::index::RawFelsenthal(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {1, 1, 0, 0, 0, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawJohnston") {
		const auto index = new epic::index::RawJohnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {36.3333, 6.16667, 4.5, 1.25, 1.25, 1.25, 1.25};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexF") {
		const auto index = new epic::index::RawPowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {12.6429, 7.80952, 7.30952, 6.30952, 6.30952, 6.30952, 6.30952};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexG") {
		const auto index = new epic::index::RawPowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {52, 33, 32, 29, 29, 29, 29};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicGood") {
		const auto index = new epic::index::RawPublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {9, 2, 5, 5, 5, 5, 5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpTheta") {
		const auto index = new epic::index::RawPublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {52, 33, 32, 29, 29, 29, 29};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpThetaSD") {
		const auto index = new epic::index::RawPublicHelpThetaSD(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {52, 33, 32, 29, 29, 29, 29};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubik") {
		const auto index = new epic::index::RawShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2712, 696, 528, 276, 276, 276, 276};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubikBelow") {
		const auto index = new epic::index::RawShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2712, 696, 528, 276, 276, 276, 276};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubik") {
		const auto index = new epic::index::ShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.538095, 0.138095, 0.104762, 0.0547619, 0.0547619, 0.0547619, 0.0547619};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubikBelow") {
		const auto index = new epic::index::ShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.538095, 0.138095, 0.104762, 0.0547619, 0.0547619, 0.0547619, 0.0547619};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueW") {
		const auto index = new epic::index::SingleValueW();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {53};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWM") {
		const auto index = new epic::index::SingleValueWM();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {10};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWS") {
		const auto index = new epic::index::SingleValueWS(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
}

TEST_CASE("TestCase #12: Game: Quota: 8; Weights: 7, 2, 4, 1, 1; Precoalitions = {{0, 1}, {2, 3}, {4}};", "[index]") {
	epic::longUInt quota = 8;
	std::vector<epic::longUInt> weights = {7, 2, 4, 1, 1};
	std::vector<std::vector<int>> precoalitions = {{0, 1}, {2, 3}, {4}};

	const auto game = new epic::Game(quota, weights, false);
	const auto precoalition_game = new epic::PrecoalitionGame(quota, weights, false, precoalitions);
	const auto approx = new epic::FastUpperBoundApproximation(*game);
	epic::bigFloat tolerance = 0.00001;

	SECTION("AbsoluteBanzhaf") {
		const auto index = new epic::index::AbsoluteBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.125, 0.125, 0.125, 0.125};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePowerIndexG") {
		const auto index = new epic::index::AbsolutePowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePublicGood") {
		const auto index = new epic::index::AbsolutePublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.8, 0.4, 0.4, 0.4, 0.4};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Banzhaf") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.636364, 0.0909091, 0.0909091, 0.0909091, 0.0909091};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafBelow") {
		const auto index = new epic::index::BanzhafBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.636364, 0.0909091, 0.0909091, 0.0909091, 0.0909091};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafOwen") {
		const auto index = new epic::index::BanzhafOwen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0, 0.25, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanCollective") {
		const auto index = new epic::index::ColemanCollective();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.5, 0.5, 0.5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanInitiative") {
		const auto index = new epic::index::ColemanInitiative();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.125, 0.125, 0.125, 0.125};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanPreventive") {
		const auto index = new epic::index::ColemanPreventive();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.125, 0.125, 0.125, 0.125};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("DeeganPackel") {
		const auto index = new epic::index::DeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.4, 0.15, 0.15, 0.15, 0.15};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("FelsenthalIndex") {
		const auto index = new epic::index::FelsenthalIndex(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.125, 0.125, 0.125, 0.125};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("HarsanyiNashIndex") {
		const auto index = new epic::index::HarsanyiNashIndex();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.2, 0.2, 0.2, 0.2, 0.2};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Johnston") {
		const auto index = new epic::index::Johnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.8, 0.05, 0.05, 0.05, 0.05};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuninger") {
		const auto index = new epic::index::KoenigBraeuninger(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuningerPH") {
		const auto index = new epic::index::KoenigBraeuningerPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Nevison") {
		const auto index = new epic::index::Nevison(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("NevisonPH") {
		const auto index = new epic::index::NevisonPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Owen") {
		const auto index = new epic::index::Owen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.666667, 0, 0.333333, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexF") {
		const auto index = new epic::index::PowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.325, 0.16875, 0.16875, 0.16875, 0.16875};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexG") {
		const auto index = new epic::index::PowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.294118, 0.176471, 0.176471, 0.176471, 0.176471};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexGPH") {
		const auto index = new epic::index::PowerIndexGPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.294118, 0.176471, 0.176471, 0.176471, 0.176471};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicGood") {
		const auto index = new epic::index::PublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.166667, 0.166667, 0.166667, 0.166667};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpTheta") {
		const auto index = new epic::index::PublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.294118, 0.176471, 0.176471, 0.176471, 0.176471};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpThetaPH") {
		const auto index = new epic::index::PublicHelpThetaPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.294118, 0.176471, 0.176471, 0.176471, 0.176471};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpXi") {
		const auto index = new epic::index::PublicHelpXi(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.359021, 0.160245, 0.160245, 0.160245, 0.160245};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Rae") {
		const auto index = new epic::index::Rae(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625, 0.5625};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhaf") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {14, 2, 2, 2, 2};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhafBelow") {
		const auto index = new epic::index::RawBanzhafBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {14, 2, 2, 2, 2};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawDeeganPackel") {
		const auto index = new epic::index::RawDeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2, 0.75, 0.75, 0.75, 0.75};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawFelsenthal") {
		const auto index = new epic::index::RawFelsenthal(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4, 1, 1, 1, 1};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawJohnston") {
		const auto index = new epic::index::RawJohnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {12, 0.75, 0.75, 0.75, 0.75};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexF") {
		const auto index = new epic::index::RawPowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {5.2, 2.7, 2.7, 2.7, 2.7};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexG") {
		const auto index = new epic::index::RawPowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15, 9, 9, 9, 9};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicGood") {
		const auto index = new epic::index::RawPublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4, 2, 2, 2, 2};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpTheta") {
		const auto index = new epic::index::RawPublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15, 9, 9, 9, 9};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpThetaSD") {
		const auto index = new epic::index::RawPublicHelpThetaSD(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15, 9, 9, 9, 9};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubik") {
		const auto index = new epic::index::RawShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {72, 12, 12, 12, 12};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubikBelow") {
		const auto index = new epic::index::RawShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {72, 12, 12, 12, 12};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubik") {
		const auto index = new epic::index::ShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.6, 0.1, 0.1, 0.1, 0.1};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubikBelow") {
		const auto index = new epic::index::ShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.6, 0.1, 0.1, 0.1, 0.1};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueW") {
		const auto index = new epic::index::SingleValueW();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {16};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWM") {
		const auto index = new epic::index::SingleValueWM();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWS") {
		const auto index = new epic::index::SingleValueWS(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {8};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
}

TEST_CASE("TestCase #13: Game: Quota: 8; Weights: 5, 3, 5, 1, 1; Precoalitions = {{0, 1}, {2, 3}, {4}};", "[index]") {
	epic::longUInt quota = 8;
	std::vector<epic::longUInt> weights = {5, 3, 5, 1, 1};
	std::vector<std::vector<int>> precoalitions = {{0, 1}, {2, 3}, {4}};

	const auto game = new epic::Game(quota, weights, false);
	const auto precoalition_game = new epic::PrecoalitionGame(quota, weights, false, precoalitions);
	const auto approx = new epic::FastUpperBoundApproximation(*game);
	epic::bigFloat tolerance = 0.00001;

	SECTION("AbsoluteBanzhaf") {
		const auto index = new epic::index::AbsoluteBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.5, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePublicGood") {
		const auto index = new epic::index::AbsolutePublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.666667, 0.666667, 0.666667, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Banzhaf") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.333333, 0.333333, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafBelow") {
		const auto index = new epic::index::BanzhafBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.333333, 0.333333, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafOwen") {
		const auto index = new epic::index::BanzhafOwen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0, 0.5, 0.5, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanCollective") {
		const auto index = new epic::index::ColemanCollective();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.5, 0.5, 0.5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanInitiative") {
		const auto index = new epic::index::ColemanInitiative();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.5, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanPreventive") {
		const auto index = new epic::index::ColemanPreventive();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.5, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("DeeganPackel") {
		const auto index = new epic::index::DeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.333333, 0.333333, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("FelsenthalIndex") {
		const auto index = new epic::index::FelsenthalIndex(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.333333, 0.333333, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("HarsanyiNashIndex") {
		const auto index = new epic::index::HarsanyiNashIndex();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.2, 0.2, 0.2, 0.2, 0.2};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Johnston") {
		const auto index = new epic::index::Johnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.333333, 0.333333, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuninger") {
		const auto index = new epic::index::KoenigBraeuninger(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.75, 0.5, 0.5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuningerPH") {
		const auto index = new epic::index::KoenigBraeuningerPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.75, 0.5, 0.5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Nevison") {
		const auto index = new epic::index::Nevison(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.75, 0.5, 0.5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("NevisonPH") {
		const auto index = new epic::index::NevisonPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.75, 0.5, 0.5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Owen") {
		const auto index = new epic::index::Owen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0, 0.5, 0.5, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicGood") {
		const auto index = new epic::index::PublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.333333, 0.333333, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpTheta") {
		const auto index = new epic::index::PublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.230769, 0.230769, 0.230769, 0.153846, 0.153846};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpThetaPH") {
		const auto index = new epic::index::PublicHelpThetaPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.230769, 0.230769, 0.230769, 0.153846, 0.153846};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpXi") {
		const auto index = new epic::index::PublicHelpXi(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.254679, 0.254679, 0.254679, 0.117981, 0.117981};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Rae") {
		const auto index = new epic::index::Rae(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.75, 0.5, 0.5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhaf") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {8, 8, 8, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhafBelow") {
		const auto index = new epic::index::RawBanzhafBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {8, 8, 8, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawDeeganPackel") {
		const auto index = new epic::index::RawDeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {1, 1, 1, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawFelsenthal") {
		const auto index = new epic::index::RawFelsenthal(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2, 2, 2, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawJohnston") {
		const auto index = new epic::index::RawJohnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4, 4, 4, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicGood") {
		const auto index = new epic::index::RawPublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2, 2, 2, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpTheta") {
		const auto index = new epic::index::RawPublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {12, 12, 12, 8, 8};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpThetaSD") {
		const auto index = new epic::index::RawPublicHelpThetaSD(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {12, 12, 12, 8, 8};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubik") {
		const auto index = new epic::index::RawShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {40, 40, 40, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubikBelow") {
		const auto index = new epic::index::RawShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {40, 40, 40, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubik") {
		const auto index = new epic::index::ShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.333333, 0.333333, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubikBelow") {
		const auto index = new epic::index::ShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.333333, 0.333333, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueW") {
		const auto index = new epic::index::SingleValueW();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {16};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWM") {
		const auto index = new epic::index::SingleValueWM();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {3};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWS") {
		const auto index = new epic::index::SingleValueWS(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {6};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
}

TEST_CASE("TestCase #14: Game: Quota: 8; Weights: 6, 1, 1, 1, 1, 5, 1, 1; Precoalitions = {{0, 1, 2, 3, 4}, {5, 6}, {7}};", "[index]") {
	epic::longUInt quota = 8;
	std::vector<epic::longUInt> weights = {6, 1, 1, 1, 1, 5, 1, 1};
	std::vector<std::vector<int>> precoalitions = {{0, 1, 2, 3, 4}, {5, 6}, {7}};

	const auto game = new epic::Game(quota, weights, false);
	const auto precoalition_game = new epic::PrecoalitionGame(quota, weights, false, precoalitions);
	const auto approx = new epic::FastUpperBoundApproximation(*game);
	epic::bigFloat tolerance = 0.0001;

	SECTION("AbsoluteBanzhaf") {
		const auto index = new epic::index::AbsoluteBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.617188, 0.382813, 0.117188, 0.117188, 0.117188, 0.117188, 0.117188, 0.117188};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePowerIndexG") {
		const auto index = new epic::index::AbsolutePowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.742331, 0.650307, 0.546012, 0.546012, 0.546012, 0.546012, 0.546012, 0.546012};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePublicGood") {
		const auto index = new epic::index::AbsolutePublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.444444, 0.583333, 0.416667, 0.416667, 0.416667, 0.416667, 0.416667, 0.416667};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Banzhaf") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.362385, 0.224771, 0.0688073, 0.0688073, 0.0688073, 0.0688073, 0.0688073, 0.0688073};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafBelow") {
		const auto index = new epic::index::BanzhafBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.362385, 0.224771, 0.0688073, 0.0688073, 0.0688073, 0.0688073, 0.0688073, 0.0688073};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafOwen") {
		const auto index = new epic::index::BanzhafOwen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0, 0, 0, 0.125, 0.125, 0.125, 0.125};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanCollective") {
		const auto index = new epic::index::ColemanCollective();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.636719, 0.636719, 0.636719, 0.636719, 0.636719, 0.636719, 0.636719, 0.636719};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanInitiative") {
		const auto index = new epic::index::ColemanInitiative();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.849462, 0.526882, 0.16129, 0.16129, 0.16129, 0.16129, 0.16129, 0.16129};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanPreventive") {
		const auto index = new epic::index::ColemanPreventive();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.484663, 0.300613, 0.0920245, 0.0920245, 0.0920245, 0.0920245, 0.0920245, 0.0920245};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("DeeganPackel") {
		const auto index = new epic::index::DeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.152778, 0.152778, 0.115741, 0.115741, 0.115741, 0.115741, 0.115741, 0.115741};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("FelsenthalIndex") {
		const auto index = new epic::index::FelsenthalIndex(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0, 0, 0, 0, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("HarsanyiNashIndex") {
		const auto index = new epic::index::HarsanyiNashIndex();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125, 0.125};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Johnston") {
		const auto index = new epic::index::Johnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.541322, 0.252066, 0.0344353, 0.0344353, 0.0344353, 0.0344353, 0.0344353, 0.0344353};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuninger") {
		const auto index = new epic::index::KoenigBraeuninger(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.742331, 0.650307, 0.546012, 0.546012, 0.546012, 0.546012, 0.546012, 0.546012};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuningerPH") {
		const auto index = new epic::index::KoenigBraeuningerPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.742331, 0.650307, 0.546012, 0.546012, 0.546012, 0.546012, 0.546012, 0.546012};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Nevison") {
		const auto index = new epic::index::Nevison(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.945313, 0.828125, 0.695313, 0.695313, 0.695313, 0.695313, 0.695313, 0.695313};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("NevisonPH") {
		const auto index = new epic::index::NevisonPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.945313, 0.828125, 0.695313, 0.695313, 0.695313, 0.695313, 0.695313, 0.695313};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Owen") {
		const auto index = new epic::index::Owen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.466667, 0, 0, 0, 0.133333, 0.133333, 0.133333, 0.133333};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexF") {
		const auto index = new epic::index::PowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.171012, 0.140337, 0.114775, 0.114775, 0.114775, 0.114775, 0.114775, 0.114775};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexG") {
		const auto index = new epic::index::PowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.159001, 0.13929, 0.116951, 0.116951, 0.116951, 0.116951, 0.116951, 0.116951};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexGPH") {
		const auto index = new epic::index::PowerIndexGPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.159001, 0.13929, 0.116951, 0.116951, 0.116951, 0.116951, 0.116951, 0.116951};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicGood") {
		const auto index = new epic::index::PublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.125984, 0.165354, 0.11811, 0.11811, 0.11811, 0.11811, 0.11811, 0.11811};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpTheta") {
		const auto index = new epic::index::PublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.159001, 0.13929, 0.116951, 0.116951, 0.116951, 0.116951, 0.116951, 0.116951};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpThetaPH") {
		const auto index = new epic::index::PublicHelpThetaPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.159001, 0.13929, 0.116951, 0.116951, 0.116951, 0.116951, 0.116951, 0.116951};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpXi") {
		const auto index = new epic::index::PublicHelpXi(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.186738, 0.141715, 0.111924, 0.111924, 0.111924, 0.111924, 0.111924, 0.111924};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Rae") {
		const auto index = new epic::index::Rae(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.808594, 0.691406, 0.558594, 0.558594, 0.558594, 0.558594, 0.558594, 0.558594};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhaf") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {79, 49, 15, 15, 15, 15, 15, 15};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhafBelow") {
		const auto index = new epic::index::RawBanzhafBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {79, 49, 15, 15, 15, 15, 15, 15};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawDeeganPackel") {
		const auto index = new epic::index::RawDeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {5.5, 5.5, 4.16667, 4.16667, 4.16667, 4.16667, 4.16667, 4.16667};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawFelsenthal") {
		const auto index = new epic::index::RawFelsenthal(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {1, 1, 0, 0, 0, 0, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawJohnston") {
		const auto index = new epic::index::RawJohnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {65.5, 30.5, 4.16667, 4.16667, 4.16667, 4.16667, 4.16667, 4.16667};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexF") {
		const auto index = new epic::index::RawPowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {27.875, 22.875, 18.7083, 18.7083, 18.7083, 18.7083, 18.7083, 18.7083};
		//REQUIRE(actual_result == expected_result);
		//
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexG") {
		const auto index = new epic::index::RawPowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {121, 106, 89, 89, 89, 89, 89, 89};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicGood") {
		const auto index = new epic::index::RawPublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {16, 21, 15, 15, 15, 15, 15, 15};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpTheta") {
		const auto index = new epic::index::RawPublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {121, 106, 89, 89, 89, 89, 89, 89};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpThetaSD") {
		const auto index = new epic::index::RawPublicHelpThetaSD(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {121, 106, 89, 89, 89, 89, 89, 89};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubik") {
		const auto index = new epic::index::RawShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15120, 9360, 2640, 2640, 2640, 2640, 2640, 2640};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubikBelow") {
		const auto index = new epic::index::RawShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15120, 9360, 2640, 2640, 2640, 2640, 2640, 2640};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubik") {
		const auto index = new epic::index::ShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.375, 0.232143, 0.0654762, 0.0654762, 0.0654762, 0.0654762, 0.0654762, 0.0654762};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubikBelow") {
		const auto index = new epic::index::ShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.375, 0.232143, 0.0654762, 0.0654762, 0.0654762, 0.0654762, 0.0654762, 0.0654762};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueW") {
		const auto index = new epic::index::SingleValueW();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {163};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWM") {
		const auto index = new epic::index::SingleValueWM();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {36};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWS") {
		const auto index = new epic::index::SingleValueWS(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
}

TEST_CASE("TestCase #15: Game: Quota: 8; Weights: 6, 1, 1, 5, 1, 1; Precoalitions = {{0, 1, 2}, {3, 4}, {5}};", "[index]") {
	epic::longUInt quota = 8;
	std::vector<epic::longUInt> weights = {6, 1, 1, 5, 1, 1};
	std::vector<std::vector<int>> precoalitions = {{0, 1, 2}, {3, 4}, {5}};

	const auto game = new epic::Game(quota, weights, false);
	const auto precoalition_game = new epic::PrecoalitionGame(quota, weights, false, precoalitions);
	const auto approx = new epic::FastUpperBoundApproximation(*game);
	epic::bigFloat tolerance = 0.00001;

	SECTION("AbsoluteBanzhaf") {
		const auto index = new epic::index::AbsoluteBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.6875, 0.3125, 0.1875, 0.1875, 0.1875, 0.1875};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePowerIndexG") {
		const auto index = new epic::index::AbsolutePowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.84375, 0.65625, 0.59375, 0.59375, 0.59375, 0.59375};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePublicGood") {
		const auto index = new epic::index::AbsolutePublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.636364, 0.454545, 0.545455, 0.545455, 0.545455, 0.545455};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Banzhaf") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.392857, 0.178571, 0.107143, 0.107143, 0.107143, 0.107143};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafBelow") {
		const auto index = new epic::index::BanzhafBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.392857, 0.178571, 0.107143, 0.107143, 0.107143, 0.107143};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafOwen") {
		const auto index = new epic::index::BanzhafOwen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0, 0, 0, 0.25, 0.25};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanCollective") {
		const auto index = new epic::index::ColemanCollective();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.5, 0.5, 0.5, 0.5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanInitiative") {
		const auto index = new epic::index::ColemanInitiative();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.6875, 0.3125, 0.1875, 0.1875, 0.1875, 0.1875};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanPreventive") {
		const auto index = new epic::index::ColemanPreventive();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.6875, 0.3125, 0.1875, 0.1875, 0.1875, 0.1875};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("DeeganPackel") {
		const auto index = new epic::index::DeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.227273, 0.136364, 0.159091, 0.159091, 0.159091, 0.159091};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("FelsenthalIndex") {
		const auto index = new epic::index::FelsenthalIndex(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0, 0, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("HarsanyiNashIndex") {
		const auto index = new epic::index::HarsanyiNashIndex();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.166667, 0.166667, 0.166667, 0.166667, 0.166667, 0.166667};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Johnston") {
		const auto index = new epic::index::Johnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.574074, 0.166667, 0.0648148, 0.0648148, 0.0648148, 0.0648148};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuninger") {
		const auto index = new epic::index::KoenigBraeuninger(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.84375, 0.65625, 0.59375, 0.59375, 0.59375, 0.59375};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuningerPH") {
		const auto index = new epic::index::KoenigBraeuningerPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.84375, 0.65625, 0.59375, 0.59375, 0.59375, 0.59375};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Nevison") {
		const auto index = new epic::index::Nevison(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.84375, 0.65625, 0.59375, 0.59375, 0.59375, 0.59375};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("NevisonPH") {
		const auto index = new epic::index::NevisonPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.84375, 0.65625, 0.59375, 0.59375, 0.59375, 0.59375};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Owen") {
		const auto index = new epic::index::Owen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.444444, 0, 0, 0, 0.277778, 0.277778};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexF") {
		const auto index = new epic::index::PowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.234375, 0.171875, 0.148437, 0.148437, 0.148437, 0.148437};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexG") {
		const auto index = new epic::index::PowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.217742, 0.169355, 0.153226, 0.153226, 0.153226, 0.153226};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexGPH") {
		const auto index = new epic::index::PowerIndexGPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.217742, 0.169355, 0.153226, 0.153226, 0.153226, 0.153226};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicGood") {
		const auto index = new epic::index::PublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.194444, 0.138889, 0.166667, 0.166667, 0.166667, 0.166667};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpTheta") {
		const auto index = new epic::index::PublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.217742, 0.169355, 0.153226, 0.153226, 0.153226, 0.153226};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpThetaPH") {
		const auto index = new epic::index::PublicHelpThetaPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.217742, 0.169355, 0.153226, 0.153226, 0.153226, 0.153226};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpXi") {
		const auto index = new epic::index::PublicHelpXi(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.25447, 0.177842, 0.141922, 0.141922, 0.141922, 0.141922};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Rae") {
		const auto index = new epic::index::Rae(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.84375, 0.65625, 0.59375, 0.59375, 0.59375, 0.59375};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhaf") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {22, 10, 6, 6, 6, 6};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhafBelow") {
		const auto index = new epic::index::RawBanzhafBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {22, 10, 6, 6, 6, 6};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawDeeganPackel") {
		const auto index = new epic::index::RawDeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2.5, 1.5, 1.75, 1.75, 1.75, 1.75};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawFelsenthal") {
		const auto index = new epic::index::RawFelsenthal(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {1, 1, 0, 0, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawJohnston") {
		const auto index = new epic::index::RawJohnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {15.5, 4.5, 1.75, 1.75, 1.75, 1.75};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexF") {
		const auto index = new epic::index::RawPowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7.5, 5.5, 4.75, 4.75, 4.75, 4.75};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexG") {
		const auto index = new epic::index::RawPowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {27, 21, 19, 19, 19, 19};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicGood") {
		const auto index = new epic::index::RawPublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7, 5, 6, 6, 6, 6};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpTheta") {
		const auto index = new epic::index::RawPublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {27, 21, 19, 19, 19, 19};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpThetaSD") {
		const auto index = new epic::index::RawPublicHelpThetaSD(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {27, 21, 19, 19, 19, 19};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubik") {
		const auto index = new epic::index::RawShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {288, 144, 72, 72, 72, 72};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubikBelow") {
		const auto index = new epic::index::RawShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {288, 144, 72, 72, 72, 72};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubik") {
		const auto index = new epic::index::ShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.4, 0.2, 0.1, 0.1, 0.1, 0.1};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubikBelow") {
		const auto index = new epic::index::ShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.4, 0.2, 0.1, 0.1, 0.1, 0.1};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueW") {
		const auto index = new epic::index::SingleValueW();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {32};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWM") {
		const auto index = new epic::index::SingleValueWM();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {11};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWS") {
		const auto index = new epic::index::SingleValueWS(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
}

TEST_CASE("TestCase #16: Game: Quota: 8; Weights: 6, 1, 1, 6, 1; Precoalitions = {{0, 1, 2}, {3}, {4}};", "[index]") {
	epic::longUInt quota = 8;
	std::vector<epic::longUInt> weights = {6, 1, 1, 6, 1};
	std::vector<std::vector<int>> precoalitions = {{0, 1, 2}, {3}, {4}};

	const auto game = new epic::Game(quota, weights, false);
	const auto precoalition_game = new epic::PrecoalitionGame(quota, weights, false, precoalitions);
	const auto approx = new epic::FastUpperBoundApproximation(*game);
	epic::bigFloat tolerance = 0.00001;

	SECTION("AbsoluteBanzhaf") {
		const auto index = new epic::index::AbsoluteBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.25, 0.25, 0.25};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePowerIndexG") {
		const auto index = new epic::index::AbsolutePowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.625, 0.625, 0.625};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePublicGood") {
		const auto index = new epic::index::AbsolutePublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.571429, 0.571429, 0.571429, 0.571429, 0.571429};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Banzhaf") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.285714, 0.285714, 0.142857, 0.142857, 0.142857};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafBelow") {
		const auto index = new epic::index::BanzhafBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.285714, 0.285714, 0.142857, 0.142857, 0.142857};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafOwen") {
		const auto index = new epic::index::BanzhafOwen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0, 0.5, 0, 0.25, 0.25};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanCollective") {
		const auto index = new epic::index::ColemanCollective();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.5, 0.5, 0.5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanInitiative") {
		const auto index = new epic::index::ColemanInitiative();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.25, 0.25, 0.25};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanPreventive") {
		const auto index = new epic::index::ColemanPreventive();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0.25, 0.25, 0.25};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("DeeganPackel") {
		const auto index = new epic::index::DeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.214286, 0.214286, 0.190476, 0.190476, 0.190476};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("FelsenthalIndex") {
		const auto index = new epic::index::FelsenthalIndex(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.5, 0, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("HarsanyiNashIndex") {
		const auto index = new epic::index::HarsanyiNashIndex();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.2, 0.2, 0.2, 0.2, 0.2};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Johnston") {
		const auto index = new epic::index::Johnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.333333, 0.333333, 0.111111, 0.111111, 0.111111};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuninger") {
		const auto index = new epic::index::KoenigBraeuninger(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.625, 0.625, 0.625};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuningerPH") {
		const auto index = new epic::index::KoenigBraeuningerPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.625, 0.625, 0.625};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Nevison") {
		const auto index = new epic::index::Nevison(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.625, 0.625, 0.625};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("NevisonPH") {
		const auto index = new epic::index::NevisonPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.625, 0.625, 0.625};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Owen") {
		const auto index = new epic::index::Owen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0, 0.444444, 0, 0.277778, 0.277778};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexF") {
		const auto index = new epic::index::PowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.23125, 0.23125, 0.179167, 0.179167, 0.179167};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexG") {
		const auto index = new epic::index::PowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.222222, 0.222222, 0.185185, 0.185185, 0.185185};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexGPH") {
		const auto index = new epic::index::PowerIndexGPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.222222, 0.222222, 0.185185, 0.185185, 0.185185};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicGood") {
		const auto index = new epic::index::PublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.2, 0.2, 0.2, 0.2, 0.2};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpTheta") {
		const auto index = new epic::index::PublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.222222, 0.222222, 0.185185, 0.185185, 0.185185};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpThetaPH") {
		const auto index = new epic::index::PublicHelpThetaPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.222222, 0.222222, 0.185185, 0.185185, 0.185185};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpXi") {
		const auto index = new epic::index::PublicHelpXi(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.243771, 0.243771, 0.170819, 0.170819, 0.170819};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Rae") {
		const auto index = new epic::index::Rae(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.75, 0.625, 0.625, 0.625};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhaf") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {8, 8, 4, 4, 4};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhafBelow") {
		const auto index = new epic::index::RawBanzhafBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {8, 8, 4, 4, 4};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawDeeganPackel") {
		const auto index = new epic::index::RawDeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {1.5, 1.5, 1.33333, 1.33333, 1.33333};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawFelsenthal") {
		const auto index = new epic::index::RawFelsenthal(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {1, 1, 0, 0, 0};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawJohnston") {
		const auto index = new epic::index::RawJohnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4, 4, 1.33333, 1.33333, 1.33333};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexF") {
		const auto index = new epic::index::RawPowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {3.7, 3.7, 2.86667, 2.86667, 2.86667};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexG") {
		const auto index = new epic::index::RawPowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {12, 12, 10, 10, 10};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicGood") {
		const auto index = new epic::index::RawPublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {4, 4, 4, 4, 4};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpTheta") {
		const auto index = new epic::index::RawPublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {12, 12, 10, 10, 10};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpThetaSD") {
		const auto index = new epic::index::RawPublicHelpThetaSD(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {12, 12, 10, 10, 10};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubik") {
		const auto index = new epic::index::RawShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {36, 36, 16, 16, 16};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubikBelow") {
		const auto index = new epic::index::RawShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {36, 36, 16, 16, 16};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubik") {
		const auto index = new epic::index::ShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.3, 0.3, 0.133333, 0.133333, 0.133333};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubikBelow") {
		const auto index = new epic::index::ShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.3, 0.3, 0.133333, 0.133333, 0.133333};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueW") {
		const auto index = new epic::index::SingleValueW();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {16};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWM") {
		const auto index = new epic::index::SingleValueWM();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWS") {
		const auto index = new epic::index::SingleValueWS(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
}

TEST_CASE("TestCase #17: Game: Quota: 10; Weights: 9, 1, 1, 1; Precoalitions = {{0, 1}, {2}, {3}};", "[index]") {
	epic::longUInt quota = 10;
	std::vector<epic::longUInt> weights = {9, 1, 1, 1};
	std::vector<std::vector<int>> precoalitions = {{0, 1}, {2}, {3}};

	const auto game = new epic::Game(quota, weights, false);
	const auto precoalition_game = new epic::PrecoalitionGame(quota, weights, false, precoalitions);
	const auto approx = new epic::FastUpperBoundApproximation(*game);
	epic::bigFloat tolerance = 0.00001;

	SECTION("AbsoluteBanzhaf") {
		const auto index = new epic::index::AbsoluteBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.125, 0.125, 0.125};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePowerIndexG") {
		const auto index = new epic::index::AbsolutePowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {1, 0.571429, 0.571429, 0.571429};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("AbsolutePublicGood") {
		const auto index = new epic::index::AbsolutePublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {1, 0.333333, 0.333333, 0.333333};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Banzhaf") {
		const auto index = new epic::index::Banzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.7, 0.1, 0.1, 0.1};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafBelow") {
		const auto index = new epic::index::BanzhafBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.7, 0.1, 0.1, 0.1};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("BanzhafOwen") {
		const auto index = new epic::index::BanzhafOwen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0, 0, 0.125};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanCollective") {
		const auto index = new epic::index::ColemanCollective();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.4375, 0.4375, 0.4375, 0.4375};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanInitiative") {
		const auto index = new epic::index::ColemanInitiative();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.777778, 0.111111, 0.111111, 0.111111};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ColemanPreventive") {
		const auto index = new epic::index::ColemanPreventive();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {1, 0.142857, 0.142857, 0.142857};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("DeeganPackel") {
		const auto index = new epic::index::DeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.166667, 0.166667, 0.166667};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("FelsenthalIndex") {
		const auto index = new epic::index::FelsenthalIndex(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.166667, 0.166667, 0.166667};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("HarsanyiNashIndex") {
		const auto index = new epic::index::HarsanyiNashIndex();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.25, 0.25, 0.25, 0.25};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Johnston") {
		const auto index = new epic::index::Johnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.785714, 0.0714286, 0.0714286, 0.0714286};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuninger") {
		const auto index = new epic::index::KoenigBraeuninger(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {1, 0.571429, 0.571429, 0.571429};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("KoenigBraeuningerPH") {
		const auto index = new epic::index::KoenigBraeuningerPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {1, 0.571429, 0.571429, 0.571429};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Nevison") {
		const auto index = new epic::index::Nevison(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.5, 0.5, 0.5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("NevisonPH") {
		const auto index = new epic::index::NevisonPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.875, 0.5, 0.5, 0.5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Owen") {
		const auto index = new epic::index::Owen();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(precoalition_game);
		std::vector<epic::bigFloat> expected_result = {0.833333, 0, 0, 0.166667};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexF") {
		const auto index = new epic::index::PowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.392857, 0.202381, 0.202381, 0.202381};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexG") {
		const auto index = new epic::index::PowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.368421, 0.210526, 0.210526, 0.210526};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PowerIndexGPH") {
		const auto index = new epic::index::PowerIndexGPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.368421, 0.210526, 0.210526, 0.210526};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicGood") {
		const auto index = new epic::index::PublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.5, 0.166667, 0.166667, 0.166667};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpTheta") {
		const auto index = new epic::index::PublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.368421, 0.210526, 0.210526, 0.210526};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpThetaPH") {
		const auto index = new epic::index::PublicHelpThetaPH(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.368421, 0.210526, 0.210526, 0.210526};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("PublicHelpXi") {
		const auto index = new epic::index::PublicHelpXi(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.416667, 0.194444, 0.194444, 0.194444};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("Rae") {
		const auto index = new epic::index::Rae(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.9375, 0.5625, 0.5625, 0.5625};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhaf") {
		const auto index = new epic::index::RawBanzhaf();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7, 1, 1, 1};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawBanzhafBelow") {
		const auto index = new epic::index::RawBanzhafBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7, 1, 1, 1};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawDeeganPackel") {
		const auto index = new epic::index::RawDeeganPackel();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {1.5, 0.5, 0.5, 0.5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawFelsenthal") {
		const auto index = new epic::index::RawFelsenthal(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {3, 1, 1, 1};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawJohnston") {
		const auto index = new epic::index::RawJohnston(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {5.5, 0.5, 0.5, 0.5};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexF") {
		const auto index = new epic::index::RawPowerIndexF(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {2.75, 1.41667, 1.41667, 1.41667};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPowerIndexG") {
		const auto index = new epic::index::RawPowerIndexG(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7, 4, 4, 4};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicGood") {
		const auto index = new epic::index::RawPublicGood();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {3, 1, 1, 1};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpTheta") {
		const auto index = new epic::index::RawPublicHelpTheta(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7, 4, 4, 4};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawPublicHelpThetaSD") {
		const auto index = new epic::index::RawPublicHelpThetaSD(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7, 4, 4, 4};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubik") {
		const auto index = new epic::index::RawShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {18, 2, 2, 2};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("RawShapleyShubikBelow") {
		const auto index = new epic::index::RawShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {18, 2, 2, 2};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubik") {
		const auto index = new epic::index::ShapleyShubik();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.0833333, 0.0833333, 0.0833333};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("ShapleyShubikBelow") {
		const auto index = new epic::index::ShapleyShubikBelow();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {0.75, 0.0833333, 0.0833333, 0.0833333};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueW") {
		const auto index = new epic::index::SingleValueW();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {7};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWM") {
		const auto index = new epic::index::SingleValueWM();
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {3};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
	SECTION("SingleValueWS") {
		const auto index = new epic::index::SingleValueWS(game);
		epic::lint::GlobalCalculator::init(epic::lint::CalculatorConfig(index->getMaxValueRequirement(approx),
																		index->getOperationRequirement(),
																		epic::IntRepresentation::DEFAULT));
		auto actual_result = index->calculate(game);
		std::vector<epic::bigFloat> expected_result = {6};
		//REQUIRE(actual_result == expected_result);
		REQUIRE(isResultIdentical(actual_result, expected_result, tolerance));
		epic::lint::GlobalCalculator::remove();
	}
}
