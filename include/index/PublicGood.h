#ifndef EPIC_INDEX_PUBLICGOOD_H_
#define EPIC_INDEX_PUBLICGOOD_H_

#include "RawPublicGood.h"

#include "lint/LargeNumber.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>Public Good (Holler)</i> power index (`PG`) based on the <i>absolute Public Good</i> power index.
 *
 * Let \f$W_i^m\f$ denote the set of minimal winning coalitions containing player \f$i\f$.
 * Then the <i>Public Good</i> index of player \f$i\f$ is defined as
 * \f[
 *		h_i = \frac{|W_i^m|}{\sum_{k = 1}^n |W_k^m|}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>print the total number of minimal winning coalitions as well as the number of minimal winning coalitions each player is a member of (absolute Public Good).
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation and assigns them the index 0
 * </table>
 *
 * <b>Implementation note</b>:
 * If the mGame contains player of weight zero, they get excluded from the calculation. At the end the corresponding players get assigned the index `0` (<i>null player</i> property).
 */
class PublicGood : public RawPublicGood {
public:
	std::vector<bigFloat> calculate(Game* g) override;

	std::string getFullName() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_PUBLICGOOD_H_ */
