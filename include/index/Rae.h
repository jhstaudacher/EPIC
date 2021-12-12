#ifndef EPIC_INDEX_RAE_H_
#define EPIC_INDEX_RAE_H_

#include "ItfUpperBoundApproximation.h"
#include "RawBanzhaf.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>Rae</i> power index (`RA`) based on the <i>raw Banzhaf</i> power index.
 *
 * Let \f$W\f$ denote the set of winning coalitions and \f$W_i \subseteq W\f$ the set of winning coalitions containing player \f$i\f$.
 * Then the <i>Rae</i> index of player \f$i\f$ is defined as
 * \f[
 *		R_i = \frac{1}{2} + \frac{2 |W_i| - |W|}{2^n}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>prints how often each player is a swing player (raw Banzhaf)
 * 		<tr><td>`--filter-null / -f`	<td>INVALID - this power index is null player sensitive
 * </table>
 *
 * <b>Implementation note</b>:
 * If the mGame contains player of weight zero, they get excluded from the calculation (<i>Null player removable</i> property). At the end the corresponding players get assigned the index `0.5`. If the `-v` flag is set, the extra output gets scaled as if the players of weight `0` were part of the mGame.
 */
class Rae : public RawBanzhaf {
public:
	/**
	 * Construct the Rae object
	 *
	 * @param g The Game for which the Rae index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to ItfLargeNumberCalculator::new_calculator()).
	 */
	Rae(Game* g);

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_RAE_H_ */
