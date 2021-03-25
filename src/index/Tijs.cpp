#include "index/Tijs.h"

#include "Logging.h"
#include "types.h"

epic::index::Tijs::Tijs(Game& g)
	: ItfPowerIndex(g) {
	mCalculator = nullptr; // don't need a lint::ChineseNumber variable
}

std::vector<epic::bigFloat> epic::index::Tijs::calculate() {
	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());

	// n_vp: the number of veto players
	bigFloat n_vp = mGame.getNumberOfVetoPlayers();

	if (n_vp == 0) {
		throw std::invalid_argument("Tijs index not defined: There are no veto players.");
	} else {
		log::out << log::info << "Total number of veto players: " << n_vp << log::endl;

		for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
			if (mGame.getVetoPlayerVector()[i]) {
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

epic::longUInt epic::index::Tijs::getMemoryRequirement() {
	bigFloat n_vp = mGame.getNumberOfPlayers();
	n_vp = 1 / n_vp;
	bigInt memory = mGame.getNumberOfPlayers() * GMPHelper::size_of_float(n_vp); // solution

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}
