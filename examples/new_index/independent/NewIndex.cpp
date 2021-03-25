#include "index/NewIndex.h"

epic::index::NewIndex::NewIndex(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: ItfPowerIndex(g) {
	/*
	 * Initialize the protected ItfPowerIndex::mCalculator object. This gets used to do all large integer calculations later.
	 * For an approximation of the maximum value needed for the calculations the approx object can get used.
	 */
	bigInt max_value = approx->upperBound_totalNumberOfSwingPlayer();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);

	/*
	 * If needed add additional initializations below
	 */
}

epic::index::NewIndex::~NewIndex() {
	/*
	 * Delete the mCalculator object
	 */
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);

	/**
	 * If needed do additional cleanup below
	 */
}

std::vector<epic::bigFloat> epic::index::NewIndex::calculate() {
	/*
	 * Use mGame and mCalculator to calculate the power index and return the results.
	 */
	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	return solution;
}

std::string epic::index::NewIndex::getFullName() {
	/*
	 * Return the full name of the implemented power index.
	 */
	return "NewIndex";
}

epic::longUInt epic::index::NewIndex::getMemoryRequirement() {
	/*
	 * Return an approximated value of the amount of memory needed for the power index calculation.
	 */
	bigInt memory = 42; // write the needed memory into this variable

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}
	return ret;
}
