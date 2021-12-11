#include "index/FelsenthalIndex.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

epic::index::FelsenthalIndex::FelsenthalIndex(Game& g) : RawFelsenthal(g) {}

std::vector<epic::bigFloat> epic::index::FelsenthalIndex::calculate(Game& g) {
	// if a veto player exists, that player gets assigned 1 while all other players get assigned 0
	// note: the weights have to be sorted decreasingly
	if (g.getWeights()[0] >= g.getQuota()) {
		std::vector<bigFloat> solution(g.getNumberOfPlayers());
		{
			solution[0] = 1.0;

			for (longUInt i = 1; i < g.getNumberOfPlayers(); ++i) {
				solution[i] = 0.0;
			}
		}
		return solution;
	}
	// mwcs: number of minimal winning coalitions for each player i
	auto mwcs = new lint::LargeNumber[g.getNumberOfNonZeroPlayers()];
	gCalculator->allocInit_largeNumberArray(mwcs, g.getNumberOfNonZeroPlayers());
	minimal_winning_coalitions_of_least_size(g, mwcs);

	// sum_mwcs: number of all minimal winning coalitions
	lint::LargeNumber sum_mwcs;
	gCalculator->allocInit_largeNumber(sum_mwcs);

	for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) {
		gCalculator->plusEqual(sum_mwcs, mwcs[i]);
	}

	std::vector<bigFloat> solution(g.getNumberOfPlayers());
	{
		bigInt int_tmp;
		bigFloat arbitrary_sum_mwcs;

		gCalculator->to_bigInt(&int_tmp, sum_mwcs);
		arbitrary_sum_mwcs = int_tmp;

		// compute Felsenthal-index: Number of minimal winning coalitions of player i / (SUM(Number of minimal winning coalitions of all players))
		if (arbitrary_sum_mwcs == 0) {
			log::out << log::warning << "sum of minimal winning coalitions of least size is zero!" << log::endl;
		} else {
			log::out << log::info << "Total number of minimal winning coalitions of least size: " << arbitrary_sum_mwcs << log::endl;
			log::out << log::info << "Number of minimal winning coalitions of least size individual players belong to: " << log::endl;
			for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) {
				gCalculator->to_bigInt(&int_tmp, mwcs[i]);
				log::out << "Player " << g.playerIndexToNumber(i) << ": " << int_tmp << log::endl;
				solution[i] = (bigFloat)int_tmp / arbitrary_sum_mwcs;
			}

			for (longUInt i = g.getNumberOfNonZeroPlayers(); i < g.getNumberOfPlayers(); ++i) {
				solution[i] = 0;
			}
		}
	}

	/*
	 * DELETE
	 */
	gCalculator->free_largeNumber(sum_mwcs);
	gCalculator->free_largeNumberArray(mwcs);
	delete[] mwcs;

	return solution;
}

std::string epic::index::FelsenthalIndex::getFullName() {
	return "Felsenthal Index";
}

epic::longUInt epic::index::FelsenthalIndex::getMemoryRequirement(Game& g) {
	bigInt memory = gCalculator->getLargeNumberSize(); // sum_mwcs
	memory /= cMemUnit_factor;

	memory += RawFelsenthal::getMemoryRequirement(g);

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}
