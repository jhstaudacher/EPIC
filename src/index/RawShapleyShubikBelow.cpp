#include "index/RawShapleyShubikBelow.h"

#include "Array.h"

#include <algorithm>

epic::index::RawShapleyShubikBelow::RawShapleyShubikBelow(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: ItfPowerIndex(g) {
	bigInt max_value = approx->upperBound_swingPlayerPerCardinality();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);
}

epic::index::RawShapleyShubikBelow::~RawShapleyShubikBelow() {
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

std::vector<epic::bigFloat> epic::index::RawShapleyShubikBelow::calculate() {
	// ssi(x, y): ShapleyShubik matrix - number of times player x is a swing player in a coalition of cardinality y
	Array2d<lint::LargeNumber> ssi(mNonZeroPlayerCount, mNonZeroPlayerCount + 1);
	mCalculator->allocInit_largeNumberArray(ssi.getArrayPointer(), ssi.getNumberOfElements());
	swingsPerPlayerAndCardinality(ssi);

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

std::string epic::index::RawShapleyShubikBelow::getFullName() {
	return "RawShapleyShubikBelow";
}

epic::longUInt epic::index::RawShapleyShubikBelow::getMemoryRequirement() {
	bigInt memory = mNonZeroPlayerCount;
	memory *= (mNonZeroPlayerCount + 1) * mCalculator->getLargeNumberSize(); // ssi
	memory /= cMemUnit_factor;

	bigInt memory_1 = bigInt(mGame.getQuota()) * (mNonZeroPlayerCount + 1) * mCalculator->getLargeNumberSize() * 2; // n_lc, helper
	memory_1 /= cMemUnit_factor;

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

// n_lc must be zero-initialized
void epic::index::RawShapleyShubikBelow::numberOfLosingCoalitionsPerWeightAndCardinality(Array2d<lint::LargeNumber>& n_lc) {
	mCalculator->assign_one(n_lc(0, 0));

	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) { // i: player index
		longUInt wi = mGame.getWeights()[i];

		for (longInt w = mGame.getQuota() - wi - 1; w >= 0; --w) { // w: weight
			for (longUInt c = 0; c < mNonZeroPlayerCount; ++c) { // c: cardinality
				mCalculator->plusEqual(n_lc(w + wi, c + 1), n_lc(w, c));
			}
		}
	}
}

// raw_ssi must be zero-initialized
void epic::index::RawShapleyShubikBelow::swingsPerPlayerAndCardinality(Array2d<lint::LargeNumber>& raw_ssi) {
	Array2d<lint::LargeNumber> n_lc(mGame.getQuota(), mNonZeroPlayerCount + 1);
	mCalculator->allocInit_largeNumberArray(n_lc.getArrayPointer(), n_lc.getNumberOfElements());
	numberOfLosingCoalitionsPerWeightAndCardinality(n_lc);

	 Array2d<lint::LargeNumber> helper(mGame.getQuota(), mNonZeroPlayerCount + 1);
	 mCalculator->allocInit_largeNumberArray(helper.getArrayPointer(), helper.getNumberOfElements());

	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) { // i: player index
		longUInt wi = mGame.getWeights()[i];

		longUInt m = std::min(wi, mGame.getQuota());
		for (longUInt w = 0; w < m; ++w) { // w: weight
			for (longUInt c = 0; c < mNonZeroPlayerCount; ++c) { // c: cardinality
				mCalculator->assign(helper(w, c), n_lc(w, c));
			}
		}

		for (longUInt w = wi; w < mGame.getQuota(); ++w) { // w: weight
			for (longUInt c = 1; c <= mNonZeroPlayerCount; ++c) { // c: cardinality
				mCalculator->minus(helper(w, c), n_lc(w, c), helper(w - wi, c - 1));
			}
		}

		m = wi > mGame.getQuota() ? 0UL : mGame.getQuota() - wi;
		for (longUInt w = m; w < mGame.getQuota(); ++w) { // w: weight
			for (longUInt c = 0; c < mNonZeroPlayerCount; ++c) { // c: cardinality
				mCalculator->plusEqual(raw_ssi(i, c+1), helper(w, c));
			}
		}
	}

	mCalculator->free_largeNumberArray(helper.getArrayPointer());
	mCalculator->free_largeNumberArray(n_lc.getArrayPointer());
}
