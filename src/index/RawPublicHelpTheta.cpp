#include "index/RawPublicHelpTheta.h"

#include "Logging.h"

epic::longUInt epic::index::RawPublicHelpTheta::getMemoryRequirement() {
	bigInt memory = (mGame.getWeightSum() + 1 - mGame.getQuota()) * mCalculator->getLargeNumberSize() * 2;	// n_wc, helper
	memory += mGame.getNumberOfPlayers() * mCalculator->getLargeNumberSize();								// wci
	memory += GMPHelper::size_of_int(bigInt(1) << mGame.getNumberOfPlayers()) * mGame.getNumberOfPlayers(); // big_wci

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

epic::index::RawPublicHelpTheta::RawPublicHelpTheta(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: PowerIndexWithWinningCoalitions(g) {
	if (mGame.getNumberOfNullPlayers() > 0 && mGame.getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::invalidFlagF);
	}

	bigInt max_value = approx->upperBound_numberOfWinningCoalitionsPerWeight();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);
}

epic::index::RawPublicHelpTheta::~RawPublicHelpTheta() {
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

std::vector<epic::bigFloat> epic::index::RawPublicHelpTheta::calculate() {
	auto big_wci = new bigInt[mGame.getNumberOfPlayers()];

	winningCoalitionsForPlayer(big_wci);
	bigInt factor = bigInt(1) << mGame.getNumberOfPlayersWithWeight0(); // additional winning coalitions due to players of weight 0

	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	for (longUInt i = 0; i < mGame.getNumberOfPlayers(); i++) {
		solution[i] = big_wci[i] * factor;
	}

	delete[] big_wci;
	return solution;
}

std::string epic::index::RawPublicHelpTheta::getFullName() {
	return "Absolute Public Help Theta";
}

void epic::index::RawPublicHelpTheta::winningCoalitionsForPlayer(bigInt big_wci[], bigFloat* big_total_wc) {
	// n_wc[x]: number of winning coalitions of weight x.
	ArrayOffset<lint::LargeNumber> n_wc(mGame.getWeightSum() + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());

	// helper: helper array for n_wc
	ArrayOffset<lint::LargeNumber> helper(mGame.getWeightSum() + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(helper.getArrayPointer(), helper.getNumberOfElements());

	numberOfWinningCoalitionsPerWeight(n_wc);

	// wci[x]: number of winning coalitions for player x
	auto wci = new lint::LargeNumber[mGame.getNumberOfPlayers()];

	mCalculator->allocInit_largeNumberArray(wci, mGame.getNumberOfPlayers());

	/*
	 * Handle all players of weight > 0
	 */

	// This algorithm is nearly the same as it is used in the PowerIndexWithWinningCoalitionsAndSwingPlayers::numberOfTimesPlayerIsSwingPlayer() function
	for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
		longUInt wi = mGame.getWeights()[i];

		// skip dummy players if they are present in the array (!mGame.getFlagNullPlayerHandling())
		if ((i >= mGame.getNumberOfPlayers() - mGame.getNumberOfNullPlayers()) && mGame.getFlagNullPlayerHandling()) {
			continue;
		}

		longUInt m = std::max(mGame.getWeightSum() - wi, mGame.getQuota() - 1);
		for (longUInt k = mGame.getWeightSum(); k > m; --k) {
			mCalculator->assign(helper[k], n_wc[k]);
		}

		for (longUInt k = mGame.getWeightSum() - wi; k >= mGame.getQuota(); --k) {
			mCalculator->minus(helper[k], n_wc[k], helper[k + wi]);
		}

		// this is the only difference to the previously mentioned algorithm.
		// instead of looping over all coalitions the player would turn into losing ones, we step over all winning coalitions the player is a member of (up to weightsum).
		for (longUInt k = mGame.getQuota(); k <= mGame.getWeightSum(); ++k) {
			mCalculator->plusEqual(wci[i], helper[k]);
		}

		mCalculator->to_bigInt(&big_wci[i], wci[i]);
	}

	mCalculator->free_largeNumberArray(wci);
	delete[] wci;

	// If total_wc is wished for, provide it, otherwise delete it
	bool delete_total_wc;
	if (big_total_wc == nullptr) {
		delete_total_wc = true;
		big_total_wc = new bigFloat;
	} else {
		delete_total_wc = false;
	}

	// initialize total_wc
	// total_wc: the total number of winning coalitions (sum over all n_wc)
	lint::LargeNumber total_wc;
	mCalculator->allocInit_largeNumber(total_wc);

	numberOfWinningCoalitions(n_wc, total_wc);

	bigInt tmp;
	mCalculator->to_bigInt(&tmp, total_wc);
	*big_total_wc = tmp;

	mCalculator->free_largeNumber(total_wc);
	mCalculator->free_largeNumberArray(n_wc.getArrayPointer());
	mCalculator->free_largeNumberArray(helper.getArrayPointer());

	// Handle null/dummy players
	// If there are null players, naturally they start at index mGame.getNumberOfPlayers() - mGame.getNumberOfNullPlayers()
	for (longUInt i = mGame.getNumberOfPlayers() - mGame.getNumberOfNullPlayers(); i < mGame.getNumberOfPlayers(); ++i) {
		big_wci[i] = *big_total_wc / 2;
	}

	if (delete_total_wc) {
		delete big_total_wc;
	}
}
