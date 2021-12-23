#include "lint/ItfLargeNumberCalculator.h"

#include "lint/BigIntCalculator.h"
#include "lint/ChineseRemainder.h"
#include "lint/ItfPrimeCalculator.h"
#include "lint/PrimeCalculatorAddition.h"
#include "lint/PrimeCalculatorMultiplication.h"
#include "lint/UIntCalculator.h"

epic::lint::ItfLargeNumberCalculator* epic::lint::ItfLargeNumberCalculator::new_calculator(const CalculatorConfig& config) {
	ItfLargeNumberCalculator* ret;

	ItfPrimeCalculator* calculator;
	if (config.op == Operation::multiplication) {
		calculator = new PrimeCalculatorMultiplication();
	} else {
		calculator = new PrimeCalculatorAddition();
	}

	PrimesInfo pInfo = calculator->getPrimes(config.maxValue);
	if (pInfo.primesAmount == 1) {
		ret = new UIntCalculator();
	} else if ((config.intRep == DEFAULT && pInfo.primesAmount > 4) || config.intRep == GMP) {
		ret = new BigIntCalculator();
	} else { // if ((int_representation == DEFAULT && pInfo.primesAmount <= 4) || int_representation == PRIMES)
		ret = new ChineseRemainder(pInfo.primesArray, pInfo.primesAmount, pInfo.maxValue);
	}

	delete calculator;

	return ret;
}

void epic::lint::ItfLargeNumberCalculator::delete_calculator(ItfLargeNumberCalculator* calculator) {
	delete calculator;
}
