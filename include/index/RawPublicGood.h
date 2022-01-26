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
	RawPublicGood();

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game* g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;

	/**
	 * Another interface to the calculate()-function allowing to get the values as a LargeNumber array instead of a bigFloat vector. Passing in the solution array also avoids unnecessary allocations.
	 *
	 * @param g A Game-object for which the power index should be calculated
	 * @param solution Return vector the solution will be stored in (will be resized to g->getNumberOfPlayers() elements)
	 */
	void calculate(Game* g, lint::LargeNumber* solution);

protected:
	/**
	 * Calculating the number of minimal winning coalitions a player belongs to.
	 *
	 * @param g The Game object for the current calculation
	 * @param mwc The return array, mwc[x]: the number of minimal winning coalitions player x belongs to. The array must have enough memory for at least numberOfPlayers entries. Each entry must be initialized with zero!
	 */
	void calculateMinimalWinningCoalitionsPerPlayer(Game* g, lint::LargeNumber mwc[]);

private:
	/**
	 * Calculating the f vector needed for the minimal winning coalitions calculation.
	 *
	 * @param g The Game object for the current calculation
	 * @param f The return array. The array must have enough memory for at least quota entries. Each entry must be initialized with zero!
	 */
	void calculateFVector(Game* g, lint::LargeNumber f[]);
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_RAWPUBLICGOOD_H_ */
