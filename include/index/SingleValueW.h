#ifndef EPIC_INDEX_SINGLEVALUEW_H_
#define EPIC_INDEX_SINGLEVALUEW_H_

#include "RawBanzhaf.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the single value \f$|W|\f$ (i.e. the number of winning coalitions) where \f$W\f$ is the set of winning coalitions. The calculation is based on the <i>raw Banzhaf</i> power index.
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>no extra output
 * 		<tr><td>`--filter-null / -f`	<td>no special behaviour
 * </table>
 *
 * <b>Implementation note</b>:
 * If the mGame contains player of weight zero, they get excluded from the calculation. At the end the result gets properly scaled as if the players of weight zero were part of the calculation.
 */
class SingleValueW : public RawBanzhaf {
public:
	SingleValueW();

	std::vector<bigFloat> calculate(Game& g) override;
	std::string getFullName() override;
};

} // namespace epic::index

#endif //EPIC_INDEX_SINGLEVALUEW_H_
