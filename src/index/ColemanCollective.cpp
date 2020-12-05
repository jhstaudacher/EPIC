#include "index/ColemanCollective.h"

#include <iostream>

epic::index::ColemanCollective::ColemanCollective(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: PowerIndexWithWinningCoalitions(g) {
	bigInt max_value = approx->upperBound_numberOfWinningCoalitionsPerWeight();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);
}

epic::index::ColemanCollective::~ColemanCollective() {
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

std::vector<epic::bigFloat> epic::index::ColemanCollective::calculate() {
	// n_wc[x]: number of winning coalitions of weight x.
	ArrayOffset<lint::LargeNumber> n_wc(mGame.getWeightSum() + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());
	numberOfWinningCoalitionsPerWeight(n_wc);

	// total_wc: number of winning coalitions (summing up)
	lint::LargeNumber total_wc;
	mCalculator->allocInit_largeNumber(total_wc);
	numberOfWinningCoalitions(n_wc, total_wc);

	//delete n_wc[]
	mCalculator->free_largeNumberArray(n_wc.getArrayPointer());

	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	{
		bigInt big_total_wc;
		mCalculator->to_bigInt(&big_total_wc, total_wc);

		if (mGame.getFlagOfVerbose()) {
			std::cout << "Total number of winning coalitions: " << big_total_wc * (bigInt(1) << mGame.getNumberOfPlayersWithWeight0()) << std::endl
					  << std::endl;
		}

		//total number of winning coalitions / maximal number of winning coalitions(= 2^mNonZeroPlayerCount)
		bigFloat big_total_wc_float = big_total_wc;
		bigInt maximum_big_total_wc = bigInt(1) << mNonZeroPlayerCount; // = 2^mNonZeroPlayerCount
		big_total_wc_float /= maximum_big_total_wc;

		for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
			solution[i] = big_total_wc_float;
		}
	}

	mCalculator->free_largeNumber(total_wc);
	return solution;
}

std::string epic::index::ColemanCollective::getFullName() {
	return "ColemanCollective";
}

epic::longUInt epic::index::ColemanCollective::getMemoryRequirement() {
	bigInt memory = (mGame.getWeightSum() + 1 - mGame.getQuota()) * mCalculator->getLargeNumberSize(); // n_wc
	memory += mCalculator->getLargeNumberSize();													   // total_wc

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}
