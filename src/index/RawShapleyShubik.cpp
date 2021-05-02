#include "index/RawShapleyShubik.h"

#include "Array.h"

#include <algorithm>

epic::index::RawShapleyShubik::RawShapleyShubik(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: SwingsPerPlayerAndCardinality(g) {
	bigInt max_value = approx->upperBound_swingPlayerPerCardinality();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);
}

epic::index::RawShapleyShubik::~RawShapleyShubik() {
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

std::vector<epic::bigFloat> epic::index::RawShapleyShubik::calculate() {
	// ssi(x, y): ShapleyShubik matrix - number of times player x is a swing player in a coalition of cardinality y
	Array2d<lint::LargeNumber> ssi(mNonZeroPlayerCount, mNonZeroPlayerCount + 1);
	mCalculator->allocInit_largeNumberArray(ssi.getArrayPointer(), ssi.getNumberOfElements());
	swingsPerPlayerAndCardinality(ssi, true);

	// factorial_s_ns1[x]: x! * (n - x - 1)! ;	n: the number of players (See section 3.2 in Sascha Kurz's paper)
	auto factorial_s_ns1 = new bigInt[mNonZeroPlayerCount];
	{
		// factorial[x]: x!
		auto factorial = new bigInt[mNonZeroPlayerCount + 1];
		factorial[0] = 1;
		factorial[1] = 1;

		for (longUInt i = 2; i <= mNonZeroPlayerCount; ++i) {
			factorial[i] = factorial[i - 1] * i;
		}
		for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
			factorial_s_ns1[i] = factorial[i] * factorial[mNonZeroPlayerCount - i - 1];
		}

		delete[] factorial;
	}

	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	{
		bigInt dummy;
		bigInt raw_ssi;

		for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
			raw_ssi = 0;

			for (longUInt k = 1; k <= mNonZeroPlayerCount; ++k) {
				mCalculator->to_bigInt(&dummy, ssi(i, k));
				raw_ssi += dummy * factorial_s_ns1[k - 1];
			}

			solution[i] = raw_ssi;
		}
		for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) {
			solution[i] = 0;
		}
	}

	/*
	 * DELETE
	 */

	// delete ssi
	mCalculator->free_largeNumberArray(ssi.getArrayPointer());
	delete[] factorial_s_ns1;

	return solution;
}

std::string epic::index::RawShapleyShubik::getFullName() {
	return "RawShapleyShubik";
}

epic::longUInt epic::index::RawShapleyShubik::getMemoryRequirement() {
	bigInt memory = mNonZeroPlayerCount;
	memory *= (mNonZeroPlayerCount + 1) * mCalculator->getLargeNumberSize(); // ssi
	memory /= cMemUnit_factor;

	bigInt memory_1 = SwingsPerPlayerAndCardinality::getMemoryRequirement();

	bigInt factorial_n = 0;
	mpz_fac_ui(factorial_n.get_mpz_t(), mNonZeroPlayerCount);
	bigInt memory_2 = 2 * GMPHelper::size_of_int(factorial_n) * 2; // factorial, factorial_s_ns: sum_{i = 0}^{N} i! < 2 * N!
	memory_2 /= cMemUnit_factor;

	memory += (memory_1 > memory_2) ? memory_1 : memory_2;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}
