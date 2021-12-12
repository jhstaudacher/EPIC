#include "index/RawPowerIndexF.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

epic::index::RawPowerIndexF::RawPowerIndexF(Game* g) : SwingsPerPlayerAndCardinality() {
	if (g->getNumberOfNullPlayers() > 0 && !g->getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::missingFlagF);
	}
}

std::vector<epic::bigFloat> epic::index::RawPowerIndexF::calculate(Game* g) {
	Array2dOffset<lint::LargeNumber> n_wc;
	n_wc.alloc(g->getWeightSum() + 1, g->getNumberOfPlayers() + 1, g->getQuota(), 0);
	gCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());
	numberOfWinningCoalitionsPerWeightAndCardinality(g, n_wc);

	// pif(x, y): PowerIndexF matrix - number of times player x is member in a winning coalition of cardinality y
	Array2d<lint::LargeNumber> pif(g->getNumberOfPlayers(), g->getNumberOfPlayers() + 1);
	gCalculator->allocInit_largeNumberArray(pif.getArrayPointer(), pif.getNumberOfElements());
	swingsPerPlayerAndCardinality(g, n_wc, pif, false);

	// delete n_wc and helper_wc
	gCalculator->free_largeNumberArray(n_wc.getArrayPointer());
	n_wc.free();

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
		}
	}

	// delete pif
	gCalculator->free_largeNumberArray(pif.getArrayPointer());

	return solution;
}

std::string epic::index::RawPowerIndexF::getFullName() {
	return "RawPowerIndexF";
}

epic::longUInt epic::index::RawPowerIndexF::getMemoryRequirement(Game* g) {
	bigInt memory = g->getNumberOfPlayers();
	memory *= (g->getNumberOfPlayers() + 1) * gCalculator->getLargeNumberSize(); // pif
	memory /= cMemUnit_factor;

	memory += SwingsPerPlayerAndCardinality::getMemoryRequirement(g);

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

epic::bigInt epic::index::RawPowerIndexF::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_numberOfWinningCoalitionsPerWeight();
}

epic::lint::Operation epic::index::RawPowerIndexF::getOperationRequirement() {
	return lint::Operation::addition;
}
