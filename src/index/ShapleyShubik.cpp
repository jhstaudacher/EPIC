#include "index/ShapleyShubik.h"

#include "Array.h"

#include <algorithm>

epic::index::ShapleyShubik::ShapleyShubik(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawShapleyShubik(g, approx, int_representation) {
}

std::vector<epic::bigFloat> epic::index::ShapleyShubik::calculate() {
	std::vector<bigFloat> solution = RawShapleyShubik::calculate();

	bigFloat n_factorial; // = n!
	{
		// factorial[x]: x!
		auto factorial = new bigInt[mNonZeroPlayerCount + 1];
		factorial[0] = 1;
		factorial[1] = 1;

		for (longUInt i = 2; i <= mNonZeroPlayerCount; ++i) {
			factorial[i] = factorial[i - 1] * i;
		}

		n_factorial = factorial[mNonZeroPlayerCount];
		delete[] factorial;
	}

	for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
		solution[i] /= n_factorial;
	}

	return solution;
}

std::string epic::index::ShapleyShubik::getFullName() {
	return "ShapleyShubik";
}
