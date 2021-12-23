#include "index/ThreatPGI3.h"
#include "index/PublicGood.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

#include <cmath>

epic::index::ThreatPGI3::ThreatPGI3() : PowerIndexWithPrecoalitions() {}

std::vector<epic::bigFloat> epic::index::ThreatPGI3::calculate(Game* g_) {
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
	
	bigFloat denominator = 0;
	
	PublicGood* extPGI = new PublicGood();
	
	extPGI->calculate(precoalitionGame, externalSolution);
	std::cout << "externalSolution:" << "\n";
	for (auto it : externalSolution) {
			std::cout << it << "\n";
	}
	std::cout << "\n";
	
	
	/* R Code for ThreatPGI3:
		w = partitionWeights
		extGame = weightedVotingGameVector(w=partitionWeights,n=nbPartitions,q=q)
		pgiExt = publicGoodValue(extGame)
		threatPGI3 = rep(0, nbPlayers)
		k = 1
		for (i in (1:nbPartitions)){
		  nbPlayersInParti = length(weightList[[i]])
		  intPGIs = rep(0,nbPlayersInParti)
		  nbIntGame = nbPartitions + 1
		  weightsVector = rep(0, nbIntGame)
		  for (ii in (1:nbPlayersInParti)){
			weightsVector[1] = weightList[[i]][ii]
			weightsVector[2] = sum(weightList[[i]]) - weightsVector[1]
			weightsVector[3:nbIntGame] = partitionWeights[-i]
			intGame = weightedVotingGameVector(w=weightsVector,n=nbIntGame,q=q)
			pgiInt = publicGoodValue(intGame)
			intPGIs[ii] = pgiInt[1]
		  }
		  denominator = sum(intPGIs)
		  for (j in (1:nbPlayersInParti)){
			if (denominator > 0){
			  threatPGI3[k] = pgiExt[i] * intPGIs[j]/denominator
			} else {
			  threatPGI3[k] = pgiExt[i] / nbPlayersInParti
			}
			k = k+1
		  }
		}
	}*/
	for (longUInt i = 0; i < g->getNumberOfPrecoalitions(); i++) {
		longUInt nbPlayersInParti = g->getPrecoalitions()[i].size();
		longUInt nbIntGame = g->getNumberOfPrecoalitions() + 1;
		denominator = 0.0;
		std::vector<longUInt> weightsVector(nbIntGame);
		std::vector<bigFloat> intSolution(nbIntGame);
		std::vector<bigFloat> sortedSolution(nbIntGame);
		std::vector<bigFloat> intPGIs(nbIntGame);
		for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
			weightsVector[0] =  g->getWeights()[g->getPrecoalitions()[i][ii]];
			weightsVector[1] =  g->getPrecoalitionWeights()[i] - weightsVector[0];
			for (longUInt j = 0; j < i; j++) {
			weightsVector[j+2] = g->getPrecoalitionWeights()[j];
			}
			for (longUInt j = i+1; j < g->getNumberOfPrecoalitions(); j++) {
				weightsVector[j+1] = g->getPrecoalitionWeights()[j];
			}
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
			intPGIs[ii] = sortedSolution[0];
			delete intGame;
		}
		
		for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
			denominator += intPGIs[ii];
		}
		std::cout << denominator << "\n";
		for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
			if (denominator > 0){
				solution[g->getPrecoalitions()[i][ii]] = externalSolution[i] * (intPGIs[ii] / denominator);
			} else
			{
				solution[g->getPrecoalitions()[i][ii]] = externalSolution[i]/nbPlayersInParti;
			}
		}
    }

	
	delete precoalitionGame;
	delete extPGI;

	return solution;
}

std::string epic::index::ThreatPGI3::getFullName() {
	return "ThreatPGI3";
}

epic::longUInt epic::index::ThreatPGI3::getMemoryRequirement(Game* g_) {
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

epic::bigInt epic::index::ThreatPGI3::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_totalNumberOfSwingPlayer();
}

epic::lint::Operation epic::index::ThreatPGI3::getOperationRequirement() {
	return lint::Operation::multiplication;
}
