#ifndef EPIC_LINT_ITFPRIMECALCULATOR_H_
#define EPIC_LINT_ITFPRIMECALCULATOR_H_

#include "LargeNumber.h"
#include "types.h"

#include <array>
#include <cmath>
#include <numeric>
#include <vector>

namespace epic::lint {

/**
 * A Structure containing the basic Primes information
 * 
 * It contains the primes and their amount as well as the largest representable number + 1 which is the product over all primes
 */
struct PrimesInfo {
	const longUInt* primesArray;
	const size_t primesAmount;
	const bigInt maxValue;
};

/**
 * A Wrapper class for a C-Style array of a static size
 * 
 * This class is needed for the calculation of arrays at compile time using constexpr
 */
template<class type, size_t size> struct StaticArray {
	type array[size];
};

/**
 * A Interface class for calculating primes
 */
class ItfPrimeCalculator {
public:
	virtual ~ItfPrimeCalculator() = default;

	/**
	 * Returning the Primes that are needed to represent max_value using the Chinese Remainder Theorem.
	 * 
	 * @param max_value the largest value that should be representable by the primes
	 * 
	 * @return The PrimesInfo containing the exact number of primes needed for the Chinese Remainder Theorem
	 */
	virtual PrimesInfo getPrimes(bigInt max_value) = 0;

	/**
	 * A function to precalculate a certain number of co-prime numbers at compile time
	 * 
	 * @tparam n_primes the number of primes to calculate (corresponds to the number of array elements
	 * @param largest_prime defines the largest co-primes where the calculation starts and continues downwards
	 * 
	 * @return A StaticArray containing the calculated co-primes
	 */
	template<size_t n_primes> static constexpr StaticArray<longUInt, n_primes> precalculate_primes(longUInt largest_prime) {
		auto coprimes = StaticArray<longUInt, n_primes>();

		coprimes.array[0] = largest_prime;
		for (size_t i = 1; i < n_primes; ++i) {
			longUInt tmp = coprimes.array[i - 1];
			bool pairwise_coprime = true;

			do {
				--tmp;
				pairwise_coprime = true;
				for (size_t k = 0; k < i; ++k) {
					if (std::gcd<longUInt, longUInt>(coprimes.array[k], tmp) != 1) {
						pairwise_coprime = false;
						break;
					}
				}
			} while (!pairwise_coprime);

			coprimes.array[i] = tmp;
		}

		return coprimes;
	}

	/**
	 * Calculates Co-Primes based on a set of already Co-Prime values.
	 *
	 * @param primes A Set of co-primes.
	 * @param max_value The largest value that should be representable.
	 *
	 * @return The product of all co-primes (= largest possible value - 1)
	 */
	static bigInt calculate_primes(std::vector<longUInt>& primes, const bigInt& max_value) {
		bigInt prod("1");
		for (auto it : primes) {
			prod *= it;
		}

		while (prod <= max_value) {
			longUInt tmp = primes.back();
			bool pairwise_coprime = true;

			do {
				--tmp;
				pairwise_coprime = true;
				for (auto it : primes) {
					if (std::gcd<longUInt, longUInt>(it, tmp) != 1) {
						pairwise_coprime = false;
						break;
					}
				}
			} while (!pairwise_coprime);

			primes.push_back(tmp);
			prod *= tmp;
		}

		return prod;
	}
};

} /* namespace epic::lint */

#endif /* EPIC_LINT_ITFPRIMECALCULATOR_H_ */
