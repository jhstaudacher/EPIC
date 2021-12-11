#ifndef EPIC_INDEX_COLEMANCOLLECTIVE_H_
#define EPIC_INDEX_COLEMANCOLLECTIVE_H_

#include "ItfUpperBoundApproximation.h"
#include "PowerIndexWithWinningCoalitions.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>collective Coleman</i> power index (`CC`).
 *
 * Let \f$\nu\f$ be a simple \f$n\f$-player mGame. \f$W\f$ denotes the set of winning coalitions.
 * Then the <i>collective Coleman</i> index of player \f$i\f$ is defined as
 * \f[
 *		C_i^C = \frac{|W|}{2^n}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>print the total number of winning coalitions
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation and assigns them the index 0 (the indices of the remaining players do not change)
 * </table>
 *
 * <b>Implementation note</b>:
 * If the mGame contains player of weight zero, they get excluded from the calculation (<i>Null player removable</i> property). Those players get the same index as all the other players (the result remains correct). If the `-v` flag is set, the extra output gets scaled as if the players of weight `0` were part of the mGame.
 */
class ColemanCollective : public PowerIndexWithWinningCoalitions {
public:
	/**
	 * Construct the ColemanCollective object
	 *
	 * @param g The Game for which the ColemanCollective index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to ItfLargeNumberCalculator::new_calculator()).
	 */
	ColemanCollective();

	std::vector<bigFloat> calculate(Game& g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game& g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_COLEMANCOLLECTIVE_H_ */
