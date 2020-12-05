#include "lint/ItfLargeNumberCalculator.h"

#include "lint/BigIntCalculator.h"
#include "lint/ChineseRemainder.h"
#include "lint/ItfPrimeCalculator.h"
#include "lint/PrimeCalculatorAddition.h"
#include "lint/PrimeCalculatorMultiplication.h"
#include "lint/UIntCalculator.h"

epic::lint::ItfLargeNumberCalculator* epic::lint::ItfLargeNumberCalculator::new_calculator(const bigInt& max_value, Operation op, IntRepresentation int_representation) {
	ItfLargeNumberCalculator* ret;

	ItfPrimeCalculator* calculator;
	if (op == Operation::multiplication) {
		calculator = new PrimeCalculatorMultiplication();
	} else {
		calculator = new PrimeCalculatorAddition();
	}

	PrimesInfo pInfo = calculator->getPrimes(max_value);
	if (pInfo.primesAmount == 1) {
		ret = new UIntCalculator();
	} else if ((int_representation == DEFAULT && pInfo.primesAmount > 4) || int_representation == GMP) {
		ret = new BigIntCalculator();
	} else { // if ((int_representation == DEFAULT && pInfo.primesAmount <= 4) || int_representation == PRIMES)
		ret = new ChineseRemainder(pInfo.primesArray, pInfo.primesAmount, pInfo.maxValue);
	}

	delete calculator;

	return ret;
}

void epic::lint::ItfLargeNumberCalculator::delete_calculator(ItfLargeNumberCalculator* remainder) {
	delete remainder;
}
