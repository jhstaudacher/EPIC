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
