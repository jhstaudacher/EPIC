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
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

std::vector<epic::bigFloat> epic::index::Owen::calculate() {
	longUInt nbPartitions = mGame.getPrecoalitions().size();
	auto partW = new longUInt[nbPartitions](); // partition weight (partW[i]: weight of partition i)
	longUInt maxMembersInPartition = 1;

	for (longUInt i = 0; i < nbPartitions; ++i) {
		for (longUInt p = 0; p < mGame.getPrecoalitions()[i].size(); ++p) {
			partW[i] += mGame.getWeights()[mGame.getPrecoalitions()[i][p]];
		}
		longUInt partSize = mGame.getPrecoalitions()[i].size();
		if (partSize > maxMembersInPartition) {
			maxMembersInPartition = partSize;
		}
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

	Array2dOffset<lint::LargeNumber> cc(mGame.getWeightSum() + 1, nbPartitions, mGame.getQuota(), 0);
	mCalculator->allocInit_largeNumberArray(cc.getArrayPointer(), cc.getNumberOfElements());
	backwardCountingPerWeightAndCardinality(cc, partW);

	Array2dOffset<lint::LargeNumber> cw(mGame.getWeightSum() + 1, nbPartitions, mGame.getQuota(), 0);
	mCalculator->allocInit_largeNumberArray(cw.getArrayPointer(), cw.getNumberOfElements());

	auto shapleysInternal = new lint::LargeNumber[mGame.getNumberOfPlayers()];
	mCalculator->allocInit_largeNumberArray(shapleysInternal, mGame.getNumberOfPlayers());

	Array2dOffset<lint::LargeNumber> cw2(mGame.getWeightSum() + 1, maxMembersInPartition, mGame.getQuota(), 0);
	mCalculator->allocInit_largeNumberArray(cw2.getArrayPointer(), cw2.getNumberOfElements());

	Array2dOffset<lint::LargeNumber> cwi(mGame.getWeightSum() + 1, maxMembersInPartition, mGame.getQuota(), 0);
	mCalculator->alloc_largeNumberArray(cwi.getArrayPointer(), cwi.getNumberOfElements());

	auto winternal = new longUInt[maxMembersInPartition];

	lint::LargeNumber tmp, tmp2;
	mCalculator->alloc_largeNumber(tmp);
	mCalculator->alloc_largeNumber(tmp2);
	lint::LargeNumber factor;
	mCalculator->alloc_largeNumber(factor);


	for (longUInt i = 0; i < nbPartitions; ++i) {
		// initialize cw
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

		longUInt nbPlayersInParti = mGame.getPrecoalitions()[i].size();

		// initialize winternal
		for (longUInt z = 0; z < nbPlayersInParti; ++z) {
			winternal[z] = mGame.getWeights()[mGame.getPrecoalitions()[i][z]];
		}

		for (longUInt s = 0; s < nbPartitions - 1; ++s) {
			mCalculator->mul(factor, factorial[s + 1], factorial[nbPartitions - s - 2]);

			if (nbPlayersInParti > 1) {
				for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
					for (longUInt y = 0; y < nbPlayersInParti - 1; ++y) {
						mCalculator->assign_zero(cw2(x, y));
					}
				}
				for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
					mCalculator->assign(cw2(x, nbPlayersInParti - 1), cw(x, s + 1));
				}

				for (longUInt ii = 0; ii < nbPlayersInParti; ++ii) {
					for (longUInt x = mGame.getQuota() + winternal[ii]; x <= mGame.getWeightSum(); ++x) {
						for (longUInt m = nbPlayersInParti - 1; m > 0; --m) {
							mCalculator->plus(cw2(x - winternal[ii], m - 1), cw2(x, m), cw2(x - winternal[ii], m - 1));
						}
					}
				}

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

					for (longUInt sinternal = 0; sinternal < nbPlayersInParti; ++sinternal) { // ! Using sinternal increased by 1
						mCalculator->mul(tmp, factorial[sinternal], factorial[nbPlayersInParti - sinternal - 1]);

						mCalculator->assign_zero(tmp2);
						for (longUInt w = mGame.getQuota(); w <= mGame.getQuota() + winternal[ii] - 1; ++w) {
							mCalculator->plusEqual(tmp2, cwi(w, sinternal));
						}

						// shapleysInternal[mGame.getPrecoalitions()[i][ii] - 1] += tmp * tmp_sum * factor
						mCalculator->mul(tmp, tmp, tmp2);
						mCalculator->mulEqual(tmp, factor);
						mCalculator->plusEqual(shapleysInternal[mGame.getPrecoalitions()[i][ii]], tmp);
					}
				}
				mCalculator->free_largeNumberArray(cwi.getArrayPointer());
			} else {
				// shapleysInternal[mGame.getPrecoalitions()[i][0] - 1] += factor * sum(cw[q:tmp_min, s+1])
				mCalculator->assign_zero(tmp2);
				longUInt tmp_min = std::min(mGame.getQuota() + partW[i] - 1, mGame.getWeightSum());
				for (longUInt z = mGame.getQuota(); z <= tmp_min; ++z) {
					mCalculator->plusEqual(tmp2, cw(z, s + 1));
				}
				mCalculator->mul(tmp, factor, tmp2);
				mCalculator->plusEqual(shapleysInternal[mGame.getPrecoalitions()[i][0]], tmp);
			}
		}
	}

	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	{
		bigInt shapleys_internal;
		bigInt factor;
		bigInt factorial_nbPartitions;
		mCalculator->to_bigInt(&factorial_nbPartitions, factorial[nbPartitions]);

		for (longUInt part = 0; part < nbPartitions; ++part) {
			longUInt nbPlayersInParti = mGame.getPrecoalitions()[part].size();
			mCalculator->to_bigInt(&factor, factorial[nbPlayersInParti]);
			factor *= factorial_nbPartitions;

			for (longUInt i = 0; i < nbPlayersInParti; ++i) {
				// solution[player] = shapleysInternal[player] / (factorial[nbPlayersInParti] * factorial[nbPartitions]);
				longUInt player = mGame.getPrecoalitions()[part][i];
				mCalculator->to_bigInt(&shapleys_internal, shapleysInternal[player]);
				solution[player] = bigFloat(shapleys_internal) / factor;
			}
		}
	}


	mCalculator->free_largeNumberArray(shapleysInternal);
	delete[] shapleysInternal;

	mCalculator->free_largeNumberArray(factorial);
	delete[] factorial;

	// DEBUG
	std::cout << "owenIndices:" << std::endl;
	for (longUInt x = 0; x < mGame.getNumberOfPlayers(); ++x) {
		std::cout << "[" << x << "]\t" << solution[x] << std::endl;
	}

	mCalculator->free_largeNumber(tmp);
	mCalculator->free_largeNumber(tmp2);
	mCalculator->free_largeNumber(factor);

	delete[] partW;
	mCalculator->free_largeNumberArray(cc.getArrayPointer());
	mCalculator->free_largeNumberArray(cw.getArrayPointer());
	mCalculator->free_largeNumberArray(cw2.getArrayPointer());
	mCalculator->free_largeNumberArray(cwi.getArrayPointer());
	delete[] winternal;

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
