#include "index/Owen.h"

#include "Array.h"

#include <numeric>

// DEBUG
#include <iostream>

epic::index::Owen::Owen(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: ItfPowerIndex(g) {

}

epic::index::Owen::~Owen() {

}

epic::longUInt factorial(epic::longUInt x) {
	if (x == 0) {
		return 1;
	}
	for (epic::longUInt i = x - 1; i > 0; --i) {
		x *= i;
	}
	return x;
}

std::vector<epic::bigFloat> epic::index::Owen::calculate() {
	// nbPlayers = length(unlist(weightList))
	longUInt nbPlayers = mGame.getNumberOfPlayers();
	// nbPartitions = length(weightList)
	longUInt nbPartitions = mGame.getPrecoalitions().size();
	// partitionWeights = rep(0, nbPartitions)
	auto partitionWeights = new longUInt[nbPartitions]();

	// maxMembersInPartition = 1
	longUInt maxMembersInPartition = 1;
	// membersInPartition = 0
	longUInt membersInPartition = 0;

	// for (i in 1:nbPartitions){
	for (longUInt i = 0; i < nbPartitions; ++i) {
		// partitionWeights[i] = sum(weightList[[i]])
		for (longUInt p = 0; p < mGame.getPrecoalitions()[i].size(); ++p) {
			partitionWeights[i] += mGame.getWeights()[mGame.getPrecoalitions()[i][p] - 1];
		}
		// membersInPartition = length(weightList[[i]])
		membersInPartition = mGame.getPrecoalitions()[i].size();
		// if (membersInPartition > maxMembersInPartition) {
		if (membersInPartition > maxMembersInPartition) {
			// maxMembersInPartition = membersInPartition
			maxMembersInPartition = membersInPartition;
		}
	}

	// w = partitionWeights
	longUInt* w = partitionWeights;
	// capitalC = sum(w)
	longUInt capitalC = 0;
	for (longUInt i = 0; i < nbPartitions; ++i) {
		capitalC += partitionWeights[i];
	}

	// shapleysExternalGame = rep(0,length(w))
	auto shapleysExternalGame = new double[nbPartitions]();
	// owenIndices = rep(0,nbPlayers)
	auto owenIndices = new double[nbPlayers]();
	// owenIndicesCheck = rep(0,nbPlayers)
	auto owenIndicesCheck = new double[nbPlayers]();

	// cc = BackwardCountingPerWeightCardinality(nbPartitions,q,w)
	Array2d<longUInt> cc(capitalC + 1, nbPartitions, true);
	backwardCountingPerWeightAndCardinality(cc, capitalC, partitionWeights);

	// cwvec=rep(0, capitalC*nbPartitions)
	// cw=matrix(cwvec, nrow=capitalC, ncol=nbPartitions)
	Array2d<longUInt> cw(capitalC + 1, nbPartitions, true);
	// cw[q:capitalC, 1:nbPartitions]=cc[q:capitalC, 1:nbPartitions]
	for (longUInt w = mGame.getQuota(); w <= capitalC; ++w) {
		for (longUInt c = 0; c < nbPartitions; ++c) {
			cw(w, c) = cc(w, c);
		}
	}

	// DEBUG
	std::cout << "cw untouched:" << std::endl;
	for (longUInt x = 1; x <= capitalC; ++x) {
		std::cout << "[" << x << ", ]";
		for (longUInt y = 0; y < nbPartitions; ++y) {
			std::cout << "\t" << cw(x, y);
		}
		std::cout << std::endl;
	}

	// player = 1
	longUInt player = 0;
	// for (i in (1:nbPartitions)){
	for (longUInt i = 0; i < nbPartitions; ++i) {
		// for (x in (q:sum(w))){ cw[x,1:nbPartitions] =cc[x,1:nbPartitions]}
		for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
			for (longUInt z = 0; z < nbPartitions; ++z) {
				cw(x, z) = cc(x, z);
			}
		}

		// if ((sum(w)-w[i]) >= q){
		if (mGame.getWeightSum() - w[i] >= mGame.getQuota()) {
			// for (x in seq(from = (sum(w)-w[i]), to=q, by=-1)){
			for (longUInt x = mGame.getWeightSum() - w[i]; x >= mGame.getQuota(); --x) {
				// s = 1
				// while (s < nbPartitions){
				for (longUInt s = 1; s < nbPartitions; ++s) {
					// cw[x,nbPartitions-s] = cc[x,nbPartitions-s]- cw[x+w[i],nbPartitions-s+1]
					cw(x, nbPartitions - s - 1) = cc(x, nbPartitions - s - 1) - cw(x + w[i], nbPartitions - s);
					// s = s+1
				}
			}
		}

		// DEBUG
		std::cout << "i: " << i << std::endl;
		std::cout << "cw:" << std::endl;
		for (longUInt x = 1; x <= capitalC; ++x) {
			std::cout << "[" << x << ", ]";
			for (longUInt y = 0; y < nbPartitions; ++y) {
				std::cout << "\t" << cw(x, y);
			}
			std::cout << std::endl;
		}

		// nbPlayersInParti = length(weightList[[i]])
		longUInt nbPlayersInParti = mGame.getPrecoalitions()[i].size();
		// cw2vec=rep(0, capitalC*nbPlayersInParti)
		// cw2=matrix(cw2vec, nrow=capitalC, ncol=nbPlayersInParti)
		Array2d<longUInt> cw2(capitalC + 1, nbPlayersInParti, false);
		// winternal = rep(0,nbPlayersInParti)
		auto winternal = new longUInt[nbPlayersInParti];
		// winternal = weightList[[i]]
		for (longUInt z = 0; z < nbPlayersInParti; ++z) {
			winternal[z] = mGame.getWeights()[mGame.getPrecoalitions()[i][z]];
		}
		// shapleysInternal = rep(0,nbPlayersInParti)
		auto shapleysInternal = new longUInt[nbPlayersInParti]();
		// rawShapleysInternal = shapleysInternal
		auto rawShapleysInternal = new longUInt[nbPlayersInParti]();

		// for (s in (1:(nbPartitions-1))){
		for (longUInt s = 0; s < nbPartitions - 1; ++s) {
			// factor = factorial(s)*factorial(nbPartitions-s-1)
			longUInt factor = factorial(s + 1) * factorial(nbPartitions - s - 2);
			// shapleysExternalGame[i] = shapleysExternalGame[i] + factor * sum(cw[(q:min(q+w[i]-1,capitalC)),s+1])
			longUInt tmp_sum = 0;
			longUInt tmp_min = std::min(mGame.getQuota() + w[i] - 1, capitalC);
			for (longUInt z = mGame.getQuota(); z <= tmp_min; ++z) {
				tmp_sum += cw(z, s + 1);
			}
			shapleysExternalGame[i] += factor * tmp_sum;

			// if (nbPlayersInParti > 1){
			if (nbPlayersInParti > 1) {
				// cw2=matrix(cw2vec, nrow=capitalC, ncol=nbPlayersInParti)
				for (longUInt x = 1; x <= capitalC; ++x) {
					for (longUInt y = 0; y < nbPlayersInParti - 1; ++y) {
						cw2(x, y) = 0;
					}
				}
				// cw2[1:capitalC, nbPlayersInParti]=cw[1:capitalC,s+1]
				for (longUInt x = 1; x <= capitalC; ++x) {
					cw2(x, nbPlayersInParti - 1) = cw(x, s + 1);
				}

				// DEBUG
				std::cout << "s+1 -- with s being index from loop over number of precoalitions: " << s+1 << std::endl;
				std::cout << "cw2[q:capitalC,nbPlayersInParti-1]" << std::endl;
				for (longUInt z = mGame.getQuota(); z <= capitalC; ++z) {
					std::cout << "[" << z << ", " << nbPlayersInParti-1 << "] = " << cw2(z, nbPlayersInParti-1) << std::endl;
				}

				// for (ii in 1:length(winternal)){
				for (longUInt ii = 0; ii < nbPlayersInParti; ++ii) {
					// x = q+winternal[ii]
					// while (x <= capitalC){
					for (longUInt x = mGame.getQuota() + winternal[ii]; x <= capitalC; ++x) {
						// m=nbPlayersInParti
						// while(m>1){
						for (longUInt m = nbPlayersInParti - 1; m > 0; --m) {
							// cw2[x-winternal[ii],m-1] = cw2[x,m] + cw2[x-winternal[ii],m-1]
							cw2(x - winternal[ii], m - 1) = cw2(x, m) + cw2(x - winternal[ii], m - 1);
							// m=m-1
						}
						// x=x+1
					}
				}

				// DEBUG
				std::cout << "cw2:" << std::endl;
				for (longUInt x = 1; x <= capitalC; ++x) {
					std::cout << "[" << x << ", ]";
					for (longUInt y = 0; y < nbPlayersInParti; ++y) {
						std::cout << "\t" << cw2(x, y);
					}
					std::cout << std::endl;
				}

				// cwi = cw2
				Array2d<longUInt> cwi(capitalC + 1, nbPlayersInParti);
				for (longUInt x = 0; x <= capitalC; ++x) {
					for (longUInt y = 0; y < nbPlayersInParti; ++y) {
						cwi(x, y) = cw2(x, y);
					}
				}
				// for (ii in (1:nbPlayersInParti)){
				for (longUInt ii = 0; ii < nbPlayersInParti; ++ii) {
					// for (x in (q:sum(w))){cwi[x,1:nbPlayersInParti] =cw2[x,1:nbPlayersInParti]}
					for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
						for (longUInt y = 0; y < nbPlayersInParti; ++y) {
							cwi(x, y) = cw2(x, y);
						}
					}
					// if ((sum(w)-winternal[ii]) >= q){
					if (mGame.getWeightSum() - winternal[ii] >= mGame.getQuota()) {
						// for (x in seq(from = (sum(w)-winternal[ii]), to=q, by=-1)){
						for (longUInt x = mGame.getWeightSum() - winternal[ii]; x >= mGame.getQuota(); --x) {
							// sinternal = 1
							// while (sinternal < nbPlayersInParti){
							for (longUInt sinternal = 0; sinternal <= nbPlayersInParti; ++sinternal) {
								// cwi[x,nbPlayersInParti-sinternal] = cw2[x,nbPlayersInParti-sinternal]- cwi[x+winternal[ii],nbPlayersInParti-sinternal+1]
								cwi(x, nbPlayersInParti - sinternal - 1) = cw2(x, nbPlayersInParti - sinternal - 1) - cwi(x + winternal[ii], nbPlayersInParti - sinternal);
								// sinternal = sinternal+1
							}
						}
					}

					// DEBUG
					std::cout << "cwi:" << std::endl;
					for (longUInt x = 1; x <= capitalC; ++x) {
						std::cout << "[" << x << ", ]";
						for (longUInt y = 0; y < nbPlayersInParti; ++y) {
							std::cout << "\t" << cwi(x, y);
						}
						std::cout << std::endl;
					}

					// for (sinternal in 0:(nbPlayersInParti-1)){
					for (longInt sinternal = 0; sinternal < nbPlayersInParti; ++sinternal) { // ! Using sinternal increased by 1
						// factorInternal = factorial(sinternal)*factorial(nbPlayersInParti-sinternal-1)
						longUInt factorInternal = factorial(sinternal) * factorial(nbPlayersInParti - sinternal - 1);

						// shapleysInternal[ii] = shapleysInternal[ii] + factor * factorInternal * sum(cwi[(q:(q+winternal[ii]-1)),sinternal+1])
						longUInt tmp_sum = 0;
						for (longUInt w = mGame.getQuota(); w <= mGame.getQuota() + winternal[ii] - 1; ++w) {
							tmp_sum += cwi(w, sinternal);
						}
						shapleysInternal[ii] += factor * factorInternal * tmp_sum;
						// rawShapleysInternal[ii] = shapleysInternal[ii]
						rawShapleysInternal[ii] = shapleysInternal[ii];

						// DEBUG
						std::cout << "rawShapleysInternal[ii]: " << rawShapleysInternal[ii] << std::endl;
					}
				}
			} else {
				// shapleysInternal[1]=shapleysExternalGame[i]
				shapleysInternal[0] = shapleysExternalGame[i];
				// rawShapleysInternal[1]=shapleysInternal[1]
				rawShapleysInternal[0] = shapleysInternal[0];
			}

			// DEBUG
			std::cout << "s: " << s << std::endl;
			std::cout << "rawShapleysInternal: " << std::endl;
			for (longUInt z = 0; z < nbPlayersInParti; ++z) {
				std::cout << "[" << z << "]\t" << rawShapleysInternal[z] << std::endl;
			}
		}

		// DEBUG
		std::cout << "i (after external Shapley loop for precoalitions): " << i << std::endl;

		// shapleysExternalGame[i] = shapleysExternalGame[i]/factorial(nbPartitions)
		shapleysExternalGame[i] /= factorial(nbPartitions);

		// DEBUG
		std::cout << "shapleysExternalGame[i]: " << shapleysExternalGame[i] << std::endl;

		// sumRawShapleysInternal = sum(rawShapleysInternal)
		longUInt sumRawShapleysInternal = 0;
		for (longUInt x = 0; x < nbPlayersInParti; ++x) {
			sumRawShapleysInternal += rawShapleysInternal[x];
		}

		// for (ii in (1:nbPlayersInParti)){
		for (longUInt ii = 0; ii < nbPlayersInParti; ++ii) {
			// owenIndices[player] = shapleysInternal[ii]/(factorial(nbPlayersInParti)*factorial(nbPartitions))
			owenIndices[player] = static_cast<double>(shapleysInternal[ii]) / (factorial(nbPlayersInParti) * factorial(nbPartitions));
			// owenIndicesCheck[player] = shapleysExternalGame[i] * rawShapleysInternal[ii]/sumRawShapleysInternal
			owenIndicesCheck[player] = static_cast<double>(shapleysExternalGame[i]) * rawShapleysInternal[ii] / sumRawShapleysInternal;

			// DEBUG
			std::cout << "player number: " << player << std::endl;
			std::cout << "Owen index of player: " << owenIndices[player] << std::endl;
			std::cout << "Check for Owen index of player: " << owenIndicesCheck[player] << std::endl;

			// player = player + 1
			++player;
		}

		delete[] rawShapleysInternal;
		delete[] shapleysInternal;
		delete[] winternal;
	}

	// DEBUG
	std::cout << "shapleysExternalGame:" << std::endl;
	for (longUInt x = 0; x < nbPartitions; ++x) {
		std::cout << "[" << x << "]\t" << shapleysExternalGame[x] << std::endl;
	}

	std::cout << "owenIndices:" << std::endl;
	for (longUInt x = 0; x < mGame.getNumberOfPlayers(); ++x) {
		std::cout << "[" << x << "]\t" << owenIndices[x] << std::endl;
	}

	std::cout << "owenIndicesCheck:" << std::endl;
	for (longUInt x = 0; x < mGame.getNumberOfPlayers(); ++x) {
		std::cout << "[" << x << "]\t" << owenIndicesCheck[x] << std::endl;
	}

	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
		solution[i] = owenIndices[i];
	}

	delete[] partitionWeights;
	delete[] shapleysExternalGame;
	delete[] owenIndices;
	delete[] owenIndicesCheck;


	return solution;
}

std::string epic::index::Owen::getFullName() {
	return "Owen";
}

epic::longUInt epic::index::Owen::getMemoryRequirement() {

}

// ! c must be allocated and zero-initialized in the range [0, capitalC] x [0, nbPartitions - 1]
epic::longUInt epic::index::Owen::backwardCountingPerWeightAndCardinality(Array2d<longUInt>& c, longUInt capitalC, longUInt* partition_weights) {
	longUInt n = mGame.getPrecoalitions().size();
	longUInt q = mGame.getQuota();
	longUInt* w = partition_weights;

	// c[capitalC,n] = 1
	c(capitalC, n - 1) = 1;
	// for (i in 1:n){
	for (longUInt i = 0; i < n; ++i) {
		// x = q+w[i]
		// while (x <= capitalC){
		for (longUInt x = q + w[i]; x <= capitalC; ++x) {
			// m=2
			// while(m<(n+1)){
			for (longUInt m = 1; m < n; ++m) {
				// c[x-w[i],m-1] = c[x,m] + c[x-w[i],m-1]
				c(x - w[i], m - 1) = c(x, m) + c(x - w[i], m - 1);
				// m=m+1
			}
			// x=x+1
		}
	}
}
