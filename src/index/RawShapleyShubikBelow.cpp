#include "index/RawShapleyShubikBelow.h"

#include "Array.h"
#include "lint/GlobalCalculator.h"

#include <algorithm>

epic::index::RawShapleyShubikBelow::RawShapleyShubikBelow() : ItfPowerIndex() {}

std::vector<epic::bigFloat> epic::index::RawShapleyShubikBelow::calculate(Game& g) {
	// ssi(x, y): ShapleyShubik matrix - number of times player x is a swing player in a coalition of cardinality y
	Array2d<lint::LargeNumber> ssi(g.getNumberOfNonZeroPlayers(), g.getNumberOfNonZeroPlayers() + 1);
	gCalculator->allocInit_largeNumberArray(ssi.getArrayPointer(), ssi.getNumberOfElements());
	swingsPerPlayerAndCardinality(g, ssi);

	// factorial_s_ns1[x]: x! * (n - x - 1)! ;	n: the number of players (See section 3.2 in Sascha Kurz's paper)
	auto factorial_s_ns1 = new bigInt[g.getNumberOfNonZeroPlayers()];
	{
		// factorial[x]: x!
		auto factorial = new bigInt[g.getNumberOfNonZeroPlayers() + 1];
		factorial[0] = 1;
		factorial[1] = 1;

		for (longUInt i = 2; i <= g.getNumberOfNonZeroPlayers(); ++i) {
			factorial[i] = factorial[i - 1] * i;
		}
		for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) {
			factorial_s_ns1[i] = factorial[i] * factorial[g.getNumberOfNonZeroPlayers() - i - 1];
		}

		delete[] factorial;
	}

	std::vector<bigFloat> solution(g.getNumberOfPlayers());
	{
		bigInt dummy;
		bigInt raw_ssi;

		for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) {
			raw_ssi = 0;

			for (longUInt k = 1; k <= g.getNumberOfNonZeroPlayers(); ++k) {
				gCalculator->to_bigInt(&dummy, ssi(i, k));
				raw_ssi += dummy * factorial_s_ns1[k - 1];
			}

			solution[i] = raw_ssi;
		}

		for (longUInt i = g.getNumberOfNonZeroPlayers(); i < g.getNumberOfPlayers(); ++i) {
			solution[i] = 0;
		}
	}

	/*
	 * DELETE
	 */
	gCalculator->free_largeNumberArray(ssi.getArrayPointer());
	delete[] factorial_s_ns1;

	return solution;
}

std::string epic::index::RawShapleyShubikBelow::getFullName() {
	return "RawShapleyShubikBelow";
}

epic::longUInt epic::index::RawShapleyShubikBelow::getMemoryRequirement(Game& g) {
	bigInt memory = g.getNumberOfNonZeroPlayers();
	memory *= (g.getNumberOfNonZeroPlayers() + 1) * gCalculator->getLargeNumberSize(); // ssi
	memory /= cMemUnit_factor;

	bigInt memory_1 = bigInt(g.getQuota()) * (g.getNumberOfNonZeroPlayers() + 1) * gCalculator->getLargeNumberSize() * 2; // n_lc, helper
	memory_1 /= cMemUnit_factor;

	bigInt factorial_n = 0;
	mpz_fac_ui(factorial_n.get_mpz_t(), g.getNumberOfNonZeroPlayers());
	bigInt memory_2 = 2 * GMPHelper::size_of_int(factorial_n) * 2; // factorial, factorial_s_ns: sum_{i = 0}^{N} i! < 2 * N!
	memory_2 /= cMemUnit_factor;

	memory += (memory_1 > memory_2) ? memory_1 : memory_2;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

epic::bigInt epic::index::RawShapleyShubikBelow::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_swingPlayerPerCardinality();
}

epic::lint::Operation epic::index::RawShapleyShubikBelow::getOperationRequirement() {
	return lint::Operation::addition;
}

void epic::index::RawShapleyShubikBelow::numberOfLosingCoalitionsPerWeightAndCardinality(Game& g, Array2d<lint::LargeNumber>& n_lc) {
	gCalculator->assign_one(n_lc(0, 0));

	for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) { // i: player index
		longUInt wi = g.getWeights()[i];

		for (longInt w = g.getQuota() - wi - 1; w >= 0; --w) { // w: weight
			for (longUInt c = 0; c < g.getNumberOfNonZeroPlayers(); ++c) {   // c: cardinality
				gCalculator->plusEqual(n_lc(w + wi, c + 1), n_lc(w, c));
			}
		}
	}
}

void epic::index::RawShapleyShubikBelow::swingsPerPlayerAndCardinality(Game& g, Array2d<lint::LargeNumber>& raw_ssi) {
	Array2d<lint::LargeNumber> n_lc(g.getQuota(), g.getNumberOfNonZeroPlayers() + 1);
	gCalculator->allocInit_largeNumberArray(n_lc.getArrayPointer(), n_lc.getNumberOfElements());
	numberOfLosingCoalitionsPerWeightAndCardinality(g, n_lc);

	Array2d<lint::LargeNumber> helper(g.getQuota(), g.getNumberOfNonZeroPlayers() + 1);
	gCalculator->allocInit_largeNumberArray(helper.getArrayPointer(), helper.getNumberOfElements());

	for (longUInt i = 0; i < g.getNumberOfNonZeroPlayers(); ++i) { // i: player index
		longUInt wi = g.getWeights()[i];

		longUInt m = std::min(wi, g.getQuota());
		for (longUInt w = 0; w < m; ++w) {						 // w: weight
			for (longUInt c = 0; c < g.getNumberOfNonZeroPlayers(); ++c) { // c: cardinality
				gCalculator->assign(helper(w, c), n_lc(w, c));
			}
		}

		for (longUInt w = wi; w < g.getQuota(); ++w) {		  // w: weight
			for (longUInt c = 1; c <= g.getNumberOfNonZeroPlayers(); ++c) { // c: cardinality
				gCalculator->minus(helper(w, c), n_lc(w, c), helper(w - wi, c - 1));
			}
		}

		m = wi > g.getQuota() ? 0UL : g.getQuota() - wi;
		for (longUInt w = m; w < g.getQuota(); ++w) {		 // w: weight
			for (longUInt c = 0; c < g.getNumberOfNonZeroPlayers(); ++c) { // c: cardinality
				gCalculator->plusEqual(raw_ssi(i, c + 1), helper(w, c));
			}
		}
	}

	gCalculator->free_largeNumberArray(helper.getArrayPointer());
	gCalculator->free_largeNumberArray(n_lc.getArrayPointer());
}
