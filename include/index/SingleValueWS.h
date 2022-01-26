#ifndef EPIC_INDEX_SINGLEVALUEWS_H_
#define EPIC_INDEX_SINGLEVALUEWS_H_

#include "RawFelsenthal.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the single value \f$|W^s|\f$ (i.e. the number of minimal winning coalitions of smallest cardinality) where \f$W^s\f$ is the set of minimal winning coalitions of smallest cardinality. The calculation is based on the <i>raw Felsenthal</i> power index.
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>no extra output
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation (should not change the result)
 * </table>
 *
 * <b>Implementation note</b>:
 * If the mGame contains player of weight zero, they get always (not matter whether `-f` ist set) excluded from the calculation.
 */
class SingleValueWS : public RawFelsenthal {
public:
	/**
	 *
	 * @param g The Game-object for which the SingleValueWS should later be calculated
	 */
	SingleValueWS(Game* g);

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game* g) override;
};

} // namespace epic::index

#endif //EPIC_INDEX_SINGLEVALUEWS_H_
