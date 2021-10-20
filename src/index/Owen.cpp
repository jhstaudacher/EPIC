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
	longUInt nbPlayers = mGame.getNumberOfPlayers();
	longUInt nbPartitions = mGame.getPrecoalitions().size();
	auto partitionWeights = new longUInt[nbPartitions]();

	longUInt maxMembersInPartition = 1;
	longUInt membersInPartition = 0;

	for (longUInt i = 0; i < nbPartitions; ++i) {
		for (longUInt p = 0; p < mGame.getPrecoalitions()[i].size(); ++p) {
			partitionWeights[i] += mGame.getWeights()[mGame.getPrecoalitions()[i][p] - 1];
		}
		membersInPartition = mGame.getPrecoalitions()[i].size();
		if (membersInPartition > maxMembersInPartition) {
			maxMembersInPartition = membersInPartition;
		}
	}

	longUInt* w = partitionWeights;
	longUInt capitalC = 0;
	for (longUInt i = 0; i < nbPartitions; ++i) {
		capitalC += partitionWeights[i];
	}

	auto shapleysExternalGame = new double[nbPartitions]();
	auto owenIndices = new double[nbPlayers]();
	auto owenIndicesCheck = new double[nbPlayers]();

	Array2d<longUInt> cc(capitalC + 1, nbPartitions, true);
	backwardCountingPerWeightAndCardinality(cc, capitalC, partitionWeights);

	Array2d<longUInt> cw(capitalC + 1, nbPartitions, true);
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

	longUInt player = 0;
	for (longUInt i = 0; i < nbPartitions; ++i) {
		for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
			for (longUInt z = 0; z < nbPartitions; ++z) {
				cw(x, z) = cc(x, z);
			}
		}

		if (mGame.getWeightSum() - w[i] >= mGame.getQuota()) {
			for (longUInt x = mGame.getWeightSum() - w[i]; x >= mGame.getQuota(); --x) {
				for (longUInt s = 1; s < nbPartitions; ++s) {
					cw(x, nbPartitions - s - 1) = cc(x, nbPartitions - s - 1) - cw(x + w[i], nbPartitions - s);
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

		longUInt nbPlayersInParti = mGame.getPrecoalitions()[i].size();
		Array2d<longUInt> cw2(capitalC + 1, nbPlayersInParti, false);
		auto winternal = new longUInt[nbPlayersInParti];
		for (longUInt z = 0; z < nbPlayersInParti; ++z) {
			winternal[z] = mGame.getWeights()[mGame.getPrecoalitions()[i][z]];
		}
		auto shapleysInternal = new longUInt[nbPlayersInParti]();
		auto rawShapleysInternal = new longUInt[nbPlayersInParti]();

		for (longUInt s = 0; s < nbPartitions - 1; ++s) {
			longUInt factor = factorial(s + 1) * factorial(nbPartitions - s - 2);
			longUInt tmp_sum = 0;
			longUInt tmp_min = std::min(mGame.getQuota() + w[i] - 1, capitalC);
			for (longUInt z = mGame.getQuota(); z <= tmp_min; ++z) {
				tmp_sum += cw(z, s + 1);
			}
			shapleysExternalGame[i] += factor * tmp_sum;

			if (nbPlayersInParti > 1) {
				for (longUInt x = 1; x <= capitalC; ++x) {
					for (longUInt y = 0; y < nbPlayersInParti - 1; ++y) {
						cw2(x, y) = 0;
					}
				}
				for (longUInt x = 1; x <= capitalC; ++x) {
					cw2(x, nbPlayersInParti - 1) = cw(x, s + 1);
				}

				// DEBUG
				std::cout << "s+1 -- with s being index from loop over number of precoalitions: " << s+1 << std::endl;
				std::cout << "cw2[q:capitalC,nbPlayersInParti-1]" << std::endl;
				for (longUInt z = mGame.getQuota(); z <= capitalC; ++z) {
					std::cout << "[" << z << ", " << nbPlayersInParti-1 << "] = " << cw2(z, nbPlayersInParti-1) << std::endl;
				}

				for (longUInt ii = 0; ii < nbPlayersInParti; ++ii) {
					for (longUInt x = mGame.getQuota() + winternal[ii]; x <= capitalC; ++x) {
						for (longUInt m = nbPlayersInParti - 1; m > 0; --m) {
							cw2(x - winternal[ii], m - 1) = cw2(x, m) + cw2(x - winternal[ii], m - 1);
						}
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

				Array2d<longUInt> cwi(capitalC + 1, nbPlayersInParti);
				for (longUInt x = 0; x <= capitalC; ++x) {
					for (longUInt y = 0; y < nbPlayersInParti; ++y) {
						cwi(x, y) = cw2(x, y);
					}
				}
				for (longUInt ii = 0; ii < nbPlayersInParti; ++ii) {
					for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
						for (longUInt y = 0; y < nbPlayersInParti; ++y) {
							cwi(x, y) = cw2(x, y);
						}
					}
					if (mGame.getWeightSum() - winternal[ii] >= mGame.getQuota()) {
						for (longUInt x = mGame.getWeightSum() - winternal[ii]; x >= mGame.getQuota(); --x) {
							for (longUInt sinternal = 0; sinternal <= nbPlayersInParti; ++sinternal) {
								cwi(x, nbPlayersInParti - sinternal - 1) = cw2(x, nbPlayersInParti - sinternal - 1) - cwi(x + winternal[ii], nbPlayersInParti - sinternal);
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

					for (longInt sinternal = 0; sinternal < nbPlayersInParti; ++sinternal) { // ! Using sinternal increased by 1
						longUInt factorInternal = factorial(sinternal) * factorial(nbPlayersInParti - sinternal - 1);

						longUInt tmp_sum = 0;
						for (longUInt w = mGame.getQuota(); w <= mGame.getQuota() + winternal[ii] - 1; ++w) {
							tmp_sum += cwi(w, sinternal);
						}
						shapleysInternal[ii] += factor * factorInternal * tmp_sum;
						rawShapleysInternal[ii] = shapleysInternal[ii];

						// DEBUG
						std::cout << "rawShapleysInternal[ii]: " << rawShapleysInternal[ii] << std::endl;
					}
				}
			} else {
				shapleysInternal[0] = shapleysExternalGame[i];
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

		shapleysExternalGame[i] /= factorial(nbPartitions);

		// DEBUG
		std::cout << "shapleysExternalGame[i]: " << shapleysExternalGame[i] << std::endl;

		longUInt sumRawShapleysInternal = 0;
		for (longUInt x = 0; x < nbPlayersInParti; ++x) {
			sumRawShapleysInternal += rawShapleysInternal[x];
		}

		for (longUInt ii = 0; ii < nbPlayersInParti; ++ii) {
			owenIndices[player] = static_cast<double>(shapleysInternal[ii]) / (factorial(nbPlayersInParti) * factorial(nbPartitions));
			owenIndicesCheck[player] = static_cast<double>(shapleysExternalGame[i]) * rawShapleysInternal[ii] / sumRawShapleysInternal;

			// DEBUG
			std::cout << "player number: " << player << std::endl;
			std::cout << "Owen index of player: " << owenIndices[player] << std::endl;
			std::cout << "Check for Owen index of player: " << owenIndicesCheck[player] << std::endl;

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

	c(capitalC, n - 1) = 1;
	for (longUInt i = 0; i < n; ++i) {
		for (longUInt x = q + w[i]; x <= capitalC; ++x) {
			for (longUInt m = 1; m < n; ++m) {
				c(x - w[i], m - 1) = c(x, m) + c(x - w[i], m - 1);
			}
		}
	}
}
