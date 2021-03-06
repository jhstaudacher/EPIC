#include "index/PowerIndexF.h"

#include "Logging.h"

epic::index::PowerIndexF::PowerIndexF(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: SwingsPerPlayerAndCardinality(g) {
	if (mGame.getNumberOfNullPlayers() > 0 && !mGame.getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::missingFlagF);
	}

	bigInt max_value = approx->upperBound_numberOfWinningCoalitionsPerWeight();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);
}

epic::index::PowerIndexF::~PowerIndexF() {
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

std::vector<epic::bigFloat> epic::index::PowerIndexF::calculate() {
	Array2dOffset<lint::LargeNumber> n_wc;
	n_wc.alloc(mGame.getWeightSum() + 1, mGame.getNumberOfPlayers() + 1, mGame.getQuota(), 0);
	mCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());
	numberOfWinningCoalitionsPerWeightAndCardinality(n_wc);

	// pif(x, y): PowerIndexF matrix - number of times player x is member in a winning coalition of cardinality y
	Array2d<lint::LargeNumber> pif(mGame.getNumberOfPlayers(), mGame.getNumberOfPlayers() + 1);
	mCalculator->allocInit_largeNumberArray(pif.getArrayPointer(), pif.getNumberOfElements());
	swingsPerPlayerAndCardinality(n_wc, pif, false);

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
			solution[i] /= total_number_of_winning_coalitions;
		}
	}
	
	// delete pif
	mCalculator->free_largeNumberArray(pif.getArrayPointer());

	return solution;
}

std::string epic::index::PowerIndexF::getFullName() {
	return "PowerIndexF";
}

epic::longUInt epic::index::PowerIndexF::getMemoryRequirement() {
	bigInt memory = SwingsPerPlayerAndCardinality::getMemoryRequirement();
	memory += mGame.getNumberOfPlayers() * (mGame.getNumberOfPlayers() + 1) * mCalculator->getLargeNumberSize(); // pif

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}
