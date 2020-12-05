#include "lint/UIntCalculator.h"

epic::longUInt epic::lint::UIntCalculator::getLargeNumberSize() {
	return sizeof(LargeNumber); // only the longUInt. No dynamic allocated memory
}

void epic::lint::UIntCalculator::increment(LargeNumber& dest) {
	++dest.uint;
}

void epic::lint::UIntCalculator::plus(LargeNumber& dest, const LargeNumber& op1, const LargeNumber& op2) {
	dest.uint = op1.uint + op2.uint;
}

void epic::lint::UIntCalculator::plusEqual(LargeNumber& dest, const longUInt& op) {
	dest.uint += op;
}

void epic::lint::UIntCalculator::plusEqual(LargeNumber& dest, const LargeNumber& op) {
	dest.uint += op.uint;
}

void epic::lint::UIntCalculator::minus(LargeNumber& dest, const LargeNumber& op1, const LargeNumber& op2) {
	dest.uint = op1.uint - op2.uint;
}

void epic::lint::UIntCalculator::minusEqual(LargeNumber& dest, const LargeNumber& op) {
	dest.uint -= op.uint;
}

void epic::lint::UIntCalculator::mul(LargeNumber& dest, const LargeNumber& op1, const longUInt& op2) {
	dest.uint = op1.uint * op2;
}

void epic::lint::UIntCalculator::mul(LargeNumber& dest, const LargeNumber& op1, const LargeNumber& op2) {
	dest.uint = op1.uint * op2.uint;
}

void epic::lint::UIntCalculator::mulEqual(LargeNumber& dest, const LargeNumber& op) {
	dest.uint *= op.uint;
}

void epic::lint::UIntCalculator::assign(LargeNumber& dest, const LargeNumber& value) {
	dest.uint = value.uint;
}

void epic::lint::UIntCalculator::assign(LargeNumber& dest, const longUInt& value) {
	dest.uint = value;
}

void epic::lint::UIntCalculator::assign_zero(LargeNumber& dest) {
	dest.uint = 0;
}

void epic::lint::UIntCalculator::assign_one(LargeNumber& dest) {
	dest.uint = 1;
}

void epic::lint::UIntCalculator::to_bigInt(bigInt* dest, const LargeNumber& value) {
	mpz_set_ui(dest->get_mpz_t(), value.uint);
}

std::string epic::lint::UIntCalculator::to_string(LargeNumber& val) {
	return std::to_string(val.uint);
}

void epic::lint::UIntCalculator::alloc_largeNumberArray(LargeNumber* array, longUInt number_of_elements) {
	// nothing to do here
}

void epic::lint::UIntCalculator::allocInit_largeNumberArray(LargeNumber* array, longUInt number_of_elements) {
	for (longUInt i = 0; i < number_of_elements; ++i) {
		array[i].uint = longUInt{};
	}
}

void epic::lint::UIntCalculator::free_largeNumberArray(LargeNumber* array) {
	// nothing to do here
}

void epic::lint::UIntCalculator::alloc_largeNumber(LargeNumber& big_number) {
	// nothing to do here
}

void epic::lint::UIntCalculator::allocInit_largeNumber(LargeNumber& big_number) {
	big_number.uint = longUInt{};
}

void epic::lint::UIntCalculator::free_largeNumber(LargeNumber& big_number) {
	// nothing to do here
}
