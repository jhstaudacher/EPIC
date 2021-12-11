#include "index/RawShapleyShubik.h"

#include "Array.h"
#include "lint/GlobalCalculator.h"

#include <algorithm>

epic::index::RawShapleyShubik::RawShapleyShubik() : SwingsPerPlayerAndCardinality() {}

std::vector<epic::bigFloat> epic::index::RawShapleyShubik::calculate(Game& g) {
	// ssi(x, y): ShapleyShubik matrix - number of times player x is a swing player in a coalition of cardinality y
	Array2d<lint::LargeNumber> ssi(g.getNumberOfNonZeroPlayers(), g.getNumberOfNonZeroPlayers() + 1);
	gCalculator->allocInit_largeNumberArray(ssi.getArrayPointer(), ssi.getNumberOfElements());
	swingsPerPlayerAndCardinality(g, ssi, true);

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

	// delete ssi
	gCalculator->free_largeNumberArray(ssi.getArrayPointer());
	delete[] factorial_s_ns1;

	return solution;
}

std::string epic::index::RawShapleyShubik::getFullName() {
	return "RawShapleyShubik";
}

epic::longUInt epic::index::RawShapleyShubik::getMemoryRequirement(Game& g) {
	bigInt memory = g.getNumberOfNonZeroPlayers();
	memory *= (g.getNumberOfNonZeroPlayers() + 1) * gCalculator->getLargeNumberSize(); // ssi
	memory /= cMemUnit_factor;

	bigInt memory_1 = SwingsPerPlayerAndCardinality::getMemoryRequirement(g);

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

epic::bigInt epic::index::RawShapleyShubik::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_swingPlayerPerCardinality();
}

epic::lint::Operation epic::index::RawShapleyShubik::getOperationRequirement() {
	return lint::Operation::addition;
}
