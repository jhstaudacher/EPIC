#ifndef EPIC_INDEX_TIJS_H_
#define EPIC_INDEX_TIJS_H_

#include "ItfPowerIndex.h"
#include "ItfUpperBoundApproximation.h"
#include "lint/ItfLargeNumberCalculator.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>Tijs</i> power index (`T`).
 *
 * Let \f$V\f$ denote the set of veto players.
 * Then the <i>Tijs</i> index of player \f$i\f$ is defined as
 *
 * \f[\tau_i = \left\{
 * 		\begin{array}{ll}
 *			\frac{1}{|V|} & if ~ i \in V\\
 *			0 & otherwise
 *		\end{array}
 *		\color{white}\right\}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>prints the amount of veto players
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation and assigns them the index `0` (since none of them can be veto player).
 * </table>
 */
class Tijs : public ItfPowerIndex {
public:
	explicit Tijs();

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game* g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_TIJS_H_ */
