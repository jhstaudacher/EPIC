#include "index/SolidarityPGI.h"
#include "index/PublicGood.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

#include <cmath>

epic::index::SolidarityPGI::SolidarityPGI() : PowerIndexWithPrecoalitions() {}

std::vector<epic::bigFloat> epic::index::SolidarityPGI::calculate(Game* g_) {
	auto g = static_cast<PrecoalitionGame*>(g_);

	std::vector<bigFloat> solution(g->getNumberOfPlayers());
	longUInt quota = g->getQuota();
	
	std::vector<longUInt> preCoalitionWeights = g->getPrecoalitionWeights();
	std::cout << "prec weights" << "\n";
	for (auto it : preCoalitionWeights ){
		std::cout << it << "\n";
	}
	std::cout <<"\n";

	// Create game object from weights of precoalitions with original quota
	auto precoalitionGame = new Game(quota, preCoalitionWeights, false);
	
	std::vector<bigFloat> externalSolution(g->getNumberOfPrecoalitions());
	
	PublicGood* extPGI = new PublicGood();
	
	extPGI->calculate(precoalitionGame, externalSolution);
	std::cout << "externalSolution:" << "\n";
	for (auto it : externalSolution) {
			std::cout << it << "\n";
	}
	std::cout << "\n";
	
	for (longUInt i = 0; i < g->getNumberOfPrecoalitions(); i++) {
		longUInt nbPlayersInParti = g->getPrecoalitions()[i].size();
		for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
			solution[g->getPrecoalitions()[i][ii]] = externalSolution[i]; // /(bigFloat(nbPlayersInParti));
		}
    }
	
	delete precoalitionGame;
	delete extPGI;

	return solution;
}

std::string epic::index::SolidarityPGI::getFullName() {
	return "SolidarityPGI";
}

epic::longUInt epic::index::SolidarityPGI::getMemoryRequirement(Game* g_) {
	auto g = static_cast<PrecoalitionGame*>(g_);

	bigInt memory = (g->getWeightSum() + 1 - g->getQuota()) * gCalculator->getLargeNumberSize() * 4; // c, cw, cw2, cwi -- tba
	memory += g->getMaxPrecoalitionSize() * gCalculator->getLargeNumberSize();											   // banzhafInternal (tba)
	memory += g->getMaxPrecoalitionSize() * c_sizeof_longUInt;															   // winternal
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}
	return ret;
}

epic::bigInt epic::index::SolidarityPGI::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_totalNumberOfSwingPlayer();
}

epic::lint::Operation epic::index::SolidarityPGI::getOperationRequirement() {
	return lint::Operation::multiplication;
}
