#ifndef EPIC_INDEX_ABSOLUTEBANZHAF_H_
#define EPIC_INDEX_ABSOLUTEBANZHAF_H_

#include "ItfUpperBoundApproximation.h"
#include "RawBanzhaf.h"
#include "lint/ItfLargeNumberCalculator.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>absolute Banzhaf</i> power index (`ABZ`) based on the <i>raw Banzhaf</i> power index.
 *
 * Let \f$\nu\f$ be a simple \f$n\f$-player mGame and \f$\eta_i(\nu)\f$ the number of coalitions for which \f$i\f$ is a critical player.
 * Then the <i>absolute Banzhaf</i> index of player \f$i\f$ is defined as
 * \f[
 *		\beta_i = \frac{\eta_i(\nu)}{2^{n - 1}}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>prints how often each player is a swing player (raw Banzhaf)
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation and assigns them the index 0
 * </table>
 *
 * <b>Implementation note</b>:
 * If the mGame contains player of weight zero, they get excluded from the calculation (<i>Null player removable</i> property). At the end the corresponding players get assigned the index `0` (<i>null player</i> property). If the `-v` flag is set, the extra output gets scaled as if the players of weight `0` were part of the mGame.
 *
 */
class AbsoluteBanzhaf : public RawBanzhaf {
public:
	AbsoluteBanzhaf();

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_ABSOLUTEBANZHAF_H_ */
