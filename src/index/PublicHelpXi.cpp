#include "index/PublicHelpXi.h"

#include "Logging.h"

#include <iostream>

epic::index::PublicHelpXi::PublicHelpXi(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: SwingsPerPlayerAndCardinality(g) {
	if (mGame.getNumberOfNullPlayers() > 0 && mGame.getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::invalidFlagF);
	}

	bigInt max_value = approx->upperBound_totalNumberOfSwingPlayer();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);
}

epic::index::PublicHelpXi::~PublicHelpXi() {
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

std::vector<epic::bigFloat> epic::index::PublicHelpXi::calculate() {
	// helper variables to avoid multiple allocation/deallocation cycles due to frequent use.
	bigInt tmp_bigInt;
	bigFloat tmp_bigFloat;

	// n_wc(x, y) => n_wc[x][y]: number of winning coalitions of weight x and cardinality y
	Array2dOffset<lint::LargeNumber> n_wc(mGame.getWeightSum() + 1, mGame.getNumberOfPlayers() + 1, mGame.getQuota(), 0);
	mCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());
	numberOfWinningCoalitionsPerWeightAndCardinality(n_wc);

	// wci(x, y): number of times player x is a swing player in a coalition of cardinality y (= absolute ShapleyShubik index)
	Array2d<lint::LargeNumber> wci(mGame.getNumberOfPlayers(), mGame.getNumberOfPlayers() + 1);
	mCalculator->allocInit_largeNumberArray(wci.getArrayPointer(), wci.getNumberOfElements());
	swingsPerPlayerAndCardinality(n_wc, wci, false);

	// Update n_wc and wci if there are players of weights 0
	for (longUInt count = 0; count < mGame.getNumberOfPlayersWithWeight0(); ++count) {
		// n_wc:
		for (longUInt weight = mGame.getQuota(); weight <= mGame.getWeightSum(); ++weight) {
			for (longUInt cardinality = mGame.getNumberOfPlayers(); cardinality > 1; --cardinality) {
				mCalculator->plusEqual(n_wc(weight, cardinality), n_wc(weight, cardinality - 1));
				// When a player of weight 0 joins a coalition, the weight stays unchanged while the cardinality increases.
			}
		}

		// wci:
		for (longUInt player = 0; player < mNonZeroPlayerCount; ++player) {
			for (longUInt cardinality = mGame.getNumberOfPlayers(); cardinality > 1; --cardinality) {
				mCalculator->plusEqual(wci(player, cardinality), wci(player, cardinality - 1));
				// If a player is a swing player in a coalitions with (cardinality - 1) players, then he is also a swing player in that coalition if a player of weight zero joins. This new coalition has (cardinality) players.
			}
		}
	}

	/*
	 * initialize the power indices for each player
	 */
	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
		for (longUInt p = 1; p <= mGame.getNumberOfPlayers(); ++p) {
			mCalculator->to_bigInt(&tmp_bigInt, wci(i, p));
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
		mCalculator->alloc_largeNumber(numerator);

		for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) {
			for (longUInt k = mGame.getQuota(); k <= mGame.getWeightSum(); ++k) {
				for (longUInt p = 1; p <= mGame.getNumberOfPlayers(); ++p) {
					mCalculator->plusEqual(wci(i, p), n_wc(k, p));
				}
			}

			mCalculator->assign_zero(numerator);
			for (longUInt p = 2; p <= mGame.getNumberOfPlayers(); ++p) {
				mCalculator->minus(numerator, wci(i, p - 1), numerator);

				mCalculator->to_bigInt(&tmp_bigInt, numerator);
				tmp_bigFloat = (p * p); // TODO: overflow? --> gmp function
				tmp_bigFloat = tmp_bigInt / tmp_bigFloat;
				solution[i] += tmp_bigFloat;
			}
		}

		mCalculator->free_largeNumber(numerator);
	}

	/*
	 * calculate the power index of player of weight greater than 0
	 */
	// siwcc: sum of the multiplicative inverse for the cardinality for each coalition in winning coalitions. Starting with the multiplicative inverse for the cardinality of the grand coalition because it is always winning.
	double siwcc = 0; // 1.0 / mGame.getNumberOfPlayers()
	{
		// wci: number of winning coalitions a certain player belongs to
		lint::LargeNumber n_wc_i;
		mCalculator->alloc_largeNumber(n_wc_i);

		for (longUInt i = 1; i <= mGame.getNumberOfPlayers(); ++i) {
			mCalculator->assign_zero(n_wc_i);

			// summing up the number of winning coalitions player i belongs to
			for (longUInt k = mGame.getQuota(); k <= mGame.getWeightSum(); ++k) {
				mCalculator->plusEqual(n_wc_i, n_wc(k, i));
			}

			mCalculator->to_bigInt(&tmp_bigInt, n_wc_i);
			tmp_bigFloat = i;
			tmp_bigFloat = tmp_bigInt / tmp_bigFloat;
			siwcc += tmp_bigFloat.get_d();
		}

		mCalculator->free_largeNumber(n_wc_i);
	}

	for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
		solution[i] /= siwcc;
	}

	/*
	 * DELETE
	 */
	mCalculator->free_largeNumberArray(n_wc.getArrayPointer());
	mCalculator->free_largeNumberArray(wci.getArrayPointer());

	return solution;
}

std::string epic::index::PublicHelpXi::getFullName() {
	return "PublicHelpXi";
}

epic::longUInt epic::index::PublicHelpXi::getMemoryRequirement() {
	bigInt memory = SwingsPerPlayerAndCardinality::getMemoryRequirement();
	memory += mGame.getNumberOfPlayers() * (mGame.getNumberOfPlayers() + 1) * mCalculator->getLargeNumberSize(); // wci

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}
