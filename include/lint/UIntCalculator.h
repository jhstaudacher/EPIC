#ifndef EPIC_LINT_UINTCALCULATOR_H_
#define EPIC_LINT_UINTCALCULATOR_H_

#include "ItfLargeNumberCalculator.h"

namespace epic::lint {

/**
 * An implementation of the ItfLargeNumberCalculator interface wrapping a single integer variable
 *
 * This implementation is only used if the values to represent are small enough to fit into a single integer variable. In that case no chinese lint theorem is needed. However this wrapper class is necessary to get the same ChineseNumber handling no matter if the chinese lint theorem is needed and applied.
 *
 * @remark This implementation only uses the ChineseNumber's uint member. That's why the alloc and free methods are empty.
 */
class UIntCalculator : public ItfLargeNumberCalculator {
public:
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

	void to_bigInt(bigInt* dest, const LargeNumber& value) override;
	std::string to_string(LargeNumber& val) override;

	void alloc_largeNumberArray(LargeNumber* array, longUInt number_of_elements) override;
	void allocInit_largeNumberArray(LargeNumber* array, longUInt number_of_elements) override;
	void free_largeNumberArray(LargeNumber* array) override;

	void alloc_largeNumber(LargeNumber& big_number) override;
	void allocInit_largeNumber(LargeNumber& big_number) override;
	void free_largeNumber(LargeNumber& big_number) override;
};

} /* namespace epic::lint */

#endif /* EPIC_LINT_UINTCALCULATOR_H_ */
