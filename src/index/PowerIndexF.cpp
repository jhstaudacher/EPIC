#include "index/PowerIndexF.h"

#include "Logging.h"

epic::index::PowerIndexF::PowerIndexF(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawPowerIndexF(g, approx, int_representation) {
}

std::vector<epic::bigFloat> epic::index::PowerIndexF::calculate() {
	Array2dOffset<lint::LargeNumber> n_wc;
	n_wc.alloc(mGame.getWeightSum() + 1, mNonZeroPlayerCount + 1, mGame.getQuota(), 0);
	mCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());
	numberOfWinningCoalitionsPerWeightAndCardinality(n_wc);

	bigInt total_number_of_winning_coalitions;
	{
		bigInt temp;

		for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
			for (longUInt y = 0; y <= mNonZeroPlayerCount; ++y) {
				mCalculator->to_bigInt(&temp, n_wc(x, y));
				total_number_of_winning_coalitions += temp;
			}
		}
	}

	// delete n_wc and helper_wc
	mCalculator->free_largeNumberArray(n_wc.getArrayPointer());
	n_wc.free();

	log::out << log::info << "Total number of winning coalitions: " << total_number_of_winning_coalitions << log::endl;

	std::vector<bigFloat> solution = RawPowerIndexF::calculate();

	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
		solution[i] /= total_number_of_winning_coalitions;
	}

	return solution;
}

std::string epic::index::PowerIndexF::getFullName() {
	return "PowerIndexF";
}
