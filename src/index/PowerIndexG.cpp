#include "index/PowerIndexG.h"

#include "Array.h"
#include "Logging.h"

epic::index::PowerIndexG::PowerIndexG(Game* g)
	: RawPowerIndexG(g) {}

std::vector<epic::bigFloat> epic::index::PowerIndexG::calculate(Game* g) {
	auto solution = RawPowerIndexG::calculate(g);

	bigFloat sum_swc = 0;
	for (const auto& it : solution) {
		sum_swc += it;
	}

	if (log::out.getLogLevel() <= log::info) {
		log::out << log::info << "Number of winning coalitions a player belongs to: " << log::endl;
		for (longUInt i = 0; i < g->getNumberOfPlayers(); ++i) {
			log::out << "Player " << g->playerIndexToNumber(i) << ": " << solution[i] << log::endl;
		}
	}

	for (auto& it : solution) {
		it /= sum_swc;
	}

	return solution;
}

std::string epic::index::PowerIndexG::getFullName() {
	return "PowerIndexG";
}
