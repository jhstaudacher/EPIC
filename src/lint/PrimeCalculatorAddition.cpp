#include "lint/PrimeCalculatorAddition.h"

#include <vector>

/**
 * @todo If more primes are needed than statically pre-computed at compilation time, a new array will be allocated each time the function get called. Those arrays will never be deleted. Since only one index for only one problem gets calculated each time the program runs, this has no negative effect.
 */
epic::lint::PrimesInfo epic::lint::PrimeCalculatorAddition::getPrimes(bigInt max_value) {
	size_t n_primes = 0;
	bigInt max_val = bigInt("1");

	for (size_t i = 0; i < mNPrecPrimes; ++i) {
		max_val *= bigInt(mPrecPrimes.array[i]);
		if (max_val > max_value) {
			n_primes = i + 1;
			break;
		}
	}

	// not enough precalculated primes, calculate as much as needed
	if (n_primes == 0) {
		// copy current primes to vector
		std::vector<longUInt> primes(mNPrecPrimes);
		for (size_t i = 0; i < mNPrecPrimes; ++i) {
			primes[i] = mPrecPrimes.array[i];
		}

		max_val = ItfPrimeCalculator::calculate_primes(primes, max_value);
		n_primes = primes.size();

		auto ret_ptr = new longUInt[n_primes];

		for (size_t i = 0; i < n_primes; ++i) {
			ret_ptr[i] = primes[i];
		}

		return PrimesInfo{ret_ptr, n_primes, max_val};
	} else {
		return PrimesInfo{mPrecPrimes.array, n_primes, max_val};
	}
}
