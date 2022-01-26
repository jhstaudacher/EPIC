#include "index/PublicHelpXi.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

#include <iostream>

epic::index::PublicHelpXi::PublicHelpXi(Game* g)
	: SwingsPerPlayerAndCardinality() {
	if (g->getNumberOfNullPlayers() > 0 && g->getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::invalidFlagF);
	}
}

std::vector<epic::bigFloat> epic::index::PublicHelpXi::calculate(Game* g) {
	// helper variables to avoid multiple allocation/deallocation cycles due to frequent use.
	bigInt tmp_bigInt;
	bigFloat tmp_bigFloat;

	// n_wc(x, y) => n_wc[x][y]: number of winning coalitions of weight x and cardinality y
	Array2dOffset<lint::LargeNumber> n_wc(g->getWeightSum() + 1, g->getNumberOfPlayers() + 1, g->getQuota(), 0);
	gCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());
	numberOfWinningCoalitionsPerWeightAndCardinality(g, n_wc);

	// wci(x, y): number of times player x is a swing player in a coalition of cardinality y (= absolute ShapleyShubik index)
	Array2d<lint::LargeNumber> wci(g->getNumberOfPlayers(), g->getNumberOfPlayers() + 1);
	gCalculator->allocInit_largeNumberArray(wci.getArrayPointer(), wci.getNumberOfElements());
	swingsPerPlayerAndCardinality(g, n_wc, wci, false);

	// Update n_wc and wci if there are players of weights 0
	for (longUInt count = 0; count < g->getNumberOfPlayersWithWeight0(); ++count) {
		// n_wc:
		for (longUInt weight = g->getQuota(); weight <= g->getWeightSum(); ++weight) {
			for (longUInt cardinality = g->getNumberOfPlayers(); cardinality > 1; --cardinality) {
				gCalculator->plusEqual(n_wc(weight, cardinality), n_wc(weight, cardinality - 1));
				// When a player of weight 0 joins a coalition, the weight stays unchanged while the cardinality increases.
			}
		}

		// wci:
		for (longUInt player = 0; player < g->getNumberOfNonZeroPlayers(); ++player) {
			for (longUInt cardinality = g->getNumberOfPlayers(); cardinality > 1; --cardinality) {
				gCalculator->plusEqual(wci(player, cardinality), wci(player, cardinality - 1));
				// If a player is a swing player in a coalitions with (cardinality - 1) players, then he is also a swing player in that coalition if a player of weight zero joins. This new coalition has (cardinality) players.
			}
		}
	}

	/*
	 * initialize the power indices for each player
	 */
	std::vector<bigFloat> solution(g->getNumberOfPlayers());
	for (longUInt i = 0; i < g->getNumberOfPlayers(); ++i) {
		for (longUInt p = 1; p <= g->getNumberOfPlayers(); ++p) {
			gCalculator->to_bigInt(&tmp_bigInt, wci(i, p));
			tmp_bigFloat = (p * p); // TODO: overflow ??? -> gmp function
			tmp_bigFloat = tmp_bigInt / tmp_bigFloat;
			solution[i] += tmp_bigFloat;
		}
	}

	/*
	 * calculate the power index of player of weight 0
	 */
	{
		lint::LargeNumber numerator;
		gCalculator->alloc_largeNumber(numerator);

		for (longUInt i = g->getNumberOfNonZeroPlayers(); i < g->getNumberOfPlayers(); ++i) {
			for (longUInt k = g->getQuota(); k <= g->getWeightSum(); ++k) {
				for (longUInt p = 1; p <= g->getNumberOfPlayers(); ++p) {
					gCalculator->plusEqual(wci(i, p), n_wc(k, p));
				}
			}

			gCalculator->assign_zero(numerator);
			for (longUInt p = 2; p <= g->getNumberOfPlayers(); ++p) {
				gCalculator->minus(numerator, wci(i, p - 1), numerator);

				gCalculator->to_bigInt(&tmp_bigInt, numerator);
				tmp_bigFloat = (p * p); // TODO: overflow? --> gmp function
				tmp_bigFloat = tmp_bigInt / tmp_bigFloat;
				solution[i] += tmp_bigFloat;
			}
		}

		gCalculator->free_largeNumber(numerator);
	}

	/*
	 * calculate the power index of player of weight greater than 0
	 */
	// siwcc: sum of the multiplicative inverse for the cardinality for each coalition in winning coalitions. Starting with the multiplicative inverse for the cardinality of the grand coalition because it is always winning.
	double siwcc = 0; // 1.0 / g->getNumberOfPlayers()
	{
		// wci: number of winning coalitions a certain player belongs to
		lint::LargeNumber n_wc_i;
		gCalculator->alloc_largeNumber(n_wc_i);

		for (longUInt i = 1; i <= g->getNumberOfPlayers(); ++i) {
			gCalculator->assign_zero(n_wc_i);

			// summing up the number of winning coalitions player i belongs to
			for (longUInt k = g->getQuota(); k <= g->getWeightSum(); ++k) {
				gCalculator->plusEqual(n_wc_i, n_wc(k, i));
			}

			gCalculator->to_bigInt(&tmp_bigInt, n_wc_i);
			tmp_bigFloat = i;
			tmp_bigFloat = tmp_bigInt / tmp_bigFloat;
			siwcc += tmp_bigFloat.get_d();
		}

		gCalculator->free_largeNumber(n_wc_i);
	}

	for (longUInt i = 0; i < g->getNumberOfPlayers(); ++i) {
		solution[i] /= siwcc;
	}

	/*
	 * DELETE
	 */
	gCalculator->free_largeNumberArray(n_wc.getArrayPointer());
	gCalculator->free_largeNumberArray(wci.getArrayPointer());

	return solution;
}

std::string epic::index::PublicHelpXi::getFullName() {
	return "PublicHelpXi";
}

epic::longUInt epic::index::PublicHelpXi::getMemoryRequirement(Game* g) {
	bigInt memory = g->getNumberOfPlayers() * (g->getNumberOfPlayers() + 1) * gCalculator->getLargeNumberSize(); // wci
	memory /= cMemUnit_factor;

	memory += SwingsPerPlayerAndCardinality::getMemoryRequirement(g);

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

epic::bigInt epic::index::PublicHelpXi::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_totalNumberOfSwingPlayer();
}

epic::lint::Operation epic::index::PublicHelpXi::getOperationRequirement() {
	return lint::Operation::addition;
}
