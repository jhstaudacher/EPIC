#include "index/Tijs.h"

#include "Logging.h"
#include "types.h"

epic::index::Tijs::Tijs()
	: ItfPowerIndex() {}

std::vector<epic::bigFloat> epic::index::Tijs::calculate(Game* g) {
	std::vector<bigFloat> solution(g->getNumberOfPlayers());

	// n_vp: the number of veto players
	bigFloat n_vp = g->getNumberOfVetoPlayers();

	if (n_vp == 0) {
		throw std::invalid_argument("Tijs index not defined: There are no veto players.");
	} else {
		log::out << log::info << "Total number of veto players: " << n_vp << log::endl;

		for (longUInt i = 0; i < g->getNumberOfPlayers(); ++i) {
			if (g->getVetoPlayerVector()[i]) {
				solution[i] = 1 / n_vp;
			} else {
				solution[i] = 0;
			}
		}
	}

	return solution;
}

std::string epic::index::Tijs::getFullName() {
	return "Tijs";
}

epic::longUInt epic::index::Tijs::getMemoryRequirement(Game* g) {
	bigFloat n_vp = g->getNumberOfPlayers();
	n_vp = 1 / n_vp;
	bigInt memory = g->getNumberOfPlayers() * GMPHelper::size_of_float(n_vp); // solution
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

epic::bigInt epic::index::Tijs::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return 0;
}

epic::lint::Operation epic::index::Tijs::getOperationRequirement() {
	return lint::Operation::addition; // does not need any
}
