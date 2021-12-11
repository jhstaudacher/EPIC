#ifndef EPIC_INDEX_SINGLEVALUEWM_H_
#define EPIC_INDEX_SINGLEVALUEWM_H_

#include "DeeganPackel.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the single value \f$|W^m|\f$ (i.e. the number of minimal winning coalitions) where \f$W^m\f$ is the set of minimal winning coalitions. The calculation is based on the <i>Deegan Packel</i> power index.
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>no extra output
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation (should not change the result)
 * </table>
 */
class SingleValueWM : public DeeganPackel {
public:
	SingleValueWM();

	std::vector<bigFloat> calculate(Game& g) override;
	std::string getFullName() override;
};

} // namespace epic::index

#endif //EPIC_INDEX_SINGLEVALUEWM_H_
