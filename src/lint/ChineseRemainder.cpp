#include "lint/ChineseRemainder.h"

#include "Logging.h"

#include <sstream>

epic::lint::ChineseRemainder::ChineseRemainder(const longUInt* primes, const size_t amount, const bigInt& m)
	: mPrimes(primes), mAmount(amount), mM(m) {
	// calculate the factors needed for the to_bigInt conversion
	mFactors = new bigInt[amount];
	bigInt big_prime, Mi, Mi_inverse;

	for (size_t i = 0; i < amount; ++i) {
		big_prime = mPrimes[i];
		Mi = mM / big_prime;

		if (mpz_invert(Mi_inverse.get_mpz_t(), Mi.get_mpz_t(), big_prime.get_mpz_t()) == 0) {
			throw std::runtime_error("unexpected error while calculating the modulo inverse elements of the primes!");
		}

		mFactors[i] = Mi * Mi_inverse;
	}

	log::out << log::info << "Using the Chinese Remainder Theorem (" << mAmount << " coprimes)" << log::endl;
	for (longUInt i = 0; i < mAmount; ++i) {
		log::out << " * coprime[" << i << "] = " << mPrimes[i] << log::endl;
	}
}

epic::lint::ChineseRemainder::~ChineseRemainder() {
	delete[] mFactors;
}

epic::longUInt epic::lint::ChineseRemainder::getLargeNumberSize() {
	return sizeof(LargeNumber) + (mAmount * c_sizeof_longUInt); // The allocated array of primes including the pointer to the first element
}

void epic::lint::ChineseRemainder::increment(LargeNumber& dest) {
	for (size_t i = 0; i < mAmount; ++i) {
		++dest.array[i];
		if (dest.array[i] >= mPrimes[i]) {
			dest.array[i] -= mPrimes[i];
		}
	}
}

void epic::lint::ChineseRemainder::plus(LargeNumber& dest, const LargeNumber& op1, const LargeNumber& op2) {
	for (size_t i = 0; i < mAmount; ++i) {
		dest.array[i] = op1.array[i] + op2.array[i];
		if (dest.array[i] >= mPrimes[i]) {
			dest.array[i] -= mPrimes[i];
		}
	}
}

void epic::lint::ChineseRemainder::plusEqual(LargeNumber& dest, const longUInt& op) {
	for (size_t i = 0; i < mAmount; ++i) {
		// make sure that the longUInt variable will not overflow when applying the addition. Since op is fairly small in the average case, this is more efficient than applying the %-operator every time.
		if (op >= mPrimes[i]) {
			dest.array[i] += (op % mPrimes[i]);
		} else {
			dest.array[i] += op;
		}

		if (dest.array[i] >= mPrimes[i]) {
			dest.array[i] -= mPrimes[i];
		}
	}
}

void epic::lint::ChineseRemainder::plusEqual(LargeNumber& dest, const LargeNumber& op) {
	for (size_t i = 0; i < mAmount; ++i) {
		dest.array[i] += op.array[i];
		if (dest.array[i] >= mPrimes[i]) {
			dest.array[i] -= mPrimes[i];
		}
	}
}

void epic::lint::ChineseRemainder::minus(LargeNumber& dest, const LargeNumber& op1, const LargeNumber& op2) {
	for (size_t i = 0; i < mAmount; ++i) {
		dest.array[i] = (op1.array[i] >= op2.array[i]) ? op1.array[i] - op2.array[i] : mPrimes[i] + op1.array[i] - op2.array[i];
	}
}

void epic::lint::ChineseRemainder::minusEqual(LargeNumber& dest, const LargeNumber& op) {
	for (size_t i = 0; i < mAmount; ++i) {
		if (dest.array[i] >= op.array[i]) {
			dest.array[i] -= op.array[i];
		} else {
			dest.array[i] = dest.array[i] + mPrimes[i] - op.array[i];
		}
	}
}

void epic::lint::ChineseRemainder::mul(LargeNumber& dest, const LargeNumber& op1, const longUInt& op2) {
	for (size_t i = 0; i < mAmount; ++i) {
		// make sure that the longUInt variable will not overflow when applying the addition. Since op is fairly small in the average case, this is more efficient than applying the %-operator every time.
		if (op2 >= mPrimes[i]) {
			dest.array[i] = op1.array[i] * (op2 % mPrimes[i]);
		} else {
			dest.array[i] = op1.array[i] * op2;
		}

		if (dest.array[i] >= mPrimes[i]) {
			dest.array[i] %= mPrimes[i];
		}
	}
}

void epic::lint::ChineseRemainder::mul(LargeNumber& dest, const LargeNumber& op1, const LargeNumber& op2) {
	for (size_t i = 0; i < mAmount; ++i) {
		dest.array[i] = op1.array[i] * op2.array[i];
		if (dest.array[i] >= mPrimes[i]) {
			dest.array[i] %= mPrimes[i];
		}
	}
}

void epic::lint::ChineseRemainder::mulEqual(LargeNumber& dest, const LargeNumber& op) {
	for (size_t i = 0; i < mAmount; ++i) {
		dest.array[i] *= op.array[i];
		if (dest.array[i] >= mPrimes[i]) {
			dest.array[i] %= mPrimes[i];
		}
	}
}

void epic::lint::ChineseRemainder::assign(LargeNumber& dest, const LargeNumber& value) {
	for (size_t i = 0; i < mAmount; ++i) {
		dest.array[i] = value.array[i];
	}
}

void epic::lint::ChineseRemainder::assign(LargeNumber& dest, const longUInt& value) {
	for (size_t i = 0; i < mAmount; ++i) {
		dest.array[i] = value;
		if (dest.array[i] >= mPrimes[i]) {
			dest.array[i] %= mPrimes[i];
		}
	}
}

void epic::lint::ChineseRemainder::assign_zero(LargeNumber& dest) {
	for (size_t i = 0; i < mAmount; ++i) {
		dest.array[i] = 0;
	}
}

void epic::lint::ChineseRemainder::assign_one(LargeNumber& dest) {
	for (size_t i = 0; i < mAmount; ++i) {
		dest.array[i] = 1;
	}
}

void epic::lint::ChineseRemainder::to_bigInt(bigInt* dest, const LargeNumber& value) {
	*dest = 0;

	for (size_t i = 0; i < mAmount; ++i) {
		*dest += value.array[i] * mFactors[i];
	}

	*dest %= mM;
}

std::string epic::lint::ChineseRemainder::to_string(LargeNumber& val) {
	bigInt bint;
	to_bigInt(&bint, val);

	std::stringstream sstream;
	sstream << bint;

	return sstream.str();
}

void epic::lint::ChineseRemainder::alloc_largeNumberArray(LargeNumber* array, longUInt number_of_elements) {
	auto mem_pool = new longUInt[number_of_elements * mAmount];

	for (longUInt i = 0; i < number_of_elements; ++i) {
		array[i].array = mem_pool + (i * mAmount);
	}
}

void epic::lint::ChineseRemainder::allocInit_largeNumberArray(LargeNumber* array, longUInt number_of_elements) {
	auto mem_pool = new longUInt[number_of_elements * mAmount]();

	for (longUInt i = 0; i < number_of_elements; ++i) {
		array[i].array = mem_pool + (i * mAmount);
	}
}

void epic::lint::ChineseRemainder::free_largeNumberArray(LargeNumber* array) {
	delete[] array[0].array;
}

void epic::lint::ChineseRemainder::alloc_largeNumber(LargeNumber& big_number) {
	big_number.array = new longUInt[mAmount];
}

void epic::lint::ChineseRemainder::allocInit_largeNumber(LargeNumber& big_number) {
	big_number.array = new longUInt[mAmount]();
}

void epic::lint::ChineseRemainder::free_largeNumber(LargeNumber& big_number) {
	delete[] big_number.array;
}
