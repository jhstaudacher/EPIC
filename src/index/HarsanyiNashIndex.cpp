#include "index/HarsanyiNashIndex.h"

#include "types.h"

#include <iostream>

epic::index::HarsanyiNashIndex::HarsanyiNashIndex()
	: ItfPowerIndex() {}

std::vector<epic::bigFloat> epic::index::HarsanyiNashIndex::calculate(Game* g) {
	std::vector<bigFloat> solution(g->getNumberOfPlayers());

	if (g->getNumberOfVetoPlayers() == 1) {
		for (longUInt i = 0; i < g->getNumberOfPlayers(); ++i) {
			if (g->getVetoPlayerVector()[i]) {
				solution[i] = 1.0f;
			} else {
				solution[i] = 0.0f;
			}
		}
	} else {
		bigFloat s = 1.0f / (double)g->getNumberOfPlayers();
		for (longUInt i = 0; i < g->getNumberOfPlayers(); ++i) {
			solution[i] = s;
		}
	}

	return solution;
}

std::string epic::index::HarsanyiNashIndex::getFullName() {
	return "Harsanyi-Nash";
}

epic::longUInt epic::index::HarsanyiNashIndex::getMemoryRequirement(Game* g) {
	bigFloat c_1 = 1;
	return (g->getNumberOfPlayers() * GMPHelper::size_of_float(c_1)) / cMemUnit_factor;
}

epic::bigInt epic::index::HarsanyiNashIndex::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return 0;
}

epic::lint::Operation epic::index::HarsanyiNashIndex::getOperationRequirement() {
	return lint::Operation::addition;
}
