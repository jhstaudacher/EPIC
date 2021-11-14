#include "index/ShapleyShubikBelow.h"

#include "Array.h"

#include <algorithm>

epic::index::ShapleyShubikBelow::ShapleyShubikBelow(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawShapleyShubikBelow(g, approx, int_representation) {
}

std::vector<epic::bigFloat> epic::index::ShapleyShubikBelow::calculate() {
	std::vector<bigFloat> solution = RawShapleyShubikBelow::calculate();

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

std::string epic::index::ShapleyShubikBelow::getFullName() {
	return "ShapleyShubikBelow";
}
