#include "index/SolidarityPGI.h"

#include "Logging.h"
#include "index/PublicGood.h"
#include "lint/GlobalCalculator.h"


epic::index::SolidarityPGI::SolidarityPGI() : PowerIndexWithPrecoalitions() {}

std::vector<epic::bigFloat> epic::index::SolidarityPGI::calculate(Game* g_) {
	auto g = static_cast<PrecoalitionGame*>(g_);
	std::vector<bigFloat> solution(g->getNumberOfPlayers());

	std::vector<bigFloat> externalSolution(g->getNumberOfPrecoalitions());
	{ // calculate external game
		PublicGood* pgi = new PublicGood();
		// Create game object from weights of precoalitions with original quota
		auto precoalitionGame = new Game(g->getQuota(), g->getPrecoalitionWeights(), false);
		pgi->calculate(precoalitionGame, externalSolution);

		delete precoalitionGame;
		delete pgi;
	}

	for (longUInt i = 0; i < g->getNumberOfPrecoalitions(); i++) {
		longUInt nbPlayersInParti = g->getPrecoalitions()[i].size();

		for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
			solution[g->getPrecoalitions()[i][ii]] = externalSolution[i] / nbPlayersInParti;
		}
	}

	return solution;
}

std::string epic::index::SolidarityPGI::getFullName() {
	return "SolidarityPGI";
}

epic::longUInt epic::index::SolidarityPGI::getMemoryRequirement(Game* g_) {
	auto g = static_cast<PrecoalitionGame*>(g_);

	bigInt memory = (g->getWeightSum() + 1 - g->getQuota()) * gCalculator->getLargeNumberSize(); 
	memory += g->getMaxPrecoalitionSize() * gCalculator->getLargeNumberSize();											   
	memory += g->getMaxPrecoalitionSize() * c_sizeof_longUInt;															 
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
