#ifndef EPIC_INDEX_PUBLICHELPXI_H_
#define EPIC_INDEX_PUBLICHELPXI_H_

#include "ItfUpperBoundApproximation.h"
#include "SwingsPerPlayerAndCardinality.h"
#include "lint/ItfLargeNumberCalculator.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>Public Help \f$\xi\f$</i> power index (`PHX`).
 *
 * Let \f$W\f$ denote the set of winning coalitions and \f$W_i \subseteq W\f$ the set of winning coalitions containing player \f$i\f$.
 * Then the <i>Public Help \f$\xi\f$</i> index of player \f$i\f$ is defined as
 * \f[
 *		\xi_i = \frac{1}{\sum_{T \in W} \frac{1}{|T|}} \sum_{S \in W_i} \frac{1}{|S|^2}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>no extra output
 * 		<tr><td>`--filter-null / -f`	<td>INVALID - this power index is null player sensitive
 * </table>
 *
 * <b>Implementation note</b>:
 * If the mGame contains player of weight zero, they get excluded from the calculation. At the end the indices getting properly scaled as if the players of weight zero were part of the calculation.
 */
class PublicHelpXi : public SwingsPerPlayerAndCardinality {
public:
	/**
	 * Construct the PublicHelpXi object
	 *
	 * @param g The Game for which the PublicHelpXi index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to ItfLargeNumberCalculator::new_calculator()).
	 */
	PublicHelpXi(Game& g);

	std::vector<bigFloat> calculate(Game& g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game& g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_PUBLICHELPXI_H_ */
