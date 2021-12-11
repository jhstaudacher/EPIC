#include "index/ColemanCollective.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

epic::index::ColemanCollective::ColemanCollective() : PowerIndexWithWinningCoalitions() {}

std::vector<epic::bigFloat> epic::index::ColemanCollective::calculate(Game& g) {
	// n_wc[x]: number of winning coalitions of weight x.
	ArrayOffset<lint::LargeNumber> n_wc(g.getWeightSum() + 1, g.getQuota());
	gCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());
	numberOfWinningCoalitionsPerWeight(g, n_wc);

	// total_wc: number of winning coalitions (summing up)
	lint::LargeNumber total_wc;
	gCalculator->allocInit_largeNumber(total_wc);
	numberOfWinningCoalitions(g, n_wc, total_wc);

	//delete n_wc[]
	gCalculator->free_largeNumberArray(n_wc.getArrayPointer());

	std::vector<bigFloat> solution(g.getNumberOfPlayers());
	{
		bigInt big_total_wc;
		gCalculator->to_bigInt(&big_total_wc, total_wc);

		log::out << log::info << "Total number of winning coalitions: " << big_total_wc * (bigInt(1) << g.getNumberOfPlayersWithWeight0()) << log::endl;

		//total number of winning coalitions / maximal number of winning coalitions(= 2^g.getNumberOfNonZeroPlayers())
		bigFloat big_total_wc_float = big_total_wc;
		bigInt maximum_big_total_wc = bigInt(1) << g.getNumberOfNonZeroPlayers(); // = 2^g.getNumberOfNonZeroPlayers()
		big_total_wc_float /= maximum_big_total_wc;

		for (longUInt i = 0; i < g.getNumberOfPlayers(); ++i) {
			solution[i] = big_total_wc_float;
		}
	}

	gCalculator->free_largeNumber(total_wc);
	return solution;
}

std::string epic::index::ColemanCollective::getFullName() {
	return "ColemanCollective";
}

epic::longUInt epic::index::ColemanCollective::getMemoryRequirement(Game& g) {
	bigInt memory = gCalculator->getLargeNumberSize();
	memory *= g.getWeightSum() + 1 - g.getQuota(); // n_wc
	memory += gCalculator->getLargeNumberSize();		   // total_wc
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

epic::bigInt epic::index::ColemanCollective::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_numberOfWinningCoalitionsPerWeight();
}

epic::lint::Operation epic::index::ColemanCollective::getOperationRequirement() {
	return lint::Operation::addition;
}
