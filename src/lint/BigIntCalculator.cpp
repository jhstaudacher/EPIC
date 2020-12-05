#include "lint/BigIntCalculator.h"

#include <sstream>

/**
 * @note This implementation returns 0 since each bigInt can independently grow in size (no static size)!
 */
epic::longUInt epic::lint::BigIntCalculator::getLargeNumberSize() {
	return 0;
}

void epic::lint::BigIntCalculator::increment(LargeNumber& dest) {
	++(*dest.bint);
}

void epic::lint::BigIntCalculator::plus(LargeNumber& dest, const LargeNumber& op1, const LargeNumber& op2) {
	*dest.bint = *op1.bint + *op2.bint;
}

void epic::lint::BigIntCalculator::plusEqual(LargeNumber& dest, const longUInt& op) {
	*dest.bint += op;
}

void epic::lint::BigIntCalculator::plusEqual(LargeNumber& dest, const LargeNumber& op) {
	*dest.bint += *op.bint;
}

void epic::lint::BigIntCalculator::minus(LargeNumber& dest, const LargeNumber& op1, const LargeNumber& op2) {
	*dest.bint = *op1.bint - *op2.bint;
}

void epic::lint::BigIntCalculator::minusEqual(LargeNumber& dest, const LargeNumber& op) {
	*dest.bint -= *op.bint;
}

void epic::lint::BigIntCalculator::mul(LargeNumber& dest, const LargeNumber& op1, const longUInt& op2) {
	*dest.bint = *op1.bint * op2;
}

void epic::lint::BigIntCalculator::mul(LargeNumber& dest, const LargeNumber& op1, const LargeNumber& op2) {
	*dest.bint = *op1.bint * *op2.bint;
}

void epic::lint::BigIntCalculator::mulEqual(LargeNumber& dest, const LargeNumber& op) {
	*dest.bint *= *op.bint;
}

void epic::lint::BigIntCalculator::assign(LargeNumber& dest, const LargeNumber& value) {
	*dest.bint = *value.bint;
}

void epic::lint::BigIntCalculator::assign(LargeNumber& dest, const longUInt& value) {
	*dest.bint = value;
}

void epic::lint::BigIntCalculator::assign_zero(LargeNumber& dest) {
	*dest.bint = 0;
}

void epic::lint::BigIntCalculator::assign_one(LargeNumber& dest) {
	*dest.bint = 1;
}

void epic::lint::BigIntCalculator::to_bigInt(bigInt* dest, const LargeNumber& value) {
	*dest = *value.bint;
}

std::string epic::lint::BigIntCalculator::to_string(LargeNumber& val) {
	std::stringstream sstream;
	sstream << *val.bint;
	return sstream.str();
}

void epic::lint::BigIntCalculator::alloc_largeNumberArray(LargeNumber* array, longUInt number_of_elements) {
	auto mem_pool = new bigInt[number_of_elements];

	for (longUInt i = 0; i < number_of_elements; ++i) {
		array[i].bint = mem_pool + i;
	}
}

void epic::lint::BigIntCalculator::allocInit_largeNumberArray(LargeNumber* array, longUInt number_of_elements) {
	auto mem_pool = new bigInt[number_of_elements]();

	for (longUInt i = 0; i < number_of_elements; ++i) {
		array[i].bint = mem_pool + i;
	}
}

void epic::lint::BigIntCalculator::free_largeNumberArray(LargeNumber* array) {
	delete[] array[0].bint;
}

void epic::lint::BigIntCalculator::alloc_largeNumber(LargeNumber& big_number) {
	big_number.bint = new bigInt;
}

void epic::lint::BigIntCalculator::allocInit_largeNumber(LargeNumber& big_number) {
	big_number.bint = new bigInt(0);
}

void epic::lint::BigIntCalculator::free_largeNumber(LargeNumber& big_number) {
	delete big_number.bint;
}
