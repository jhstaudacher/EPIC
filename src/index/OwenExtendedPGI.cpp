#include "index/OwenExtendedPGI.h"
#include "index/PublicGood.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

#include <cmath>

epic::index::OwenExtendedPGI::OwenExtendedPGI() : PowerIndexWithPrecoalitions() {}

std::vector<epic::bigFloat> epic::index::OwenExtendedPGI::calculate(Game* g_) {
	auto g = static_cast<PrecoalitionGame*>(g_);

	std::vector<bigFloat> solution(g->getNumberOfPlayers(),0.0);
	longUInt quota = g->getQuota();
	mpf_class big_float("0");
	
	std::vector<longUInt> preCoalitionWeights = g->getPrecoalitionWeights();
	std::cout << "prec weights" << "\n";
	for (auto it : preCoalitionWeights ){
		std::cout << it << "\n";
	}
	std::cout <<"\n";

	// Create game object from weights of precoalitions with original quota
	auto precoalitionGame = new Game(quota, preCoalitionWeights, false);
	
	std::vector<bigFloat> externalSolution(g->getNumberOfPrecoalitions());
	auto rawExternalSolution = new lint::LargeNumber[g->getNumberOfPrecoalitions()];
	gCalculator->allocInit_largeNumberArray(rawExternalSolution,g->getNumberOfPrecoalitions()); 
	
	bigFloat denominator = 0;
	
	PublicGood* extPGI = new PublicGood();
	
	extPGI->calculate(precoalitionGame, externalSolution);
	std::cout << "externalSolution:" << "\n";
	for (auto it : externalSolution) {
			std::cout << it << "\n";
	}
	std::cout << "\n";
	//
	
	RawPublicGood* rawExtPGI = new RawPublicGood();
	rawExtPGI->calculate(precoalitionGame, rawExternalSolution);
	
	bigInt big_tmp;
	
	
    //players in precoalitionGame are already in descending order	
	if (precoalitionGame->getWeights()[0] >= quota){
		longUInt kk = precoalitionGame->getPermutation().inverseIndex(0);
		longUInt nbPlayersInParti = g->getPrecoalitions()[kk].size();
		std::vector<longUInt> weightsVector(nbPlayersInParti);
		std::vector<bigFloat> intSolution(nbPlayersInParti);
		std::vector<bigFloat> sortedSolution(nbPlayersInParti);
		for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
			weightsVector[ii] =  g->getWeights()[g->getPrecoalitions()[kk][ii]];
	    }
		auto intGame = new Game(quota, weightsVector, false);
		// Reuse PublicGood-Object we already have ...
		extPGI->calculate(intGame,intSolution); 
		intGame->getPermutation().reverse(intSolution,sortedSolution);
		for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
			solution[g->getPrecoalitions()[kk][ii]] =  sortedSolution[ii];
	    }
		delete intGame;
	}
    // Else: There is no dictator precoalition	
	else {
			auto interm = new lint::LargeNumber[quota];
			gCalculator->alloc_largeNumberArray(interm, quota);
			for (longUInt i = 0; i < g->getNumberOfPrecoalitions(); i++) {
				longUInt kk = precoalitionGame->getPermutation().inverseIndex(i);
				//We need to process precoalitions by their weights ...
				longUInt nbPlayersInParti = g->getPrecoalitions()[kk].size();
				std::vector<bigFloat> intPGIs(nbPlayersInParti);
				std::vector<longUInt> weightsVector(nbPlayersInParti);
				longUInt precoalWeight = precoalitionGame->getWeights()[i];
				long lower;
				long diff;
				std::cout << "i :  " << i << "\n";
				std::cout << "precoalWeight :  " <<  precoalWeight <<"\n";
				std::cout << "Number of players in that partition: \n";
                std::cout << nbPlayersInParti << "\n";				
				// This works -- and weights of precoalitions are already in descending order.
				// IMPORTANT!!! Data type for this matrix should be in C++
				std::vector<std::vector<bigFloat>> helpPGIs(precoalWeight, std::vector<bigFloat>(nbPlayersInParti, 0));
				std::vector<bigFloat> helpSolution(nbPlayersInParti);
				std::vector<bigFloat> sortedHelpSolution(nbPlayersInParti);
				for (longUInt ii=0; ii < nbPlayersInParti; ii++){
					weightsVector[ii] = g->getWeights()[g->getPrecoalitions()[kk][ii]];
				}
				// Delete later: output weightsVector
				std::cout << "weightsVector:" << "\n";
				for (auto it : weightsVector) {
						std::cout << it << "\n";
				}
				std::cout << "End of weightsVector \n";
				//
				std::cout << "Loop for helpPGIs: \n";
				for (longUInt iii=0; iii < precoalWeight; iii++){
					if (nbPlayersInParti > 1){
						auto helpGame = new Game(iii+1, weightsVector, false);
						extPGI->calculate(helpGame, helpSolution);
						helpGame->getPermutation().reverse(helpSolution,sortedHelpSolution);
						std::cout << "weight: " << iii << "\n";
						for (longUInt ii=0; ii < nbPlayersInParti; ii++){
							helpPGIs[iii][ii] = sortedHelpSolution[ii];
							std::cout << "ii : " << ii  << "\n";
							std::cout << "helpPGIs[iii][ii]: " << helpPGIs[iii][ii] << "\n";
						}
						delete helpGame;
					}
					else{
						helpPGIs[iii][0] = 1;
						std::cout << "helpPGIs[iii][0]: " << helpPGIs[iii][0] << "\n";
					}
				}
				std::cout << "End of for-loop for weights for helpPGIs ! \n";
				// J.St. -- Comment: Matrix helpPGIs should be fine ...
				    if (i>0){
					  // J.St. -- R code has got to go later ...
					  //for (jj in (1:nbPlayersInParti)){
					  //	for (ii in ((q-w[i]):(q-1))){
					  //	  intPGIs[jj] = intPGIs[jj] + interm[ii] * helpPGIs[q-ii,jj]
					  //	}
					  //}
						for (longUInt ii=0; ii < nbPlayersInParti; ii++){
							for (longUInt weight=quota-precoalWeight; weight <= (quota-1); weight++){
								gCalculator->to_bigInt(&big_tmp, interm[weight-1]);
								big_float = big_tmp.get_d();
								std::cout << "ii in i>1 : " << ii << "  interm[weight-1]: " << big_float << "\n";
								std::cout << "ii in i>1 : " << ii << "  helpPGIs[quota-weight-1][ii]: " << helpPGIs[quota-weight-1][ii] << "\n";
								intPGIs[ii] += big_float*helpPGIs[quota-weight-1][ii];
								std::cout << "ii in i>1 : " << ii << "  intPGIs[ii]: " << intPGIs[ii] << "\n";
							}
						}
						std::cout << "End of for loop for i>1 reached. \n";
					}
					if (i < (g->getNumberOfPrecoalitions())-1){
						   // J.St. -- R code has got to go later ...
						  //wi=w[-i]
						  //intermForwards = ForwardCountingPerWeight_AllSteps(length(wi),q,wi)
						  //intermForwardsCheck = forwardsCheck 
						  auto interm2 = new lint::LargeNumber[quota];
					      gCalculator->alloc_largeNumberArray(interm2, quota);
						  for (longUInt weight = 0; weight < quota -1; weight++){
							  interm2[weight] = interm[weight];
						  }
						  for (longUInt j=i+1; j < g->getNumberOfPrecoalitions(); j++){
							longUInt precoalWeight_j = precoalitionGame->getWeights()[j];
							std::cout << "j:" << j << "\n";
							std::cout << "precoalWeight_j:" << precoalWeight_j << "\n";
							if (quota <= (precoalWeight+precoalWeight_j)){
							  for (longUInt jj=0; jj <nbPlayersInParti; jj++){
								intPGIs[jj] += helpPGIs[quota-precoalWeight_j-1][jj];
								std::cout << "jj in i<n and if : " << jj << "  intPGIs[ii]: " << intPGIs[jj] << "\n";
							  }
							}
							else{
							  diff = quota-precoalWeight-precoalWeight_j;
							  std::cout << "diff:" << diff << "\n";
							  lower = 1;
							  if (diff > 1) {lower = diff;}
							  std::cout << "lower:" << lower << "\n";
							  for (longUInt jj=0; jj <nbPlayersInParti; jj++){
								for (longUInt weight=lower; weight <= (quota-precoalWeight-1); weight++){
								  	gCalculator->to_bigInt(&big_tmp, interm2[weight-1]);
									big_float = big_tmp.get_d();
									intPGIs[jj] += big_float*helpPGIs[quota-precoalWeight_j-weight-1][jj];
									std::cout << "jj in i<n and else : " << jj << "  interm2[weight-1]: " << big_float << "\n";
									std::cout << "jj in i<n and else : " << jj << "  helpPGIs[quota-precoalWeight_j-weight-1][jj]: " << helpPGIs[quota-precoalWeight_j-weight-1][jj] << "\n";
									std::cout << "jj in i<n and else : " << jj << "  intPGIs[ii]: " << intPGIs[jj] << "\n";
								}
							  }
							}  
							// Update interm2
							for (longUInt weight = quota; weight > precoalWeight_j; --weight) {
								if (weight - precoalWeight_j - 1 == 0) {
									gCalculator->increment(interm2[weight - 1]);
								} else {
									gCalculator->plusEqual(interm2[weight - 1], interm2[weight - precoalWeight_j - 1]);
								}
							}
							//for (x in q:w[j]){
							//  if ((x-w[j]==0))
							//	intermForwardsCheck[x] = intermForwardsCheck[x] +1
							//  else
							//	intermForwardsCheck[x] = intermForwardsCheck[x] + intermForwardsCheck[x-w[j]]
							//}
						} // end for j */
						std::cout << "End of for loop for j reached. \n";
						gCalculator->free_largeNumberArray(interm2);
						delete[] interm2;
					} // end if 
					// Update interm
					for (longUInt weight = quota; weight > precoalWeight; --weight) {
							if ((weight - precoalWeight) == 0) {
									gCalculator->increment(interm[weight - 1]);
							} else {
									gCalculator->plusEqual(interm[weight - 1], interm[weight - precoalWeight - 1]);
							}
					}
					std::cout << "solution for i:" << i << "\n";
					gCalculator->to_bigInt(&big_tmp, rawExternalSolution[i]);
					big_float = big_tmp.get_d();
					std::cout << "rawExternalSolution[i]: " << big_float << "\n";
					std::cout << "externalSolution[i]: " << externalSolution[i] << "\n";
					for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
						std::cout << "ii : " << ii << " intPGIs[ii] : " << intPGIs[ii] << "\n";
						solution[g->getPrecoalitions()[kk][ii]] = (intPGIs[ii] / big_float)*externalSolution[i];
						std::cout << "solution[g->getPrecoalitions()[kk][ii]] :  " << solution[g->getPrecoalitions()[kk][ii]] << "\n";
					}
			} // for i
			gCalculator->free_largeNumberArray(interm);
			delete[] interm;
	}
	
	delete precoalitionGame;
	delete extPGI;
	delete rawExtPGI;
	gCalculator->free_largeNumberArray(rawExternalSolution);
	delete[] rawExternalSolution;

	return solution;
}


std::string epic::index::OwenExtendedPGI::getFullName() {
	return "OwenExtendedPGI";
}

epic::longUInt epic::index::OwenExtendedPGI::getMemoryRequirement(Game* g_) {
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

epic::bigInt epic::index::OwenExtendedPGI::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_totalNumberOfSwingPlayer();
}

epic::lint::Operation epic::index::OwenExtendedPGI::getOperationRequirement() {
	return lint::Operation::multiplication;
}
