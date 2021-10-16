#ifndef EPIC_INDEX_SHAPLEYSHUBIKBELOW_H_
#define EPIC_INDEX_SHAPLEYSHUBIKBELOW_H_

#include "ItfUpperBoundApproximation.h"
#include "RawShapleyShubikBelow.h"
#include "lint/ItfLargeNumberCalculator.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>Shapley Shubik</i> power index from below (`SHB`) based on the <i>raw Shapley Shubik</i> power index from below.
 *
 * Let \f$\nu\f$ be a simple \f$n\f$-player mGame and \f$\eta_i(\nu)\f$ the number of coalitions for which \f$i\f$ is a critical player.
 * Then the <i>Shapley Shubik</i> index of player \f$i\f$ is defined as
 * \f[
 *		\phi_i = \sum_{c = 1}^n \frac{\eta_i(\nu, c)}{c{n \choose c}}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>no extra output
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation and assigns them the index 0
 * </table>
 */
class ShapleyShubikBelow : public RawShapleyShubikBelow {
public:
	/**
	 * Construct the ShapleyShubikBelow object
	 *
	 * @param g The Game for which the ShapleyShubik index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to ItfLargeNumberCalculator::new_calculator()).
	 */
	ShapleyShubikBelow(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation = DEFAULT);

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_SHAPLEYSHUBIKBELOW_H_ */
