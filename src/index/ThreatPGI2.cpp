#include "index/ThreatPGI2.h"
#include "index/PublicGood.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

#include <cmath>

epic::index::ThreatPGI2::ThreatPGI2() : PowerIndexWithPrecoalitions() {}

std::vector<epic::bigFloat> epic::index::ThreatPGI2::calculate(Game* g_) {
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
	
	bigFloat denominator = 0.0;
	
	PublicGood* extPGI = new PublicGood();
	
	extPGI->calculate(precoalitionGame, externalSolution);
	std::cout << "externalSolution:" << "\n";
	for (auto it : externalSolution) {
			std::cout << it << "\n";
	}
	std::cout << "\n";
	
	// Need internal solution (PGI over all players)
	// Rest should be very easy
	
	/* R Code
		w = partitionWeights
		extGame = weightedVotingGameVector(w=partitionWeights,n=nbPartitions,q=q)
		pgiExt = publicGoodValue(extGame)
		threatPGI2 = rep(0, nbPlayers)
		k = 1
		for (i in (1:nbPartitions)){
		  nbPlayersInParti = length(weightList[[i]])
		  nbIntGame = nbPartitions + nbPlayersInParti -1
		  weightsVector = rep(0, nbIntGame)
		  weightsVector[1:nbPlayersInParti] = weightList[[i]]
		  weightsVector[(nbPlayersInParti+1):nbIntGame] = partitionWeights[-i]
		  intGame = weightedVotingGameVector(w=weightsVector,n=nbIntGame,q=q)
		  pgiInt = publicGoodValue(intGame)
		  pgiInt
		  denominator = sum(pgiInt[1:nbPlayersInParti])
		  for (j in (1:nbPlayersInParti)){
			if (denominator > 0){
			  threatPGI2[k] = pgiExt[i] * pgiInt[j]/denominator
			}
			k = k+1
		  }
		}
	}*/
	
	for (longUInt i = 0; i < g->getNumberOfPrecoalitions(); i++) {
		longUInt nbPlayersInParti = g->getPrecoalitions()[i].size();
		longUInt nbIntGame = g->getNumberOfPrecoalitions() + nbPlayersInParti - 1;
		denominator = 0.0;
		std::vector<longUInt> weightsVector(nbIntGame);
		std::vector<bigFloat> intSolution(nbIntGame);
		std::vector<bigFloat> sortedSolution(nbIntGame);
		for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
			weightsVector[ii] = g->getWeights()[g->getPrecoalitions()[i][ii]];
		}
		for (longUInt j = 0; j < i; j++) {
			weightsVector[j+nbPlayersInParti] = g->getPrecoalitionWeights()[j];
		}
		for (longUInt j = i+1; j < g->getNumberOfPrecoalitions(); j++) {
			weightsVector[j+nbPlayersInParti-1] = g->getPrecoalitionWeights()[j];
		}
		// Create internal game for precoalition i
		auto intGame = new Game(quota, weightsVector, false);
		
		// Reuse PublicGood-Object we already have ...
	    extPGI->calculate(intGame,intSolution); 
		//
		std::cout << "intSolution:" << i << "\n";
		for (auto it : intSolution) {
			std::cout << it << "\n";
		}
		std::cout << "\n";
		//
		intGame->getPermutation().reverse(intSolution,sortedSolution);
		//
		std::cout << "sortedSolution:" << i << "\n";
		for (auto it : sortedSolution) {
			std::cout << it << "\n";
		}
		std::cout << "\n";
		//
		
		for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
			denominator += sortedSolution[ii];
		}
		std::cout << denominator << "\n";
		for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
			if (denominator > 0){
				solution[g->getPrecoalitions()[i][ii]] = externalSolution[i] * (sortedSolution[ii] / denominator);
			} else
			{
				solution[g->getPrecoalitions()[i][ii]] = 0.0;
			}
		}
		delete intGame;
    }

	
	delete precoalitionGame;
	delete extPGI;

	return solution;
}

std::string epic::index::ThreatPGI2::getFullName() {
	return "ThreatPGI2";
}

epic::longUInt epic::index::ThreatPGI2::getMemoryRequirement(Game* g_) {
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

epic::bigInt epic::index::ThreatPGI2::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_totalNumberOfSwingPlayer();
}

epic::lint::Operation epic::index::ThreatPGI2::getOperationRequirement() {
	return lint::Operation::multiplication;
}
