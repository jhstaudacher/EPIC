#ifndef EPIC_LINT_PRIMECALCULATORMULTIPLICATION_H_
#define EPIC_LINT_PRIMECALCULATORMULTIPLICATION_H_

#include "ItfPrimeCalculator.h"

namespace epic::lint {

/**
 * A specialized class to calculate primes used for the Chinese Remainder Theorem if only the addition and subtraktion operation is needed
 *
 * @remark The reason to use two separate classes instead of only change the max_value is, that this class also encapsulates the pre-calculated primes for multiplication.
 */
class PrimeCalculatorMultiplication : public ItfPrimeCalculator {
public:
	PrimesInfo getPrimes(bigInt max_value) override;

private:
	/**
	 * The number of precalculated primes
	 */
	static constexpr size_t mNPrecPrimes = 20;

	/**
	 * An Array containing the precalculated primes
	 */
	// static constexpr StaticArray<longUInt, mNPrecPrimes> mPrecPrimes = ItfPrimeCalculator::precalculate_primes<mNPrecPrimes>(std::pow(2, std::numeric_limits<longUInt>::digits / 2));
	static constexpr StaticArray<longUInt, mNPrecPrimes> mPrecPrimes = ItfPrimeCalculator::precalculate_primes<mNPrecPrimes>(1 << (std::numeric_limits<longUInt>::digits / 2));
};

} /* namespace epic::lint */

#endif /* EPIC_LINT_PRIMECALCULATORMULTIPLICATION_H_ */
