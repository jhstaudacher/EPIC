#ifndef EPIC_LINT_CHINESEREMAINDER_H_
#define EPIC_LINT_CHINESEREMAINDER_H_

#include "ItfLargeNumberCalculator.h"

namespace epic::lint {

/**
 * An implementation of the ItfLargeNumberCalculator interface really using the chinese lint theorem.
 *
 * This implementation uses the chinese lint theorem and offers all functions that can get applied to an ChineseNumber.
 *
 * @remark This implementation only uses the ChineseNumber's array member. It allocates, modifies and frees this array.
 * @remark Each operation on a LargeNumber's value has to ensure (by manual overflow at mPrimes[i], e.g. array[i] %= mPrimes[i]) that each array entry is smaller then its corresponding prime (i.e. array[i] < mPrimes[i])! Violating this condition can cause overflows (at max(longUInt)) resulting in wrong values.
 */
class ChineseRemainder : public ItfLargeNumberCalculator {
public:
	/**
	 * Constructing an ChineseRemainder object
	 *
	 * @param primes An array of primes (i.e. co-primes) with at least amount entries
	 * @param amount The number of primes needed to represent the values and contained in the primes array
	 * @param m The product of all needed primes (= prod_{i = 0}^{amount - 1} primes[i] = primes[0] * primes[1] * ... * primes[n - 1])
	 *
	 * @note The primes array must exists as long the created ChineseRemainder object exists!
	 * @note The size of the primes must be chosen such that the required operations can get applied without an overflow of the longUInt variables!
	 */
	ChineseRemainder(const longUInt* primes, size_t amount, const bigInt& m);
	~ChineseRemainder() override;

	longUInt getLargeNumberSize() override;

	void increment(LargeNumber& dest) override;

	void plus(LargeNumber& dest, const LargeNumber& op1, const LargeNumber& op2) override;
	void plusEqual(LargeNumber& dest, const longUInt& op) override;
	void plusEqual(LargeNumber& dest, const LargeNumber& op) override;
	void minus(LargeNumber& dest, const LargeNumber& op1, const LargeNumber& op2) override;
	void minusEqual(LargeNumber& dest, const LargeNumber& op) override;
	void mul(LargeNumber& dest, const LargeNumber& op1, const longUInt& op2) override;
	void mul(LargeNumber& dest, const LargeNumber& op1, const LargeNumber& op2) override;
	void mulEqual(LargeNumber& dest, const LargeNumber& op) override;

	void assign(LargeNumber& dest, const LargeNumber& value) override;
	void assign(LargeNumber& dest, const longUInt& value) override;
	void assign_zero(LargeNumber& dest) override;
	void assign_one(LargeNumber& dest) override;

	/**
	 * Converting the value represented by value and its primes to an object of bigInt regarding the Chinese Remainder theorem.
	 *
	 * dest = value
	 *
	 * @param[out] dest An pointer to the conversion target
	 * @param[in] value The value to write to dest
	 *
	 * @note This method implementation uses bigInt addition and multiplication. If only a few operations are done before the to_bigInt() conversion the speedup of the Chinese Remainder Theorem might be less than the overhead of this conversion. In such a case it is preferable (in terms of speed) to use the BigIntCalculator() instead.
	 */
	void to_bigInt(bigInt* dest, const LargeNumber& value) override;
	std::string to_string(LargeNumber& val) override;

	void alloc_largeNumberArray(LargeNumber* array, longUInt number_of_elements) override;
	void allocInit_largeNumberArray(LargeNumber* array, longUInt number_of_elements) override;
	void free_largeNumberArray(LargeNumber* array) override;

	void alloc_largeNumber(LargeNumber& big_number) override;
	void allocInit_largeNumber(LargeNumber& big_number) override;
	void free_largeNumber(LargeNumber& big_number) override;

private:
	const longUInt* mPrimes; // An pointer to the first element of an array of primes (co-primes) with at least mAmount entries
	const size_t mAmount;	 // The number of used primes
	const bigInt mM;		 // The product of all used primes
	bigInt* mFactors;		 // An array to precalculated (at construction time) factors later needed for the conversion back to a bigInt object. mFactors[i] = Mi * Mi_inverse (Mi: the product of all primes except prime[i]: mM / mPrimes[i]; Mi_inverse: the modulo inverse element of mPrimes[i] mod mM)
};

} /* namespace epic::lint */

#endif /* EPIC_LINT_CHINESEREMAINDER_H_ */
