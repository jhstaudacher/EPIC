#ifndef EPIC_INDEX_RAWSHAPLEYSHUBIKBELOW_H_
#define EPIC_INDEX_RAWSHAPLEYSHUBIKBELOW_H_

#include "Array.h"
#include "ItfPowerIndex.h"
#include "ItfUpperBoundApproximation.h"
#include "lint/ItfLargeNumberCalculator.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>raw Shapley Shubik</i> power index from below (`RSHB`).
 *
 * Let \f$\nu\f$ be a simple \f$n\f$-player Game and \f$\eta_i(\nu)\f$ the number of coalitions for which \f$i\f$ is a critical player.
 * Then the <i>raw Shapley Shubik</i> index of player \f$i\f$ is defined as
 * \f[
 *		\phi_i^{raw} = \sum_{c = 1}^n \eta_i(\nu, c)
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
class RawShapleyShubikBelow : public ItfPowerIndex {
public:
	RawShapleyShubikBelow();

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game* g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;

private:
	/**
	 * Calculating the number of losing coalitions per weight and cardinality.
	 *
	 * @param g The Game object for the current calculation
	 * @param n_lc A matrix where the calculated values will be stored. n_lc(x, y): number of losing coalitions of weight x and cardinality y.
	 *
	 * @note The n_lc matrix must be allocated and zero initialized at least in the range: [0, quota] x [0, numberOfPlayers + 1]
	 */
	void numberOfLosingCoalitionsPerWeightAndCardinality(Game* g, Array2d<lint::LargeNumber>& n_lc);

	/**
	 * Calculating how often each player is a swing player in coalitions of certain cardinalities.
	 *
	 * @param g The Game object for the current calculation
	 * @param raw_ssi A matrix where the calculated values will be stored. raw_ssi(x, y): amount of coalitions of cardinality y for which player x is a swing player.
	 *
	 * @note The raw_ssi matrix must be allocated and zero initialized in at least the range [0, numberOfPlayers] x [0, numberOfPlayers + 1].
	 * @note The values for players of weight zero will remain unchanged, i.e. zero (as initialized).
	 */
	void swingsPerPlayerAndCardinality(Game* g, Array2d<lint::LargeNumber>& raw_ssi);
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_RAWSHAPLEYSHUBIKBELOW_H_ */
