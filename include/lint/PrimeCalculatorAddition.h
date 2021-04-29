#ifndef EPIC_LINT_PRIMECALCULATORADDITION_H_
#define EPIC_LINT_PRIMECALCULATORADDITION_H_

#include "ItfPrimeCalculator.h"

#include <array>
#include <cmath>
#include <numeric>

namespace epic::lint {

/**
 * A specialized class to calculate primes used for the Chinese Remainder Theorem if only the addition and subtraction operation is needed.
 *
 * @remark The reason to use two separate classes instead of only change the max_value is, that this class also encapsulates the pre-calculated primes for addition/subtraction.
 */
class PrimeCalculatorAddition : public ItfPrimeCalculator {
public:
	PrimesInfo getPrimes(bigInt max_value) override;

private:
	/**
	 * The number of precalculated primes
	 */
	static constexpr size_t mNPrecPrimes = 10;

	/**
	 * An Array containing the precalculated primes
	 */
#if defined(__APPLE__)
	static constexpr StaticArray<longUInt, mNPrecPrimes> mPrecPrimes = ItfPrimeCalculator::precalculate_primes<mNPrecPrimes>((unsigned long) 9223372036854775808);
#else
	static constexpr StaticArray<longUInt, mNPrecPrimes> mPrecPrimes = ItfPrimeCalculator::precalculate_primes<mNPrecPrimes>(std::pow(2, std::numeric_limits<longUInt>::digits - 1));
#endif
};

} /* namespace epic::lint */

#endif /* EPIC_LINT_PRIMECALCULATORADDITION_H_ */
