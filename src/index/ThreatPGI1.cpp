#include "index/ThreatPGI1.h"
#include "index/PublicGood.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

#include <cmath>

epic::index::ThreatPGI1::ThreatPGI1() : PowerIndexWithPrecoalitions() {}

std::vector<epic::bigFloat> epic::index::ThreatPGI1::calculate(Game* g_) {
	auto g = static_cast<PrecoalitionGame*>(g_);

	std::vector<bigFloat> solution(g->getNumberOfPlayers());
	std::vector<bigFloat> intSolution(g->getNumberOfPlayers());
	longUInt quota = g->getQuota();
	
	std::vector<longUInt> preCoalitionWeights = g->getPrecoalitionWeights();
	/*std::cout << "prec weights" << "\n";
	for (auto it : preCoalitionWeights ){
		std::cout << it << "\n";
	}
	std::cout <<"\n";*/

	// Create game object from weights of precoalitions with original quota
	auto precoalitionGame = new Game(quota, preCoalitionWeights, false);
	
	std::vector<bigFloat> externalSolution(g->getNumberOfPrecoalitions());
	
	bigFloat denominator = 0;
	
	PublicGood* extPGI = new PublicGood();
	
	extPGI->calculate(precoalitionGame, externalSolution);
	std::cout << "externalSolution:" << "\n";
	for (auto it : externalSolution) {
			std::cout << it << "\n";
	}
	std::cout << "\n";
	
	// Create game object from weights of players without precoalitions and with original quota
	//PublicGood* pgi = new PublicGood();
	// Reuse PublicGood-Object we already have ...
	extPGI->calculate(g,intSolution);
	
	
	for (longUInt i = 0; i < g->getNumberOfPrecoalitions(); i++) {
		longUInt nbPlayersInParti = g->getPrecoalitions()[i].size();
		denominator = 0.0;
		for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
			denominator += intSolution[g->getPrecoalitions()[i][ii]];
		}
		for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
			if (denominator > 0){
				solution[g->getPrecoalitions()[i][ii]] = externalSolution[i] * (intSolution[g->getPrecoalitions()[i][ii]] / denominator);
			} else
			{
				solution[g->getPrecoalitions()[i][ii]] = 0.0;
			}
		}
    }
	
	delete precoalitionGame;
	delete extPGI;

	return solution;
}

std::string epic::index::ThreatPGI1::getFullName() {
	return "ThreatPGI1";
}

epic::longUInt epic::index::ThreatPGI1::getMemoryRequirement(Game* g_) {
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

epic::bigInt epic::index::ThreatPGI1::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_totalNumberOfSwingPlayer();
}

epic::lint::Operation epic::index::ThreatPGI1::getOperationRequirement() {
	return lint::Operation::multiplication;
}
