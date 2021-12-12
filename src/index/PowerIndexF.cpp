#include "index/PowerIndexF.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

epic::index::PowerIndexF::PowerIndexF(Game* g) : SwingsPerPlayerAndCardinality() {
	if (g->getNumberOfNullPlayers() > 0 && !g->getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::missingFlagF);
	}
}

std::vector<epic::bigFloat> epic::index::PowerIndexF::calculate(Game* g) {
	Array2dOffset<lint::LargeNumber> n_wc;
	n_wc.alloc(g->getWeightSum() + 1, g->getNumberOfPlayers() + 1, g->getQuota(), 0);
	gCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());
	numberOfWinningCoalitionsPerWeightAndCardinality(g, n_wc);

	// pif(x, y): PowerIndexF matrix - number of times player x is member in a winning coalition of cardinality y
	Array2d<lint::LargeNumber> pif(g->getNumberOfPlayers(), g->getNumberOfPlayers() + 1);
	gCalculator->allocInit_largeNumberArray(pif.getArrayPointer(), pif.getNumberOfElements());
	swingsPerPlayerAndCardinality(g, n_wc, pif, false);

	bigInt total_number_of_winning_coalitions;
	{
		bigInt temp;

		for (longUInt x = g->getQuota(); x <= g->getWeightSum(); ++x) {
			for (longUInt y = 0; y <= g->getNumberOfNonZeroPlayers(); ++y) {
				gCalculator->to_bigInt(&temp, n_wc(x, y));
				total_number_of_winning_coalitions += temp;
			}
		}
	}

	// delete n_wc and helper_wc
	gCalculator->free_largeNumberArray(n_wc.getArrayPointer());
	n_wc.free();

	log::out << log::info << "Total number of winning coalitions: " << total_number_of_winning_coalitions << log::endl;

	std::vector<bigFloat> solution(g->getNumberOfPlayers());
	{
		bigInt tmp_int;
		bigFloat tmp_float;
		bigFloat big_pif;

		for (longUInt i = 0; i < g->getNumberOfPlayers(); ++i) {
			big_pif = 0;

			for (longUInt k = 1; k <= g->getNumberOfPlayers(); ++k) {
				gCalculator->to_bigInt(&tmp_int, pif(i, k));
				tmp_float = tmp_int;
				big_pif += tmp_float / static_cast<double>(k);
			}

			solution[i] = big_pif;
			solution[i] /= total_number_of_winning_coalitions;
		}
	}

	// delete pif
	gCalculator->free_largeNumberArray(pif.getArrayPointer());

	return solution;
}

std::string epic::index::PowerIndexF::getFullName() {
	return "PowerIndexF";
}

epic::longUInt epic::index::PowerIndexF::getMemoryRequirement(Game* g) {
	bigInt memory = g->getNumberOfPlayers() * (g->getNumberOfPlayers() + 1) * gCalculator->getLargeNumberSize(); // pif
	memory /= cMemUnit_factor;

	memory += SwingsPerPlayerAndCardinality::getMemoryRequirement(g);

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

epic::bigInt epic::index::PowerIndexF::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_numberOfWinningCoalitionsPerWeight();
}

epic::lint::Operation epic::index::PowerIndexF::getOperationRequirement() {
	return lint::Operation::addition;
}
