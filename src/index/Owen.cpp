#include "index/Owen.h"

#include "Array.h"

#include <numeric>

// DEBUG
#include <iostream>

epic::index::Owen::Owen(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: ItfPowerIndex(g) {
	bigInt max_value = approx->upperBound_swingPlayerPerCardinality();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::multiplication, int_representation);
}

epic::index::Owen::~Owen() {

}

std::vector<epic::bigFloat> epic::index::Owen::calculate() {
	longUInt nbPartitions = mGame.getPrecoalitions().size();
	auto partW = new longUInt[nbPartitions](); // partition weight (partW[i]: weight of partition i)
	longUInt maxMembersInPartition = 1;

	for (longUInt i = 0; i < nbPartitions; ++i) {
		for (longUInt p = 0; p < mGame.getPrecoalitions()[i].size(); ++p) {
			partW[i] += mGame.getWeights()[mGame.getPrecoalitions()[i][p] - 1];
		}
		longUInt partSize = mGame.getPrecoalitions()[i].size();
		if (partSize > maxMembersInPartition) {
			maxMembersInPartition = partSize;
		}
	}

	auto big_shapleysExternalGame = new bigFloat[nbPartitions]();
	auto shapleysExternalGame = new lint::LargeNumber[nbPartitions];
	mCalculator->allocInit_largeNumberArray(shapleysExternalGame, nbPartitions);

	auto owenIndices = new bigFloat[mGame.getNumberOfPlayers()]();
	auto owenIndicesCheck = new bigFloat[mGame.getNumberOfPlayers()]();

	Array2dOffset<lint::LargeNumber> cc(mGame.getWeightSum() + 1, nbPartitions, mGame.getQuota(), 0);
	mCalculator->allocInit_largeNumberArray(cc.getArrayPointer(), cc.getNumberOfElements());
	backwardCountingPerWeightAndCardinality(cc, partW);

	Array2dOffset<lint::LargeNumber> cw(mGame.getWeightSum() + 1, nbPartitions, mGame.getQuota(), 0);
	mCalculator->allocInit_largeNumberArray(cw.getArrayPointer(), cw.getNumberOfElements());
	for (longUInt w = mGame.getQuota(); w <= mGame.getWeightSum(); ++w) {
		for (longUInt c = 0; c < nbPartitions; ++c) {
			mCalculator->assign(cw(w, c), cc(w, c));
		}
	}

	// DEBUG
	std::cout << "cw untouched:" << std::endl;
	for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
		std::cout << "[" << x << ", ]";
		for (longUInt y = 0; y < nbPartitions; ++y) {
			std::cout << "\t" << mCalculator->to_string(cw(x, y));
		}
		std::cout << std::endl;
	}

	lint::LargeNumber* factorial;
	{
		longUInt size = std::max(maxMembersInPartition, nbPartitions);
		factorial = new lint::LargeNumber[size + 1];
		mCalculator->alloc_largeNumberArray(factorial, size);

		mCalculator->assign_one(factorial[0]);
		mCalculator->assign_one(factorial[1]);
		for (longUInt i = 2; i <= size; ++i) {
			mCalculator->mul(factorial[i], factorial[i - 1], i);
		}
	}

	lint::LargeNumber tmp;
	mCalculator->alloc_largeNumber(tmp);

	longUInt player = 0;
	for (longUInt i = 0; i < nbPartitions; ++i) {
		for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
			for (longUInt z = 0; z < nbPartitions; ++z) {
				mCalculator->assign(cw(x, z), cc(x, z));
			}
		}

		if (mGame.getWeightSum() - partW[i] >= mGame.getQuota()) {
			for (longUInt x = mGame.getWeightSum() - partW[i]; x >= mGame.getQuota(); --x) {
				for (longUInt s = 1; s < nbPartitions; ++s) {
					mCalculator->minus(cw(x, nbPartitions - s - 1), cc(x, nbPartitions - s - 1), cw(x + partW[i], nbPartitions - s));
				}
			}
		}

		// DEBUG
		std::cout << "i: " << i << std::endl;
		std::cout << "cw:" << std::endl;
		for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
			std::cout << "[" << x << ", ]";
			for (longUInt y = 0; y < nbPartitions; ++y) {
				std::cout << "\t" << mCalculator->to_string(cw(x, y));
			}
			std::cout << std::endl;
		}

		longUInt nbPlayersInParti = mGame.getPrecoalitions()[i].size();
		Array2dOffset<lint::LargeNumber> cw2(mGame.getWeightSum() + 1, nbPlayersInParti, mGame.getQuota(), 0);
		mCalculator->allocInit_largeNumberArray(cw2.getArrayPointer(), cw2.getNumberOfElements());

		auto winternal = new longUInt[nbPlayersInParti];
		for (longUInt z = 0; z < nbPlayersInParti; ++z) {
			winternal[z] = mGame.getWeights()[mGame.getPrecoalitions()[i][z]];
		}

		auto shapleysInternal = new lint::LargeNumber[nbPlayersInParti];
		mCalculator->allocInit_largeNumberArray(shapleysInternal, nbPlayersInParti);

		auto rawShapleysInternal = new lint::LargeNumber[nbPlayersInParti];
		mCalculator->allocInit_largeNumberArray(rawShapleysInternal, nbPlayersInParti);

		for (longUInt s = 0; s < nbPartitions - 1; ++s) {
			lint::LargeNumber factor;
			mCalculator->alloc_largeNumber(factor);
			mCalculator->mul(factor, factorial[s + 1], factorial[nbPartitions - s - 2]);

			lint::LargeNumber tmp_sum;
			mCalculator->allocInit_largeNumber(tmp_sum);
			longUInt tmp_min = std::min(mGame.getQuota() + partW[i] - 1, mGame.getWeightSum());
			for (longUInt z = mGame.getQuota(); z <= tmp_min; ++z) {
				mCalculator->plusEqual(tmp_sum, cw(z, s + 1));
			}
			mCalculator->mul(tmp, factor, tmp_sum);
			mCalculator->plusEqual(shapleysExternalGame[i], tmp);
			mCalculator->free_largeNumber(tmp_sum);

			if (nbPlayersInParti > 1) {
				for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
					for (longUInt y = 0; y < nbPlayersInParti - 1; ++y) {
						mCalculator->assign_zero(cw2(x, y));
					}
				}
				for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
					mCalculator->assign(cw2(x, nbPlayersInParti - 1), cw(x, s + 1));
				}

				// DEBUG
				std::cout << "s+1 -- with s being index from loop over number of precoalitions: " << s+1 << std::endl;
				std::cout << "cw2[q:capitalC,nbPlayersInParti-1]" << std::endl;
				for (longUInt z = mGame.getQuota(); z <= mGame.getWeightSum(); ++z) {
					std::cout << "[" << z << ", " << nbPlayersInParti-1 << "] = " << mCalculator->to_string(cw2(z, nbPlayersInParti-1)) << std::endl;
				}

				for (longUInt ii = 0; ii < nbPlayersInParti; ++ii) {
					for (longUInt x = mGame.getQuota() + winternal[ii]; x <= mGame.getWeightSum(); ++x) {
						for (longUInt m = nbPlayersInParti - 1; m > 0; --m) {
							mCalculator->plus(cw2(x - winternal[ii], m - 1), cw2(x, m), cw2(x - winternal[ii], m - 1));
						}
					}
				}

				// DEBUG
				std::cout << "cw2:" << std::endl;
				for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
					std::cout << "[" << x << ", ]";
					for (longUInt y = 0; y < nbPlayersInParti; ++y) {
						std::cout << "\t" << mCalculator->to_string(cw2(x, y));
					}
					std::cout << std::endl;
				}

				Array2dOffset<lint::LargeNumber> cwi(mGame.getWeightSum() + 1, nbPlayersInParti, mGame.getQuota(), 0);
				mCalculator->alloc_largeNumberArray(cwi.getArrayPointer(), cwi.getNumberOfElements());
				for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
					for (longUInt y = 0; y < nbPlayersInParti; ++y) {
						mCalculator->assign(cwi(x, y), cw2(x, y));
					}
				}
				for (longUInt ii = 0; ii < nbPlayersInParti; ++ii) {
					for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
						for (longUInt y = 0; y < nbPlayersInParti; ++y) {
							mCalculator->assign(cwi(x, y), cw2(x, y));
						}
					}
					if (mGame.getWeightSum() - winternal[ii] >= mGame.getQuota()) {
						for (longUInt x = mGame.getWeightSum() - winternal[ii]; x >= mGame.getQuota(); --x) {
							for (longUInt sinternal = 1; sinternal < nbPlayersInParti; ++sinternal) {
								mCalculator->minus(cwi(x, nbPlayersInParti - sinternal - 1), cw2(x, nbPlayersInParti - sinternal - 1), cwi(x + winternal[ii], nbPlayersInParti - sinternal));
							}
						}
					}

					// DEBUG
					std::cout << "cwi:" << std::endl;
					for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
						std::cout << "[" << x << ", ]";
						for (longUInt y = 0; y < nbPlayersInParti; ++y) {
							std::cout << "\t" << mCalculator->to_string(cwi(x, y));
						}
						std::cout << std::endl;
					}

					for (longUInt sinternal = 0; sinternal < nbPlayersInParti; ++sinternal) { // ! Using sinternal increased by 1
						mCalculator->mul(tmp, factorial[sinternal], factorial[nbPlayersInParti - sinternal - 1]);

						lint::LargeNumber tmp_sum;
						mCalculator->allocInit_largeNumber(tmp_sum);
						for (longUInt w = mGame.getQuota(); w <= mGame.getQuota() + winternal[ii] - 1; ++w) {
							mCalculator->plusEqual(tmp_sum, cwi(w, sinternal));
						}

						// shapleysInternal[ii] += tmp * tmp_sum * factor
						mCalculator->mul(tmp, tmp, tmp_sum);
						mCalculator->mulEqual(tmp, factor);
						mCalculator->plusEqual(shapleysInternal[ii], tmp);

						mCalculator->free_largeNumber(tmp_sum);

						mCalculator->assign(rawShapleysInternal[ii], shapleysInternal[ii]);

						// DEBUG
						std::cout << "rawShapleysInternal[ii]: " << mCalculator->to_string(rawShapleysInternal[ii]) << std::endl;
					}
				}
				mCalculator->free_largeNumberArray(cwi.getArrayPointer());
			} else {
				mCalculator->assign(shapleysInternal[0], shapleysExternalGame[i]);
				mCalculator->assign(rawShapleysInternal[0], shapleysInternal[0]);
			}

			// DEBUG
			std::cout << "s: " << s << std::endl;
			std::cout << "rawShapleysInternal: " << std::endl;
			for (longUInt z = 0; z < nbPlayersInParti; ++z) {
				std::cout << "[" << z << "]\t" << mCalculator->to_string(rawShapleysInternal[z]) << std::endl;
			}

			mCalculator->free_largeNumber(factor);
			mCalculator->free_largeNumberArray(cw2.getArrayPointer());
		}

		// DEBUG
		std::cout << "i (after external Shapley loop for precoalitions): " << i << std::endl;

		std::cout << "shapleysExternalGame[i] = " << mCalculator->to_string(shapleysExternalGame[i]) << std::endl;
		std::cout << "factorial[nbPartitions] = " << mCalculator->to_string(factorial[nbPartitions]) << std::endl;

		// shapleysExternalGame[i] /= factorial(nbPartitions);
		bigInt big1, big2;
		mCalculator->to_bigInt(&big1, shapleysExternalGame[i]);
		mCalculator->to_bigInt(&big2, factorial[nbPartitions]);
		big_shapleysExternalGame[i] = bigFloat(big1) / big2;

		// DEBUG
		std::cout << "shapleysExternalGame[i]: " << big_shapleysExternalGame[i] << std::endl;

		lint::LargeNumber sumRawShapleysInternal;
		mCalculator->allocInit_largeNumber(sumRawShapleysInternal);
		for (longUInt x = 0; x < nbPlayersInParti; ++x) {
			mCalculator->plusEqual(sumRawShapleysInternal, rawShapleysInternal[x]);
		}

		for (longUInt ii = 0; ii < nbPlayersInParti; ++ii) {
			// owenIndices[player] = shapleysInternal[ii] / (factorial[nbPlayersInParti] * factorial[nbPartitions]);
			mCalculator->to_bigInt(&big1, factorial[nbPlayersInParti]);
			mCalculator->to_bigInt(&big2, factorial[nbPartitions]);
			big1 *= big2;
			mCalculator->to_bigInt(&big2, shapleysInternal[ii]);
			owenIndices[player] = bigFloat(big2) / big1;

			// owenIndicesCheck[player] = shapleysExternalGame[i] * rawShapleysInternal[ii] / sumRawShapleysInternal
			mCalculator->to_bigInt(&big1, rawShapleysInternal[ii]);
			mCalculator->to_bigInt(&big2, sumRawShapleysInternal);
			owenIndicesCheck[player] = big_shapleysExternalGame[i] * big1;
			owenIndicesCheck[player] /= big2;

			// DEBUG
			std::cout << "player number: " << player << std::endl;
			std::cout << "Owen index of player: " << owenIndices[player] << std::endl;
			std::cout << "Check for Owen index of player: " << owenIndicesCheck[player] << std::endl;

			++player;
		}

		mCalculator->free_largeNumber(sumRawShapleysInternal);
		mCalculator->free_largeNumberArray(rawShapleysInternal);
		delete[] rawShapleysInternal;
		mCalculator->free_largeNumberArray(shapleysInternal);
		delete[] shapleysInternal;
		delete[] winternal;
	}

	mCalculator->free_largeNumberArray(factorial);
	delete[] factorial;

	// DEBUG
	std::cout << "shapleysExternalGame:" << std::endl;
	for (longUInt x = 0; x < nbPartitions; ++x) {
		std::cout << "[" << x << "]\t" << big_shapleysExternalGame[x] << std::endl;
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

	mCalculator->free_largeNumber(tmp);

	delete[] partW;
	mCalculator->free_largeNumberArray(cc.getArrayPointer());
	mCalculator->free_largeNumberArray(cw.getArrayPointer());

	delete[] big_shapleysExternalGame;
	mCalculator->free_largeNumberArray(shapleysExternalGame);
	delete[] shapleysExternalGame;

	delete[] owenIndices;
	delete[] owenIndicesCheck;


	return solution;
}

std::string epic::index::Owen::getFullName() {
	return "Owen";
}

epic::longUInt epic::index::Owen::getMemoryRequirement() {
	return 0;
}

// ! c must be allocated and zero-initialized in the range [quota, capitalC] x [0, nbPartitions - 1]
void epic::index::Owen::backwardCountingPerWeightAndCardinality(Array2dOffset<lint::LargeNumber>& c, longUInt* partW) {
	longUInt n = mGame.getPrecoalitions().size();

	mCalculator->assign_one(c(mGame.getWeightSum(), n -1));
	for (longUInt i = 0; i < n; ++i) {
		for (longUInt x = mGame.getQuota() + partW[i]; x <= mGame.getWeightSum(); ++x) {
			for (longUInt m = 1; m < n; ++m) {
				mCalculator->plus(c(x - partW[i], m - 1), c(x, m), c(x - partW[i], m - 1));
			}
		}
	}
}
