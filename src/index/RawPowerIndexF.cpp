#include "index/RawPowerIndexF.h"

#include "Logging.h"

epic::index::RawPowerIndexF::RawPowerIndexF(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: SwingsPerPlayerAndCardinality(g) {
	if (mGame.getNumberOfNullPlayers() > 0 && !mGame.getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::missingFlagF);
	}

	bigInt max_value = approx->upperBound_numberOfWinningCoalitionsPerWeight();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);
}

epic::index::RawPowerIndexF::~RawPowerIndexF() {
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

std::vector<epic::bigFloat> epic::index::RawPowerIndexF::calculate() {
	Array2dOffset<lint::LargeNumber> n_wc;
	n_wc.alloc(mGame.getWeightSum() + 1, mGame.getNumberOfPlayers() + 1, mGame.getQuota(), 0);
	mCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());
	numberOfWinningCoalitionsPerWeightAndCardinality(n_wc);

	// pif(x, y): PowerIndexF matrix - number of times player x is member in a winning coalition of cardinality y
	Array2d<lint::LargeNumber> pif(mGame.getNumberOfPlayers(), mGame.getNumberOfPlayers() + 1);
	mCalculator->allocInit_largeNumberArray(pif.getArrayPointer(), pif.getNumberOfElements());
	swingsPerPlayerAndCardinality(n_wc, pif, false);

	// delete n_wc and helper_wc
	mCalculator->free_largeNumberArray(n_wc.getArrayPointer());
	n_wc.free();

	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	{
		bigInt tmp_int;
		bigFloat tmp_float;
		bigFloat big_pif;

		for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
			big_pif = 0;

			for (longUInt k = 1; k <= mGame.getNumberOfPlayers(); ++k) {
				mCalculator->to_bigInt(&tmp_int, pif(i, k));
				tmp_float = tmp_int;
				big_pif += tmp_float / static_cast<double>(k);
			}

			solution[i] = big_pif;
		}
	}

	// delete pif
	mCalculator->free_largeNumberArray(pif.getArrayPointer());

	return solution;
}

std::string epic::index::RawPowerIndexF::getFullName() {
	return "RawPowerIndexF";
}

epic::longUInt epic::index::RawPowerIndexF::getMemoryRequirement() {
	bigInt memory = mGame.getNumberOfPlayers();
	memory *= (mGame.getNumberOfPlayers() + 1) * mCalculator->getLargeNumberSize(); // pif
	memory /= cMemUnit_factor;

	memory += SwingsPerPlayerAndCardinality::getMemoryRequirement();

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}
