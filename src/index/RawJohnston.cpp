#include "index/RawJohnston.h"

#include "Logging.h"
#include "lint/GlobalCalculator.h"

epic::index::RawJohnston::RawJohnston(Game& g) : PowerIndexWithWinningCoalitions() {
	// if a veto player exists then throw an error
	if (g.getNumberOfVetoPlayers() > 0) {
		throw std::invalid_argument("Veto player detected. The Johnston index only works without veto player. Please check your input values.");
	}
}

std::vector<epic::bigFloat> epic::index::RawJohnston::calculate(Game& g) {
	Array2d<longUInt> h;
	h.alloc(g.getNumberOfNonZeroPlayers(), g.getQuota(), true);
	subsetSumFromSmallestPerWeight(g, h);

	/*
	 * Preprocessing step 1: Collect sums later needed for surpluses
	 */
	auto max_weight = g.getWeights()[0];
	auto surplusSums = new longUInt[max_weight]();
	{ // initialize surplusSums
		longUInt hSum = 1;
		surplusSums[0] = hSum;

		for (longUInt l = 1; l < max_weight; ++l) {
			longUInt idx = findIndexInRev(g, l);

			if (idx < g.getNumberOfNonZeroPlayers()) {
				hSum += h(idx, l - 1);
			}

			surplusSums[l] = hSum;
		}
	}

	/*
	 * Preprocessing step 2: Collect sums later needed for deficiencies
	 */
	Array2d<longUInt> deficiencySums;
	deficiencySums.alloc(g.getNumberOfNonZeroPlayers(), g.getQuota(), true);

	for (longUInt deficiency = 0; deficiency < g.getQuota() - 1; ++deficiency) {
		for (longUInt j = 0; j < g.getNumberOfNonZeroPlayers(); ++j) {
			longUInt hSum = 0;
			auto wj = g.getWeights()[j];

			if (wj > 1.00001) {
				for (longUInt l = deficiency + 1; l <= wj + deficiency - 1; ++l) {
					// simplify loop with findIndexInRev() function
					longUInt idx = findIndexInRev(g, l - deficiency);
					if (idx < g.getNumberOfNonZeroPlayers() && l < g.getQuota()) {
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
	qmwcs.alloc(g.getNumberOfNonZeroPlayers(), g.getNumberOfNonZeroPlayers());
	gCalculator->allocInit_largeNumberArray(qmwcs.getArrayPointer(), qmwcs.getNumberOfElements());

	// Actually find Johnston indices for all players
	Array2d<lint::LargeNumber> interm;
	interm.alloc(g.getQuota(), g.getNumberOfNonZeroPlayers() - 1);
	gCalculator->allocInit_largeNumberArray(interm.getArrayPointer(), interm.getNumberOfElements());

	Array2d<lint::LargeNumber> forwardsArray;
	forwardsArray.alloc(g.getQuota(), g.getNumberOfNonZeroPlayers());
	gCalculator->allocInit_largeNumberArray(forwardsArray.getArrayPointer(), forwardsArray.getNumberOfElements());

	lint::LargeNumber tmp;
	gCalculator->allocInit_largeNumber(tmp);

	for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) { // All weights are greater than zero
		auto wi = g.getWeights()[i];

		longUInt deficiency = g.getQuota() - 1 - wi;

		if (wi > 1.00001) {
			gCalculator->plusEqual(qmwcs(i, 0), deficiencySums(i, deficiency));
		}

		if (i > 0) {
			forward_counting_per_weight_cardinality_next_step(g, forwardsArray, g.getWeights(), i - 1, (i == 1));

			lint::LargeNumber sum1, sum2;
			gCalculator->allocInit_largeNumber(sum1);
			gCalculator->allocInit_largeNumber(sum2);

			for (longUInt s = 2; s <= i + 1; ++s) {
				gCalculator->assign_zero(sum1);

				for (longUInt currentWeight = 1; currentWeight <= g.getQuota() - wi - 1; ++currentWeight) {
					deficiency = g.getQuota() - (currentWeight + wi) - 1;
					gCalculator->mul(tmp, forwardsArray(currentWeight - 1, s - 2), deficiencySums(i, deficiency));
					gCalculator->plusEqual(sum1, tmp);
				}
				gCalculator->plusEqual(qmwcs(i, s - 1), sum1);

				gCalculator->assign_zero(sum2);
				for (longUInt currentWeight = g.getQuota() - wi; currentWeight <= g.getQuota() - 1; ++currentWeight) {
					longUInt surplus = currentWeight + wi - g.getQuota();
					if (wi == 1) {
						gCalculator->plusEqual(sum2, forwardsArray(currentWeight - 1, s - 2));
					} else {
						gCalculator->mul(tmp, forwardsArray(currentWeight - 1, s - 2), surplusSums[wi - surplus - 1]);
						gCalculator->plusEqual(sum2, tmp);
					}
				}
				gCalculator->plusEqual(qmwcs(i, s - 1), sum2);
			}

			gCalculator->free_largeNumber(sum1);
			gCalculator->free_largeNumber(sum2);
		}

		// Second case: i not the smallest swing player in a coalition
		if (i < g.getNumberOfNonZeroPlayers() - 1) {
			std::vector<longUInt> w(g.getWeights().begin(), g.getWeights().end() - g.getNumberOfPlayersWithWeight0());
			w.erase(w.begin() + i);

			lint::LargeNumber sum1, sum2;
			gCalculator->allocInit_largeNumber(sum1);
			gCalculator->allocInit_largeNumber(sum2);

			for (longUInt j = i + 1; j < g.getNumberOfNonZeroPlayers(); ++j) {
				auto wj = g.getWeights()[j];

				if (j > 1) {
					forward_counting_per_weight_cardinality_next_step(g, interm, w, j - 2, (j == i + 1 || j == 2));
				}

				if (static_cast<longInt>(g.getQuota() - wi - wj) <= 0) {
					auto surplus = static_cast<longInt>(wi + wj - g.getQuota());
					gCalculator->plusEqual(qmwcs(i, 1), surplusSums[wj - surplus - 1]);
				} else {
					for (longUInt s = 1; s < g.getNumberOfNonZeroPlayers(); ++s) {
						gCalculator->assign_zero(sum1);
						for (longInt currentWeight = 0; currentWeight <= static_cast<longInt>(g.getQuota() - wi - wj - 1); ++currentWeight) {
							deficiency = g.getQuota() - (currentWeight + wi + wj) - 1;
							longUInt hSum = 0;

							if (wj > 1.00001) {
								hSum = deficiencySums(j, deficiency);
							}

							if (currentWeight == 0 && s == 1) {
								gCalculator->plusEqual(sum1, hSum);
							}

							if (s > 1 && currentWeight > 0.001) {
								gCalculator->mul(tmp, interm(currentWeight - 1, s - 2), hSum);
								gCalculator->plusEqual(sum1, tmp);
							}
						}
						gCalculator->plusEqual(qmwcs(i, s), sum1);

						gCalculator->assign_zero(sum2);
						for (longUInt currentWeight = g.getQuota() - wi - wj - 1; currentWeight < g.getQuota() - wi - 1; ++currentWeight) {
							longUInt surplus = 0;
							surplus = currentWeight + wi + wj - g.getQuota() + 1;

							if (wj == 1 && s > 1) {
								gCalculator->plusEqual(sum2, interm(currentWeight, s - 2));
							} else if (s > 1) {
								gCalculator->mul(tmp, interm(currentWeight, s - 2), surplusSums[wj - surplus - 1]);
								gCalculator->plusEqual(sum2, tmp);
							}
						}
						gCalculator->plusEqual(qmwcs(i, s), sum2);
					}
				}
			}

			gCalculator->free_largeNumber(sum1);
			gCalculator->free_largeNumber(sum2);
		}
	}

	delete[] surplusSums;
	deficiencySums.free();

	gCalculator->free_largeNumber(tmp);
	gCalculator->free_largeNumberArray(interm.getArrayPointer());
	interm.free();
	gCalculator->free_largeNumberArray(forwardsArray.getArrayPointer());
	forwardsArray.free();

	mpf_class big_float("0");

	bigInt tmp_int;
	bigInt nvc;

	/*
	Get vulnerable coalitions (aka quasiminimal winning coalitions)
	per weight and cardinality (of swing players in coalition)
	stored in matrix qmwcs
	*/

	std::vector<bigFloat> raw_j(g.getNumberOfPlayers());
	for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) {
		for (longUInt s = 0; s < g.getNumberOfNonZeroPlayers(); ++s) {
			gCalculator->to_bigInt(&tmp_int, qmwcs(i, s));
			big_float = tmp_int;
			nvc += tmp_int;
			big_float *= (1 / static_cast<double>(s + 1));

			raw_j[i] += big_float;
		}
	}
	for (longUInt i = g.getNumberOfNonZeroPlayers(); i < g.getNumberOfPlayers(); ++i) {
		raw_j[i] = 0;
	}

	gCalculator->free_largeNumberArray(qmwcs.getArrayPointer());
	qmwcs.free();

	log::out << log::info << "Total number of vulnerable coalitions: " << nvc << log::endl;

	return raw_j;
}

void epic::index::RawJohnston::subsetSumFromSmallestPerWeight(Game& g, Array2d<longUInt>& h) {
	auto&& w = g.getWeights();

	/*
	 * INITIALIZATION
	 */
	auto upper = new longUInt[g.getNumberOfNonZeroPlayers()]();
	{ // initialize upper-array
		upper[g.getNumberOfNonZeroPlayers() - 1] = w[g.getNumberOfNonZeroPlayers() - 1];
		longUInt j = g.getNumberOfNonZeroPlayers() - 1;

		// fill upper with the summed weights until it reaches quota
		do {
			--j;
			upper[j] = upper[j + 1] + w[j];
		} while (upper[j] < g.getQuota());

		// fill the rest with quota
		for (; j < g.getNumberOfNonZeroPlayers(); --j) { // j <= 0
			upper[j] = g.getQuota();
		}
	}

	/*
	 * PROCESSING
	 */
	{
		h(0, w[g.getNumberOfNonZeroPlayers() - 1] - 1) = 1;

		longUInt idx;
		for (longUInt i = g.getNumberOfNonZeroPlayers() - 2; i < g.getNumberOfNonZeroPlayers(); --i) { //  i <= 0
			idx = g.getNumberOfNonZeroPlayers() - i - 2;									   // previous dimension
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

epic::longUInt epic::index::RawJohnston::findIndexInRev(Game& g, longUInt weight) {
	longUInt idx = g.getNumberOfNonZeroPlayers();

	for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) {
		if (g.getWeights()[i] <= weight) {
			idx = g.getNumberOfNonZeroPlayers() - i - 1;
			break;
		}
	}

	return idx;
}

void epic::index::RawJohnston::forward_counting_per_weight_cardinality_next_step(Game& g, Array2d<lint::LargeNumber>& ret_ptr, std::vector<epic::longUInt> weights, longUInt player_limit, bool first_step) {
	/*
	 * INITIALIZATION
	 */
	longUInt player_count = (weights.size() < g.getNumberOfNonZeroPlayers()) ? weights.size() : g.getNumberOfNonZeroPlayers();
	longUInt upper_sum = 0;
	auto upper = new longUInt[player_count];

	{ // initialize upper-array
		for (longUInt j = 0; j < player_count; ++j) {
			upper_sum += weights[j];
			if (upper_sum < g.getQuota()) {
				upper[j] = upper_sum;
			} else {
				upper[j] = g.getQuota();
			}
		}
	}

	if (first_step) {
		for (longUInt i = 0; i < g.getQuota(); ++i) {
			for (longUInt j = 0; j < player_count; ++j) {
				gCalculator->assign_zero(ret_ptr(i, j));
			}
		}

		for (longUInt i = 0; i < player_limit; ++i) {
			longUInt wi = weights[i];

			for (longUInt x = upper[i] + 1; x > wi; --x) {
				if (x - wi - 1 == 0) {
					gCalculator->increment(ret_ptr(x - 2, 0));
				} else {
					for (longUInt s = 1; s <= i; ++s) {
						gCalculator->plusEqual(ret_ptr(x - 2, s), ret_ptr(x - wi - 2, s - 1));
					}
				}
			}
		}
	}

	longUInt w_limit = weights[player_limit];
	for (longUInt x = upper[player_limit] + 1; x > w_limit; --x) {
		if (x - w_limit - 1 == 0) {
			gCalculator->increment(ret_ptr(x - 2, 0));
		} else {
			for (longUInt s = 1; s <= player_limit; ++s) {
				gCalculator->plusEqual(ret_ptr(x - 2, s), ret_ptr(x - w_limit - 2, s - 1));
			}
		}
	}

	delete[] upper;
}

std::string epic::index::RawJohnston::getFullName() {
	return "RawJohnston";
}

epic::longUInt epic::index::RawJohnston::getMemoryRequirement(Game& g) {
	// h gets deleted before forwardsArray gets allocated. sizeof(forwardsArray) >= sizeof(h)
	bigInt memory = g.getWeights()[0] * c_sizeof_longUInt;									// surplusSums
	memory += g.getNumberOfNonZeroPlayers() * g.getQuota() * c_sizeof_longUInt;						// deficiencySums
	memory += g.getNumberOfNonZeroPlayers() * g.getNumberOfNonZeroPlayers() * gCalculator->getLargeNumberSize();	// qmwcs
	memory += g.getQuota() * (g.getNumberOfNonZeroPlayers() - 1) * gCalculator->getLargeNumberSize(); // interm
	memory += g.getQuota() * g.getNumberOfNonZeroPlayers() * gCalculator->getLargeNumberSize();		// forwardsArray
	memory += g.getNumberOfNonZeroPlayers() * c_sizeof_longUInt;											// forward_counting_per_weight_cardinality_next_step::upper
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

epic::bigInt epic::index::RawJohnston::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_numberOfWinningCoalitionsPerWeight();
}

epic::lint::Operation epic::index::RawJohnston::getOperationRequirement() {
	return lint::Operation::multiplication;
}
