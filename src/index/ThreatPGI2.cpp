#include "index/ThreatPGI2.h"
#include "index/PublicGood.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

#include <cmath>

epic::index::ThreatPGI2::ThreatPGI2() : PowerIndexWithPrecoalitions() {}

std::vector<epic::bigFloat> epic::index::ThreatPGI2::calculate(Game* g_) {
	auto g = static_cast<PrecoalitionGame*>(g_);
	std::vector<bigFloat> solution(g->getNumberOfPlayers(), 0);

	PublicGood* pgi = new PublicGood();

	bigFloat denominator = 0.0;
	for (longUInt i = 0; i < g->getNumberOfPrecoalitions(); i++) {
		longUInt nbPlayersInParti = g->getPrecoalitions()[i].size();
		longUInt nbIntGame = g->getNumberOfPrecoalitions() + nbPlayersInParti - 1;

		std::vector<longUInt> weightsVector(nbIntGame);
		{ // fill weightsVector containing the player weights of precoalition i followed by the precoalition weights except precoalition i
			for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
				weightsVector[ii] = g->getWeights()[g->getPrecoalitions()[i][ii]];
			}
			for (longUInt j = 0; j < i; j++) {
				weightsVector[j+nbPlayersInParti] = g->getPrecoalitionWeights()[j];
			}
			for (longUInt j = i+1; j < g->getNumberOfPrecoalitions(); j++) {
				weightsVector[j+nbPlayersInParti-1] = g->getPrecoalitionWeights()[j];
			}
		}

		// Create internal game for precoalition i
		auto intGame = new Game(g->getQuota(), weightsVector, false);

		std::vector<bigFloat> intSolution(nbIntGame);
		pgi->calculate(intGame,intSolution);

		std::vector<bigFloat> sortedSolution(nbIntGame);
		intGame->getPermutation().reverse(intSolution,sortedSolution);

		denominator = 0.0;
		for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
			denominator += sortedSolution[ii];
		}
		for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
			if (denominator > 0){
				solution[g->getPrecoalitions()[i][ii]] = sortedSolution[ii] / denominator;
			} // else solution[g->getPrecoalitions()[i][ii]] = 0.0;
		}

		delete intGame;
	}


	std::vector<bigFloat> externalSolution(g->getNumberOfPrecoalitions());
	// Create game object from weights of precoalitions with original quota
	auto precoalitionGame = new Game(g->getQuota(), g->getPrecoalitionWeights(), false);
	pgi->calculate(precoalitionGame, externalSolution);

	// scale solution
	for (longUInt i = 0; i < g->getNumberOfPrecoalitions(); ++i) {
		for (longUInt ii = 0; ii < g->getPrecoalitions()[i].size(); ++ii) {
			solution[g->getPrecoalitions()[i][ii]] *= externalSolution[i];
		}
	}


	delete precoalitionGame;
	delete pgi;

	return solution;
}

std::string epic::index::ThreatPGI2::getFullName() {
	return "ThreatPGI2";
}

epic::longUInt epic::index::ThreatPGI2::getMemoryRequirement(Game* g_) {
	auto g = static_cast<PrecoalitionGame*>(g_);

	bigInt memory = (g->getWeightSum() + 1 - g->getQuota()) * gCalculator->getLargeNumberSize(); 
	memory += g->getMaxPrecoalitionSize() * gCalculator->getLargeNumberSize() * 4;
	memory += g->getMaxPrecoalitionSize() * c_sizeof_longUInt;
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
