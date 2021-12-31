#include "index/OwenExtendedPGI.h"

#include "Logging.h"
#include "index/PublicGood.h"
#include "lint/GlobalCalculator.h"

#include <cmath>

epic::index::OwenExtendedPGI::OwenExtendedPGI()
	: PowerIndexWithPrecoalitions() {}

std::vector<epic::bigFloat> epic::index::OwenExtendedPGI::calculate(Game* g_) {
	auto g = static_cast<PrecoalitionGame*>(g_);
	std::vector<bigFloat> solution;

	// Create game object from weights of precoalitions with original quota
	auto precoalitionGame = new Game(g->getQuota(), g->getPrecoalitionWeights(), false);
	PublicGood* pgi = new PublicGood();

	//players in precoalitionGame are already in descending order
	if (precoalitionGame->getWeights()[0] >= g->getQuota()) {
		longUInt kk = precoalitionGame->getPermutation().inverseIndex(0);
		longUInt nbPlayersInParti = g->getPrecoalitions()[kk].size();

		Game* intGame = nullptr;
		{
			std::vector<longUInt> weightsVector(nbPlayersInParti);
			for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
				weightsVector[ii] = g->getWeights()[g->getPrecoalitions()[kk][ii]];
			}
			intGame = new Game(g->getQuota(), weightsVector, false);
		}

		std::vector<bigFloat> sortedSolution(nbPlayersInParti);
		{
			std::vector<bigFloat> intSolution(nbPlayersInParti);
			pgi->calculate(intGame, intSolution);

			intGame->getPermutation().reverse(intSolution, sortedSolution);
		}
		delete intGame;

		solution.resize(g->getNumberOfPlayers(), 0.0); // initialize with zero
		for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
			solution[g->getPrecoalitions()[kk][ii]] = sortedSolution[ii];
		}
	}
	else { // There is no dictator precoalition

		bigFloat big_float("0");
		bigInt big_tmp;

		auto interm = new lint::LargeNumber[g->getQuota()];
		gCalculator->allocInit_largeNumberArray(interm, g->getQuota());
		gCalculator->assign_one(interm[0]);

		solution.resize(g->getNumberOfPlayers(), 0.0); // initialize with zero
		std::vector<bigFloat> intPGIs(g->getMaxPrecoalitionSize());
		std::vector<std::vector<bigFloat>> helpPGIs(precoalitionGame->getWeights()[0], std::vector<bigFloat>(g->getMaxPrecoalitionSize()));

		for (longUInt i = 0; i < g->getNumberOfPrecoalitions(); i++) {
			for (auto& it : intPGIs) {
				it = 0;
			}

			longUInt kk = precoalitionGame->getPermutation().inverseIndex(i);
			//We need to process precoalitions by their weights ...
			longUInt nbPlayersInParti = g->getPrecoalitions()[kk].size();
			std::vector<longUInt> weightsVector(nbPlayersInParti);
			longUInt precoalWeight = precoalitionGame->getWeights()[i];
			long lower;
			long diff;

			// This works -- and weights of precoalitions are already in descending order.
			std::vector<bigFloat> helpSolution(nbPlayersInParti);
			std::vector<bigFloat> sortedHelpSolution(nbPlayersInParti);
			for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
				weightsVector[ii] = g->getWeights()[g->getPrecoalitions()[kk][ii]];
			}
			// Delete later: output weightsVector

			// calculate helpPGIs
			for (longUInt iii = 0; iii < precoalWeight; iii++) {
				if (nbPlayersInParti > 1) {
					auto helpGame = new Game(iii + 1, weightsVector, false);
					pgi->calculate(helpGame, helpSolution);
					helpGame->getPermutation().reverse(helpSolution, sortedHelpSolution);
					for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
						helpPGIs[iii][ii] = sortedHelpSolution[ii];
					}
					delete helpGame;
				} else {
					helpPGIs[iii][0] = 1;
					for (longUInt ii = 1; ii < nbPlayersInParti; ++ii) {
						helpPGIs[iii][ii] = 0;
					}
				}
			}

			if (i > 0) {
				for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
					for (longUInt weight = g->getQuota() - precoalWeight; weight <= (g->getQuota() - 1); weight++) {
						gCalculator->to_bigInt(&big_tmp, interm[weight - 1]);
						big_float = big_tmp;
						intPGIs[ii] += big_float * helpPGIs[g->getQuota() - weight - 1][ii];
					}
				}
			}
			if (i < (g->getNumberOfPrecoalitions()) - 1) {
				auto interm2 = new lint::LargeNumber[g->getQuota()];
				gCalculator->alloc_largeNumberArray(interm2, g->getQuota());
				gCalculator->assign_one(interm2[0]);
				for (longUInt weight = 0; weight <= g->getQuota() - 1; weight++) {
					gCalculator->assign(interm2[weight], interm[weight]);
				}
				for (longUInt j = i + 1; j < g->getNumberOfPrecoalitions(); j++) {
					longUInt precoalWeight_j = precoalitionGame->getWeights()[j];
					if (g->getQuota() <= (precoalWeight + precoalWeight_j)) {
						for (longUInt jj = 0; jj < nbPlayersInParti; jj++) {
							intPGIs[jj] += helpPGIs[g->getQuota() - precoalWeight_j - 1][jj];
						}
					} else {
						diff = g->getQuota() - precoalWeight - precoalWeight_j;
						lower = 1;
						if (diff > 1) {
							lower = diff;
						}

						for (longUInt jj = 0; jj < nbPlayersInParti; jj++) {
							for (longUInt weight = lower; weight <= (g->getQuota() - precoalWeight - 1); weight++) {
								gCalculator->to_bigInt(&big_tmp, interm2[weight - 1]);
								big_float = big_tmp;
								intPGIs[jj] += big_float * helpPGIs[g->getQuota() - precoalWeight_j - weight - 1][jj];
							}
						}
					} // end else

					// Update interm2
					for (longUInt weight = g->getQuota(); weight > precoalWeight_j; --weight) {
						gCalculator->plusEqual(interm2[weight - 1], interm2[weight - precoalWeight_j - 1]);
					}
				} // end for j

				gCalculator->free_largeNumberArray(interm2);
				delete[] interm2;
			} // end if

			// Update interm
			for (longUInt weight = g->getQuota(); weight > precoalWeight; --weight) {
				gCalculator->plusEqual(interm[weight - 1], interm[weight - precoalWeight - 1]);
			}

			for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
				solution[g->getPrecoalitions()[kk][ii]] = intPGIs[ii];
			}
		} // end for i

		intPGIs.clear();
		helpPGIs.clear();
		gCalculator->free_largeNumberArray(interm);
		delete[] interm;


		// calculate external games
		std::vector<bigFloat> externalSolution(g->getNumberOfPrecoalitions());
		pgi->calculate(precoalitionGame, externalSolution);

		RawPublicGood* rpgi = new RawPublicGood();
		auto rawExternalSolution = new lint::LargeNumber[g->getNumberOfPrecoalitions()];
		gCalculator->allocInit_largeNumberArray(rawExternalSolution, g->getNumberOfPrecoalitions());
		rpgi->calculate(precoalitionGame, rawExternalSolution);
		delete rpgi;

		// scale the solution
		for (longUInt i = 0; i < g->getNumberOfPrecoalitions(); i++) {
			longUInt kk = precoalitionGame->getPermutation().inverseIndex(i);
			longUInt nbPlayersInParti = g->getPrecoalitions()[kk].size();

			gCalculator->to_bigInt(&big_tmp, rawExternalSolution[i]);
			big_float = big_tmp;
			for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
				solution[g->getPrecoalitions()[kk][ii]] /= big_float;
				solution[g->getPrecoalitions()[kk][ii]] *= externalSolution[i];
			}
		}

		gCalculator->free_largeNumberArray(rawExternalSolution);
		delete[] rawExternalSolution;
	}

	delete precoalitionGame;
	delete pgi;

	return solution;
}

std::string epic::index::OwenExtendedPGI::getFullName() {
	return "OwenExtendedPGI";
}

epic::longUInt epic::index::OwenExtendedPGI::getMemoryRequirement(Game* g_) {
	auto g = static_cast<PrecoalitionGame*>(g_);

	bigInt memory = g->getNumberOfNonZeroPlayers() * (g->getQuota() - 2) * gCalculator->getLargeNumberSize(); // crude approximation for helpPGIs
	memory += (g->getQuota()) * gCalculator->getLargeNumberSize() * 2;										  // interm, interm2
	memory += g->getMaxPrecoalitionSize() * gCalculator->getLargeNumberSize();
	memory += g->getMaxPrecoalitionSize() * c_sizeof_longUInt;
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}
	return ret;
}

epic::bigInt epic::index::OwenExtendedPGI::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_totalNumberOfSwingPlayer();
}

epic::lint::Operation epic::index::OwenExtendedPGI::getOperationRequirement() {
	return lint::Operation::multiplication;
}
