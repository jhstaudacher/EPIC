#ifndef EPIC_INDEX_RAWSHAPLEYSHUBIK_H_
#define EPIC_INDEX_RAWSHAPLEYSHUBIK_H_

#include "ItfUpperBoundApproximation.h"
#include "SwingsPerPlayerAndCardinality.h"
#include "lint/ItfLargeNumberCalculator.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>raw Shapley Shubik</i> power index (`RSH`).
 *
 * Let \f$\nu\f$ be a simple \f$n\f$-player mGame and \f$\eta_i(\nu)\f$ the number of coalitions for which \f$i\f$ is a critical player.
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
class RawShapleyShubik : public SwingsPerPlayerAndCardinality {
public:
	/**
	 * Construct the RawShapleyShubik object
	 *
	 * @param g The Game for which the RawShapleyShubik index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to ItfLargeNumberCalculator::new_calculator()).
	 */
	RawShapleyShubik();

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game* g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_RAWSHAPLEYSHUBIK_H_ */
