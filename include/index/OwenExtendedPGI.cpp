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
	
	std::vector<longUInt> preCoalitionWeights = g->getPrecoalitionWeights();
	std::cout << "prec weights" << "\n";
	for (auto it : preCoalitionWeights ){
		std::cout << it << "\n";
	}
	std::cout <<"\n";

	// Create game object from weights of precoalitions with original quota
	auto precoalitionGame = new Game(quota, preCoalitionWeights, false);
	
	std::vector<bigFloat> externalSolution(g->getNumberOfPrecoalitions());
	auto rawExternalSolution = new LargeNumber[g->getNumberOfPrecoalitions()];
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
	
	
	
	/* R Code
			w = partitionWeights
			extGame = weightedVotingGameVector(w=partitionWeights,n=nbPartitions,q=q)
			pgiExt = publicGoodValue(extGame)
			absPGIExt = absolutePublicGoodValue(extGame)
			owenPGI = rep(0, nbPlayers)
			mwcs = rep(0,length(w))
			k = 1
			# Treat case of dictator coalition first, 
			# assume precoalitions ordered by weight sum
			if (partitionWeights[1]>=q){
			  nbPlayersInParti = length(weightList[[1]])
			  dictGame = weightedVotingGameVector(w=weightList[[1]],n=nbPlayersInParti,q=q)
			  gpiDict = publicGoodValue(dictGame) 
			  owenPGI[1:nbPlayersInParti] =gpiDict
			  owenPGI[(nbPlayersInParti+1):nbPlayers] = rep(0, nbPlayers-nbPlayersInParti)
			} else{
			  for (i in (1:nbPartitions)){
				nbPlayersInParti = length(weightList[[i]])
				help = rep(0,nbPlayersInParti*w[i])
				helpPGIs=matrix(help, nrow=w[i], ncol=nbPlayersInParti)
				intPGIs = rep(0,nbPlayersInParti)
				forwards = ForwardCountingPerWeight_AllSteps(length(partitionWeights),q,partitionWeights)
				for (ii in (1:w[i])){
				  if (nbPlayersInParti > 1){
					helpGameVector = weightedVotingGameVector(n=nbPlayersInParti, q=ii, w=weightList[[i]])
					helpPGIs[ii,1:nbPlayersInParti] = publicGoodValue(helpGameVector)
				  } else {
					helpPGIs[ii,1] = 1
				  }
				}
				if (i>1){
				  interm = forwards[(i-1),]
				  mwcs[i] = mwcs[i] + sum(interm[(q-w[i]):(q-1)])
				  for (jj in (1:nbPlayersInParti)){
					for (ii in ((q-w[i]):(q-1))){
					  intPGIs[jj] = intPGIs[jj] + interm[ii] * helpPGIs[q-ii,jj]
					}
				  }
				}
				if (i<length(w)){
				  wi=w[-i]
				  intermForwards = ForwardCountingPerWeight_AllSteps(length(wi),q,wi)
				  for (j in ((i+1):length(w))){
					interm = rep(0,q)
					if (j>2){
					  interm = intermForwards[(j-2),]
					}
					if ((q-w[i]-w[j]) <= 0){
					  mwcs[i] = mwcs[i] +1
					  for (jj in (1:nbPlayersInParti)){
						#for (ii in (1:(q-w[i]-1))){
						  intPGIs[jj] = intPGIs[jj]  + helpPGIs[q-w[j],jj]
						#}
					  }
					}
					else{
					  lower=max(q-w[i]-w[j],1)
					  #mwcs[i] = mwcs[i] + sum(interm[lower:(q-w[i]-1)])
					  for (jj in (1:nbPlayersInParti)){
						for (ii in (lower:(q-w[i]-1))){
						  intPGIs[jj] = intPGIs[jj] + interm[ii] * helpPGIs[q-w[j]-ii,jj]
						}
					  }
					}  
				  }
				}
				for (jj in (1:nbPlayersInParti)){
				  intPGIs[jj] = intPGIs[jj]/absPGIExt[i]
				  intPGIs[jj] = pgiExt[i] * intPGIs[jj]
				}
				owenPGI[k:(k+nbPlayersInParti-1)] = intPGIs[1:nbPlayersInParti]
				k = k + nbPlayersInParti
			  }
			}	
	*/
	
    //players in precoalitionGame are already in descending order	
	if (precoalitionGame->getWeights()[0] >= quota){
		longUInt nbPlayersInParti = g->getPrecoalitions()[i].size();
		std::vector<longUInt> weightsVector(nbPlayersInParti);
		std::vector<bigFloat> intSolution(nbPlayersInParti);
		std::vector<bigFloat> sortedSolution(nbPlayersInParti);
		longUInt kk = precoalitionGame->getPermutation().inverseIndex(0);
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
	else {
		for (longUInt i = 0; i < g->getNumberOfPrecoalitions(); i++) {
			longUInt nbPlayersInParti = g->getPrecoalitions()[i].size();
			denominator += nbPlayersInParti * externalSolution[i];
		}
		
		for (longUInt i = 0; i < g->getNumberOfPrecoalitions(); i++) {
			longUInt nbPlayersInParti = g->getPrecoalitions()[i].size();
			for (longUInt ii = 0; ii < nbPlayersInParti; ii++) {
				solution[g->getPrecoalitions()[i][ii]] = externalSolution[i] / denominator;
			}
		}
	}
	
	delete precoalitionGame;
	delete extPGI;
	delete rawExtPGI;
	gCalculator->free_largeNumberArray(rawExternalSolution);
	delete[] rawExternalSolution;

	return solution;
}

void epic::index::OwenExtendedPGI::forward_counting_per_weight_next_step(Game* g, LargeNumber* ret_ptr, std::vector<epic::longUInt> weights, longUInt player_limit, bool first_step) {
	/*
	 * INITIALIZATION
	 */
	longUInt player_count = (weights.size() < g->getNumberOfNonZeroPlayers()) ? weights.size() : g->getNumberOfNonZeroPlayers();
	longUInt upper_sum = 0;
	auto upper = new longUInt[player_count];

	{ // initialize upper-array
		for (longUInt j = 0; j < player_count; ++j) {
			upper_sum += weights[j];
			if (upper_sum < g->getQuota()) {
				upper[j] = upper_sum;
			} else {
				upper[j] = g->getQuota();
			}
		}
	}

	if (first_step) {
		for (longUInt i = 0; i < g->getQuota(); ++i) {
			gCalculator->assign_zero(ret_ptr[i]);
		}

		for (longUInt i = 0; i < player_limit; ++i) {
			longUInt wi = weights[i];

			for (longUInt x = upper[i] + 1; x > wi; --x) {
				if (x - wi - 1 == 0) {
					gCalculator->increment(ret_ptr[x - 2]);
				} else {
					gCalculator->plusEqual(ret_ptr[x - 2], ret_ptr[x - wi - 2]);
				}
			}
		}
	}

	longUInt w_limit = weights[player_limit];
	for (longUInt x = upper[player_limit] + 1; x > w_limit; --x) {
		if (x - w_limit - 1 == 0) {
			gCalculator->increment(ret_ptr[x - 2]);
		} else {
			gCalculator->plusEqual(ret_ptr[x - 2], ret_ptr[x - w_limit - 2]);
			}
		}
	}

	delete[] upper;
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
