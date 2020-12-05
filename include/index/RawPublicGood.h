#ifndef EPIC_INDEX_RAWPUBLICGOOD_H_
#define EPIC_INDEX_RAWPUBLICGOOD_H_

#include "ItfPowerIndex.h"
#include "ItfUpperBoundApproximation.h"
#include "lint/ItfLargeNumberCalculator.h"

namespace epic::index {
/**
 * Implementing the ItfPowerIndex interface to calculate the <i>absolute Public Good</i> power index (`PGA`).
 *
 * Let \f$W_i^m\f$ denote the set of minimal winning coalitions containing player \f$i\f$.
 * Then the <i>absolute Public Good</i> index of player \f$i\f$ is defined as
 * \f[
 *		h_i = |W_i^m|
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>no extra output
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation and assigns them the index 0
 * </table>
 *
 * <b>Implementation note</b>:
 * If the mGame contains player of weight zero, they get excluded from the calculation. At the end the corresponding players get assigned the index `0` (<i>null player</i> property).
 */
class RawPublicGood : public ItfPowerIndex {
public:
	/**
	 * Construct the RawPublicGood object
	 *
	 * @param g The Game for which the RawPublicGood index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to ItfLargeNumberCalculator::new_calculator()).
	 */
	RawPublicGood(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation = DEFAULT);
	~RawPublicGood() override;

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
	longUInt getMemoryRequirement() override;

protected:
	/**
	 * Calculating the number of minimal winning coalitions a player belongs to.
	 *
	 * @param mwc The return array, mwc[x]: the number of minimal winning coalitions player x belongs to. The array must have enough memory for at least numberOfPlayers entries. Each entry must be initialized with zero!
	 */
	void calculateMinimalWinningCoalitionsPerPlayer(lint::LargeNumber mwc[]);

private:
	/**
	 * Calculating the f vector needed for the minimal winning coalitions calculation.
	 *
	 * @param f The return array. The array must have enough memory for at least quota entries. Each entry must be initialized with zero!
	 */
	void calculateFVector(lint::LargeNumber f[]);
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_RAWPUBLICGOOD_H_ */
