#ifndef EPIC_INDEX_HARSANYINASHINDEX_H_
#define EPIC_INDEX_HARSANYINASHINDEX_H_

#include "ItfPowerIndex.h"
#include "ItfUpperBoundApproximation.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>Harsanyi-Nash</i> power index (`HN`).
 *
 * Let \f$V\f$ denote the set of veto players.
 * Then the <i>Harsanyi-Nash</i> index of player \f$i\f$ is defined as
 * \f[HN_i = \left\{
 * 		\begin{array}{ll}
 *			\frac{1}{n} & if ~ |V| \neq 1\\
 *			\left\{\begin{array}{ll}
 *				1 & if ~ i \in V\\
 *				0 & otherwise
 *			\end{array}\color{white}\right\} & \color{black} otherwise
 * 		\end{array}
 * 		\color{white}\right\}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>no extra output
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation and assigns them the index 0. Note that this will change the indices of the remaining players!
 * </table>
 *
 * <b>Implementation note</b>:
 * If the mGame contains player of weight zero, they get excluded from the calculation (<i>Null player removable</i> property). At the end the corresponding players get assigned the index `0` (<i>null player</i> property). If the `-v` flag is set, the extra output gets scaled as if the players of weight `0` were part of the mGame.
 */
class HarsanyiNashIndex : public ItfPowerIndex {
public:
	/**
	 * Construct the HarsanyiNash object
	 *
	 * @param g The Game for which the HarsanyiNash index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 */
	HarsanyiNashIndex(Game& g);

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
	longUInt getMemoryRequirement() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_HARSANYINASHINDEX_H_ */
