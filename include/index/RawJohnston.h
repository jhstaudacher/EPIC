#ifndef EPIC_INDEX_RAWJOHNSTON_H_
#define EPIC_INDEX_RAWJOHNSTON_H_

#include "ItfUpperBoundApproximation.h"
#include "PowerIndexWithWinningCoalitions.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>raw Johnston</i> power index (`RJ`).
 *
 * Let \f$VC\f$ denote the set of vulnerable coalitions and let \f$Cr(S)\f$ be the set of critical players in a coalition \f$S\f$.
 * Then the <i>raw Johnston</i> index of player \f$i\f$ is defined as
 * \f[
 *		\gamma_i^{raw} = \sum_{S \in VC, i \in Cr(S)} \frac{1}{|Cr(S)|}
 * \f]
 * The index for null player is `0`.
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>prints the total number of vulnerable coalitions
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation and assigns them the index 0
 * </table>
 *
 * <b>Implementation note</b>:
 * If the mGame contains player of weight zero, they get excluded from the calculation. At the end the corresponding players get assigned the index `0` (<i>null player</i> property).
 */
class RawJohnston : public PowerIndexWithWinningCoalitions {
public:
	/**
	 * Construct the RawJohnston object
	 *
	 * @param g The Game for which the RawJohnston index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to ItfLargeNumberCalculator::new_calculator()).
	 */
	RawJohnston(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation = DEFAULT);
	~RawJohnston() override;

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
	longUInt getMemoryRequirement() override;

private:
	/**
     * Sasha Kurz' algorithm (paper by Sasha Kurz, pages 3 and 4) adapted for cardinalities
     *
     * @param ret_ptr The matrix to return the calculated values. It must be allocated to have enough space for at least quota x weights.size() entries. All entries must be zero-initialized!
     * @param weights A list of all weights to consider. The first mNonZeroPlayerCount weights are getting considered at most. If you remove weights (compared to the mGame.getWeights() vector) make sure to remove the weights zero as well. In that case weights.size() weights are getting considered.
     * @param player_limit The upper cardinality limit
     * @param first_step Since this method calculates iteratively, this parameter must be set to true if it is the first iteration.
     */
	void forward_counting_per_weight_cardinality_next_step(Array2d<lint::LargeNumber>& ret_ptr, std::vector<longUInt> weights, longUInt player_limit, bool first_step);

	/**
	 * Work out Subset Sum problem by starting with the smallest weight along the lines of CMS book (see book by CMS, p. 230, below)
	 *
	 * @param h The matrix to return the result. This Array2d must be allocated to have enough space for at least player-count x quota entries. All entries must be zero-initialized!
	 */
	void subsetSumFromSmallestPerWeight(Array2d<longUInt>& h);

	/**
	 * Finding largest weight w with the property w <= weight
	 *
	 * @param weight The upper limit of the weight to find
	 * @return The index of the player having weight w
	 *
	 * @remark This method is fast since the weights are ordered.
	 */
	longUInt findIndexInRev(longUInt weight);
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_RAWJOHNSTON_H_ */
