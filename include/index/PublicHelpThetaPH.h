#ifndef EPIC_INDEX_PUBLICHELPTHETAPH_H_
#define EPIC_INDEX_PUBLICHELPTHETAPH_H_

#include "ItfUpperBoundApproximation.h"
#include "RawPublicHelpTheta.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>Public Help \f$\theta\f$</i> power index (`PHT`).
 *
 * Let \f$W_i\f$ denote the set of winning coalitions containing player \f$i\f$.
 * Then the <i>Public Help \f$\theta\f$</i> index of player \f$i\f$ is defined as
 * \f[
 *		\theta_i = \frac{|W_i|}{\sum_{k = 1}^n |W_k|}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>print the total number of winning coalitions as well as the number of winning coalitions each player is a member of (absolute Public Help \f$\theta\f$).
 * 		<tr><td>`--filter-null / -f`	<td>INVALID - this power index is null player sensitive
 * </table>
 *
 * <b>Implementation note</b>:
 * If the mGame contains player of weight zero, they get excluded from the calculation. At the end the indices get properly scaled as if the players of weight were part of the calculation. If the `-v` flag is set, the extra output gets scaled as if the players of weight `0` were part of the mGame.
 */
class PublicHelpThetaPH : public RawPublicHelpTheta {
public:
	/**
	 * Construct the PublicHelpTheta object
	 *
	 * @param g The Game for which the PublicHelpTheta index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to ItfLargeNumberCalculator::new_calculator()).
	 */
	PublicHelpThetaPH(Game* g);

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_PUBLICHELPTHETAPH_H_ */
