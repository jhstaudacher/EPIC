#include "index/SingleValueW.h"

epic::index::SingleValueW::SingleValueW(Game& g) : ItfPowerIndex(g) {

}

std::vector<epic::bigFloat> epic::index::SingleValueW::calculate() {
	return {0.1};
}

std::string epic::index::SingleValueW::getFullName() {
	return "Number of winning coalitions";
}

epic::longUInt epic::index::SingleValueW::getMemoryRequirement() {
	return 0;
}
