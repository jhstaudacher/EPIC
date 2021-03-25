#include "index/NewIndex.h"

epic::index::NewIndex::NewIndex(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawPublicGood(g, approx, int_representation) {
	/*
	 * If needed add additional initializations below
	 */
}

std::vector<epic::bigFloat> epic::index::NewIndex::calculate() {
	/*
	 * Use mGame, mCalculator and the methods offered by RawPublicGood to calculate the power index and return the results.
	 */
	std::vector<bigFloat> solution = RawPublicGood::calculate();
	return solution;
}

std::string epic::index::NewIndex::getFullName() {
	/*
	 * Return the full name of the implemented power index.
	 */
	return "NewIndex";
}
