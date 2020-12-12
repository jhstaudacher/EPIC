#include "index/SingleValueWNP.h"

epic::index::SingleValueWNP::SingleValueWNP(Game& g) : ItfPowerIndex(g) {

}

std::vector<epic::bigFloat> epic::index::SingleValueWNP::calculate() {
	return {0.3};
}

std::string epic::index::SingleValueWNP::getFullName() {
	return "Number of null-player-free winning coalitions";
}

epic::longUInt epic::index::SingleValueWNP::getMemoryRequirement() {
	return 0;
}
