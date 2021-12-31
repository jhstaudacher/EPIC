#include "index/ThreatPGI3.h"

#include "Logging.h"
#include "index/PublicGood.h"
#include "lint/GlobalCalculator.h"

epic::index::ThreatPGI3::ThreatPGI3()
	: PowerIndexWithPrecoalitions() {}

std::vector<epic::bigFloat> epic::index::ThreatPGI3::calculate(Game* g_) {
	auto g = static_cast<PrecoalitionGame*>(g_);
	std::vector<bigFloat> solution(g->getNumberOfPlayers());

	PublicGood* pgi = new PublicGood();

	const bigFloat cFloatOne("1");
	bigFloat denominator = 0;

	std::vector<bigFloat> intSolution(g->getNumberOfPrecoalitions() + 1);
	std::vector<bigFloat> intPGIs(g->getMaxPrecoalitionSize());
	std::vector<longUInt> weightsVector(g->getNumberOfPrecoalitions() + 1);

	for (longUInt i = 0; i < g->getNumberOfPrecoalitions(); i++) {
		longUInt nbPlayersInParti = g->getPrecoalitions()[i].size();

		for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
			{ // fill weightsVector with the weight of player ii, the weight of precoalition i without player ii followed by the weights of the remaining precoalitions except precoalition i
				weightsVector[0] = g->getWeights()[g->getPrecoalitions()[i][ii]];
				weightsVector[1] = g->getPrecoalitionWeights()[i] - weightsVector[0];
				for (longUInt j = 0; j < i; j++) {
					weightsVector[j + 2] = g->getPrecoalitionWeights()[j];
				}
				for (longUInt j = i + 1; j < g->getNumberOfPrecoalitions(); j++) {
					weightsVector[j + 1] = g->getPrecoalitionWeights()[j];
				}
			}

			auto intGame = new Game(g->getQuota(), weightsVector, false);
			pgi->calculate(intGame, intSolution);
			intPGIs[ii] = intSolution[intGame->getPermutation().applyIndex(0)]; // the solution for player ii

			delete intGame;
		}

		denominator = 0.0;
		for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
			denominator += intPGIs[ii];
		}
		// fill the solution to scale it later
		for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
			if (denominator > 0) {
				solution[g->getPrecoalitions()[i][ii]] = intPGIs[ii] / denominator;
			} else {
				solution[g->getPrecoalitions()[i][ii]] = cFloatOne / nbPlayersInParti;
			}
		}
	}

	intSolution.clear();
	intPGIs.clear();
	weightsVector.clear();

	// calculate external game
	std::vector<bigFloat> externalSolution(g->getNumberOfPrecoalitions());
	auto precoalitionGame = new Game(g->getQuota(), g->getPrecoalitionWeights(), false); // Create game object from weights of precoalitions with original quota
	pgi->calculate(precoalitionGame, externalSolution);

	delete precoalitionGame;
	delete pgi;

	// scale solution
	for (longUInt i = 0; i < g->getNumberOfPrecoalitions(); i++) {
		for (longUInt ii = 0; ii < g->getPrecoalitions()[i].size(); ii++) {
			solution[g->getPrecoalitions()[i][ii]] *= externalSolution[i];
		}
	}

	return solution;
}

std::string epic::index::ThreatPGI3::getFullName() {
	return "ThreatPGI3";
}

epic::longUInt epic::index::ThreatPGI3::getMemoryRequirement(Game* g_) {
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

epic::bigInt epic::index::ThreatPGI3::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_totalNumberOfSwingPlayer();
}

epic::lint::Operation epic::index::ThreatPGI3::getOperationRequirement() {
	return lint::Operation::multiplication;
}
