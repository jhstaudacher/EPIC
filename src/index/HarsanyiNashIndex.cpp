#include "index/HarsanyiNashIndex.h"

#include "types.h"

#include <iostream>

epic::index::HarsanyiNashIndex::HarsanyiNashIndex(Game& g)
	: ItfPowerIndex(g) {
}

std::vector<epic::bigFloat> epic::index::HarsanyiNashIndex::calculate() {
	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());

	if (mGame.getNumberOfVetoPlayers() == 1) {
		for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
			if (mGame.getVetoPlayerVector()[i]) {
				solution[i] = 1.0f;
			} else {
				solution[i] = 0.0f;
			}
		}
	} else {
		bigFloat s = 1.0f / (double)mGame.getNumberOfPlayers();
		for (longUInt i = 0; i < mGame.getNumberOfPlayers(); ++i) {
			solution[i] = s;
		}
	}

	return solution;
}

std::string epic::index::HarsanyiNashIndex::getFullName() {
	return "Harsanyi-Nash";
}

epic::longUInt epic::index::HarsanyiNashIndex::getMemoryRequirement() {
	bigFloat c_1 = 1;
	longUInt ret = mGame.getNumberOfPlayers() * GMPHelper::size_of_float(c_1);
	return ret;
}
