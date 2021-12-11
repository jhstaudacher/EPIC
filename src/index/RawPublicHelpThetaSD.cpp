#include "index/RawPublicHelpThetaSD.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

epic::index::RawPublicHelpThetaSD::RawPublicHelpThetaSD(Game& g) : RawBanzhaf() {
	if (g.getNumberOfNullPlayers() > 0 && g.getFlagNullPlayerHandling()) {
		throw std::invalid_argument(log::invalidFlagF);
	}
}

std::vector<epic::bigFloat> epic::index::RawPublicHelpThetaSD::calculate(Game& g) {
	auto big_wci = new bigInt[g.getNumberOfPlayers()];

	winningCoalitionsForPlayer(g, big_wci);

	bigInt factor = bigInt(1) << g.getNumberOfPlayersWithWeight0(); // additional winning coalitions due to players of weight 0
	std::vector<epic::bigFloat> solution(g.getNumberOfPlayers());
	for (longUInt i = 0; i < g.getNumberOfPlayers(); i++) {
		solution[i] = big_wci[i] * factor;
	}

	delete[] big_wci;
	return solution;
}

std::string epic::index::RawPublicHelpThetaSD::getFullName() {
	return "RawPublicHelpTheta (with Dubey-Shapley identity)";
}

void epic::index::RawPublicHelpThetaSD::winningCoalitionsForPlayer(Game& g, bigInt big_wci[]) {
	// n_sp[x]: number of times player x is a swing player.
	auto n_sp = new lint::LargeNumber[g.getNumberOfPlayers()];
	gCalculator->allocInit_largeNumberArray(n_sp, g.getNumberOfPlayers());

	numberOfTimesPlayerIsSwingPlayer(g, n_sp);

	// n_wc[x]: number of winning coalitions of weight x.
	ArrayOffset<lint::LargeNumber> n_wc(g.getWeightSum() + 1, g.getQuota());
	gCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());

	numberOfWinningCoalitionsPerWeight(g, n_wc);

	// total_wc: the total number of winning coalitions.
	lint::LargeNumber total_wc;
	gCalculator->allocInit_largeNumber(total_wc);

	numberOfWinningCoalitions(g, n_wc, total_wc);

	// delete n_wc[]
	gCalculator->free_largeNumberArray(n_wc.getArrayPointer());

	bigInt tmp;

	gCalculator->to_bigInt(&tmp, total_wc);
	bigFloat float_total_wc = tmp;

	for (longUInt i = 0; i < g.getNumberOfPlayers(); i++) {
		gCalculator->to_bigInt(&tmp, n_sp[i]);

		big_wci[i] = double(0.5) * (tmp + float_total_wc);
	}

	gCalculator->free_largeNumber(total_wc);
	// delete n_sp[]
	gCalculator->free_largeNumberArray(n_sp);
	delete[] n_sp;
}
