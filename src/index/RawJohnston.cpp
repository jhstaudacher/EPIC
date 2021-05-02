#include "index/RawJohnston.h"

#include "Logging.h"

epic::index::RawJohnston::RawJohnston(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: PowerIndexWithWinningCoalitions(g) {
	bigInt max_value = approx->upperBound_numberOfWinningCoalitionsPerWeight();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::multiplication, int_representation);

	// if a veto player exists then throw an error
	if (mGame.getNumberOfVetoPlayers() > 0) {
		throw std::invalid_argument("Veto player detected. The Johnston index only works without veto player. Please check your input values.");
	}
}

epic::index::RawJohnston::~RawJohnston() {
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

std::vector<epic::bigFloat> epic::index::RawJohnston::calculate() {
	Array2d<longUInt> h;
	h.alloc(mNonZeroPlayerCount, mGame.getQuota(), true);
	subsetSumFromSmallestPerWeight(h);

	/*
	 * Preprocessing step 1: Collect sums later needed for surpluses
	 */
	auto max_weight = mGame.getWeights()[0];
	auto surplusSums = new longUInt[max_weight]();
	{ // initialize surplusSums
		longUInt hSum = 1;
		surplusSums[0] = hSum;

		for (longUInt l = 1; l < max_weight; ++l) {
			longUInt idx = findIndexInRev(l);

			if (idx < mNonZeroPlayerCount) {
				hSum += h(idx, l - 1);
			}

			surplusSums[l] = hSum;
		}
	}

	/*
	 * Preprocessing step 2: Collect sums later needed for deficiencies
	 */
	Array2d<longUInt> deficiencySums;
	deficiencySums.alloc(mNonZeroPlayerCount, mGame.getQuota(), true);

	for (longUInt deficiency = 0; deficiency < mGame.getQuota() - 1; ++deficiency) {
		for (longUInt j = 0; j < mNonZeroPlayerCount; ++j) {
			longUInt hSum = 0;
			auto wj = mGame.getWeights()[j];

			if (wj > 1.00001) {
				for (longUInt l = deficiency + 1; l <= wj + deficiency - 1; ++l) {
					// simplify loop with findIndexInRev() function
					longUInt idx = findIndexInRev(l - deficiency);
					if (idx < mNonZeroPlayerCount && l < mGame.getQuota()) {
						hSum += h(idx, l);
					}
				}
			}
			deficiencySums(j, deficiency) = hSum;
		}
	}

	h.free();

	/*
	 * Main Processing
	 */
	Array2d<lint::LargeNumber> qmwcs;
	qmwcs.alloc(mNonZeroPlayerCount, mNonZeroPlayerCount);
	mCalculator->allocInit_largeNumberArray(qmwcs.getArrayPointer(), qmwcs.getNumberOfElements());

	// Actually find Johnston indices for all players
	Array2d<lint::LargeNumber> interm;
	interm.alloc(mGame.getQuota(), mNonZeroPlayerCount - 1);
	mCalculator->allocInit_largeNumberArray(interm.getArrayPointer(), interm.getNumberOfElements());

	Array2d<lint::LargeNumber> forwardsArray;
	forwardsArray.alloc(mGame.getQuota(), mNonZeroPlayerCount);
	mCalculator->allocInit_largeNumberArray(forwardsArray.getArrayPointer(), forwardsArray.getNumberOfElements());

	lint::LargeNumber tmp;
	mCalculator->allocInit_largeNumber(tmp);

	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) { // All weights are greater than zero
		auto wi = mGame.getWeights()[i];

		longUInt deficiency = mGame.getQuota() - 1 - wi;

		if (wi > 1.00001) {
			mCalculator->plusEqual(qmwcs(i, 0), deficiencySums(i, deficiency));
		}

		if (i > 0) {
			forward_counting_per_weight_cardinality_next_step(forwardsArray, mGame.getWeights(), i - 1, (i == 1));

			lint::LargeNumber sum1, sum2;
			mCalculator->allocInit_largeNumber(sum1);
			mCalculator->allocInit_largeNumber(sum2);

			for (longUInt s = 2; s <= i + 1; ++s) {
				mCalculator->assign_zero(sum1);

				for (longUInt currentWeight = 1; currentWeight <= mGame.getQuota() - wi - 1; ++currentWeight) {
					deficiency = mGame.getQuota() - (currentWeight + wi) - 1;
					mCalculator->mul(tmp, forwardsArray(currentWeight - 1, s - 2), deficiencySums(i, deficiency));
					mCalculator->plusEqual(sum1, tmp);
				}
				mCalculator->plusEqual(qmwcs(i, s - 1), sum1);

				mCalculator->assign_zero(sum2);
				for (longUInt currentWeight = mGame.getQuota() - wi; currentWeight <= mGame.getQuota() - 1; ++currentWeight) {
					longUInt surplus = currentWeight + wi - mGame.getQuota();
					if (wi == 1) {
						mCalculator->plusEqual(sum2, forwardsArray(currentWeight - 1, s - 2));
					} else {
						mCalculator->mul(tmp, forwardsArray(currentWeight - 1, s - 2), surplusSums[wi - surplus - 1]);
						mCalculator->plusEqual(sum2, tmp);
					}
				}
				mCalculator->plusEqual(qmwcs(i, s - 1), sum2);
			}

			mCalculator->free_largeNumber(sum1);
			mCalculator->free_largeNumber(sum2);
		}

		// Second case: i not the smallest swing player in a coalition
		if (i < mNonZeroPlayerCount - 1) {
			std::vector<longUInt> w(mGame.getWeights().begin(), mGame.getWeights().end() - mGame.getNumberOfPlayersWithWeight0());
			w.erase(w.begin() + i);

			lint::LargeNumber sum1, sum2;
			mCalculator->allocInit_largeNumber(sum1);
			mCalculator->allocInit_largeNumber(sum2);

			for (longUInt j = i + 1; j < mNonZeroPlayerCount; ++j) {
				auto wj = mGame.getWeights()[j];

				if (j > 1) {
					forward_counting_per_weight_cardinality_next_step(interm, w, j - 2, (j == i + 1 || j == 2));
				}

				if (static_cast<longInt>(mGame.getQuota() - wi - wj) <= 0) {
					auto surplus = static_cast<longInt>(wi + wj - mGame.getQuota());
					mCalculator->plusEqual(qmwcs(i, 1), surplusSums[wj - surplus - 1]);
				} else {
					for (longUInt s = 1; s < mNonZeroPlayerCount; ++s) {
						mCalculator->assign_zero(sum1);
						for (longInt currentWeight = 0; currentWeight <= static_cast<longInt>(mGame.getQuota() - wi - wj - 1); ++currentWeight) {
							deficiency = mGame.getQuota() - (currentWeight + wi + wj) - 1;
							longUInt hSum = 0;

							if (wj > 1.00001) {
								hSum = deficiencySums(j, deficiency);
							}

							if (currentWeight == 0 && s == 1) {
								mCalculator->plusEqual(sum1, hSum);
							}

							if (s > 1 && currentWeight > 0.001) {
								mCalculator->mul(tmp, interm(currentWeight - 1, s - 2), hSum);
								mCalculator->plusEqual(sum1, tmp);
							}
						}
						mCalculator->plusEqual(qmwcs(i, s), sum1);

						mCalculator->assign_zero(sum2);
						for (longUInt currentWeight = mGame.getQuota() - wi - wj - 1; currentWeight < mGame.getQuota() - wi - 1; ++currentWeight) {
							longUInt surplus = 0;
							surplus = currentWeight + wi + wj - mGame.getQuota() + 1;

							if (wj == 1 && s > 1) {
								mCalculator->plusEqual(sum2, interm(currentWeight, s - 2));
							} else if (s > 1) {
								mCalculator->mul(tmp, interm(currentWeight, s - 2), surplusSums[wj - surplus - 1]);
								mCalculator->plusEqual(sum2, tmp);
							}
						}
						mCalculator->plusEqual(qmwcs(i, s), sum2);
					}
				}
			}

			mCalculator->free_largeNumber(sum1);
			mCalculator->free_largeNumber(sum2);
		}
	}

	delete[] surplusSums;
	deficiencySums.free();

	mCalculator->free_largeNumber(tmp);
	mCalculator->free_largeNumberArray(interm.getArrayPointer());
	interm.free();
	mCalculator->free_largeNumberArray(forwardsArray.getArrayPointer());
	forwardsArray.free();

	mpf_class big_float("0");

	bigInt tmp_int;
	bigInt nvc;

	/*
	Get vulnerable coalitions (aka quasiminimal winning coalitions)
	per weight and cardinality (of swing players in coalition)
	stored in matrix qmwcs
	*/

	std::vector<bigFloat> raw_j(mGame.getNumberOfPlayers());
	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
		for (longUInt s = 0; s < mNonZeroPlayerCount; ++s) {
			mCalculator->to_bigInt(&tmp_int, qmwcs(i, s));
			big_float = tmp_int;
			nvc += tmp_int;
			big_float *= (1 / static_cast<double>(s + 1));

			raw_j[i] += big_float;
		}
	}
	for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) {
		raw_j[i] = 0;
	}

	mCalculator->free_largeNumberArray(qmwcs.getArrayPointer());
	qmwcs.free();

	log::out << log::info << "Total number of vulnerable coalitions: " << nvc << log::endl;

	return raw_j;
}

void epic::index::RawJohnston::subsetSumFromSmallestPerWeight(Array2d<longUInt>& h) {
	auto&& w = mGame.getWeights();

	/*
	 * INITIALIZATION
	 */
	auto upper = new longUInt[mNonZeroPlayerCount]();
	{ // initialize upper-array
		upper[mNonZeroPlayerCount - 1] = w[mNonZeroPlayerCount - 1];
		longUInt j = mNonZeroPlayerCount - 1;

		// fill upper with the summed weights until it reaches quota
		do {
			--j;
			upper[j] = upper[j + 1] + w[j];
		} while (upper[j] < mGame.getQuota());

		// fill the rest with quota
		for (; j < mNonZeroPlayerCount; --j) { // j <= 0
			upper[j] = mGame.getQuota();
		}
	}

	/*
	 * PROCESSING
	 */
	{
		h(0, w[mNonZeroPlayerCount - 1] - 1) = 1;

		longUInt idx;
		for (longUInt i = mNonZeroPlayerCount - 2; i < mNonZeroPlayerCount; --i) { //  i <= 0
			idx = mNonZeroPlayerCount - i - 2;									   // previous dimension
			for (longUInt x = upper[i]; x > w[i]; --x) {
				h(idx + 1, x - 1) = h(idx, x - 1) + h(idx, x - w[i] - 1);
			}
			h(idx + 1, w[i] - 1) = h(idx, w[i] - 1) + 1;

			// copy the unchanged values from the previous to the current dimension
			for (longUInt x = w[i] - 2; x < w[i]; --x) { //  x >= 0
				h(idx + 1, x) = h(idx, x);
			}
		}
	}

	/*
	 * DELETE
	 */
	delete[] upper;
}

epic::longUInt epic::index::RawJohnston::findIndexInRev(longUInt weight) {
	longUInt idx = mNonZeroPlayerCount;

	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
		if (mGame.getWeights()[i] <= weight) {
			idx = mNonZeroPlayerCount - i - 1;
			break;
		}
	}

	return idx;
}

void epic::index::RawJohnston::forward_counting_per_weight_cardinality_next_step(Array2d<lint::LargeNumber>& ret_ptr, std::vector<epic::longUInt> weights, longUInt player_limit, bool first_step) {
	/*
	 * INITIALIZATION
	 */
	longUInt player_count = (weights.size() < mNonZeroPlayerCount) ? weights.size() : mNonZeroPlayerCount;
	longUInt upper_sum = 0;
	auto upper = new longUInt[player_count];

	{ // initialize upper-array
		for (longUInt j = 0; j < player_count; ++j) {
			upper_sum += weights[j];
			if (upper_sum < mGame.getQuota()) {
				upper[j] = upper_sum;
			} else {
				upper[j] = mGame.getQuota();
			}
		}
	}

	if (first_step) {
		for (longUInt i = 0; i < mGame.getQuota(); ++i) {
			for (longUInt j = 0; j < player_count; ++j) {
				mCalculator->assign_zero(ret_ptr(i, j));
			}
		}

		for (longUInt i = 0; i < player_limit; ++i) {
			longUInt wi = weights[i];

			for (longUInt x = upper[i] + 1; x > wi; --x) {
				if (x - wi - 1 == 0) {
					mCalculator->increment(ret_ptr(x - 2, 0));
				} else {
					for (longUInt s = 1; s <= i; ++s) {
						mCalculator->plusEqual(ret_ptr(x - 2, s), ret_ptr(x - wi - 2, s - 1));
					}
				}
			}
		}
	}

	longUInt w_limit = weights[player_limit];
	for (longUInt x = upper[player_limit] + 1; x > w_limit; --x) {
		if (x - w_limit - 1 == 0) {
			mCalculator->increment(ret_ptr(x - 2, 0));
		} else {
			for (longUInt s = 1; s <= player_limit; ++s) {
				mCalculator->plusEqual(ret_ptr(x - 2, s), ret_ptr(x - w_limit - 2, s - 1));
			}
		}
	}

	delete[] upper;
}

std::string epic::index::RawJohnston::getFullName() {
	return "RawJohnston";
}

epic::longUInt epic::index::RawJohnston::getMemoryRequirement() {
	// h gets deleted before forwardsArray gets allocated. sizeof(forwardsArray) >= sizeof(h)
	bigInt memory = mGame.getWeights()[0] * c_sizeof_longUInt;									// surplusSums
	memory += mNonZeroPlayerCount * mGame.getQuota() * c_sizeof_longUInt;						// deficiencySums
	memory += mNonZeroPlayerCount * mNonZeroPlayerCount * mCalculator->getLargeNumberSize();	// qmwcs
	memory += mGame.getQuota() * (mNonZeroPlayerCount - 1) * mCalculator->getLargeNumberSize(); // interm
	memory += mGame.getQuota() * mNonZeroPlayerCount * mCalculator->getLargeNumberSize();		// forwardsArray
	memory += mNonZeroPlayerCount * c_sizeof_longUInt;											// forward_counting_per_weight_cardinality_next_step::upper
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}
