#include "index/ShapleyShubik.h"

#include "Array.h"

#include <algorithm>

epic::index::ShapleyShubik::ShapleyShubik()
	: RawShapleyShubik() {}

std::vector<epic::bigFloat> epic::index::ShapleyShubik::calculate(Game* g) {
	std::vector<bigFloat> solution = RawShapleyShubik::calculate(g);

	bigFloat n_factorial; // = n!
	{
		// factorial[x]: x!
		auto factorial = new bigInt[g->getNumberOfNonZeroPlayers() + 1];
		factorial[0] = 1;
		factorial[1] = 1;

		for (longUInt i = 2; i <= g->getNumberOfNonZeroPlayers(); ++i) {
			factorial[i] = factorial[i - 1] * i;
		}

		n_factorial = factorial[g->getNumberOfNonZeroPlayers()];
		delete[] factorial;
	}

	for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
		solution[i] /= n_factorial;
	}

	return solution;
}

std::string epic::index::ShapleyShubik::getFullName() {
	return "ShapleyShubik";
}
