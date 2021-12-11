#ifndef EPIC_INDEX_COLEMANINITIATIVE_H_
#define EPIC_INDEX_COLEMANINITIATIVE_H_

#include "ItfUpperBoundApproximation.h"
#include "RawBanzhaf.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>initiative Coleman</i> power index (`CI`) based on the <i>raw Banzhaf</i> power index.
 *
 * Let \f$\nu\f$ be a simple \f$n\f$-player mGame and \f$\eta_i(\nu)\f$ the number of coalitions for which \f$i\f$ is a critical player. \f$W\f$ denotes the set of winning coalitions.
 * Then the <i>initiative Coleman</i> index of player \f$i\f$ is defined as
 * \f[
 *		C_i^I = \frac{\eta_i}{2^n - |W|}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>print the total number of winning coalitions as well as how often each player is a swing player (raw Banzhaf)
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation and assigns them the index 0
 * </table>
 *
 * <b>Implementation note</b>:
 * If the mGame contains player of weight zero, they get excluded from the calculation (<i>Null player removable</i> property). At the end the corresponding players get assigned the index `0` (<i>null player</i> property). If the `-v` flag is set, the extra output gets scaled as if the players of weight `0` were part of the mGame.
 */
class ColemanInitiative : public RawBanzhaf {
public:
	/**
	 * Construct the ColemanInitiative object
	 *
	 * @param g The Game for which the ColemanInitiative index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to ItfLargeNumberCalculator::new_calculator()).
	 */
	ColemanInitiative();

	std::vector<bigFloat> calculate(Game& g) override;
	std::string getFullName() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_COLEMANINITIATIVE_H_ */
