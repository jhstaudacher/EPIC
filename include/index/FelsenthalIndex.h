#ifndef EPIC_INDEX_FELSENTHALINDEX_H_
#define EPIC_INDEX_FELSENTHALINDEX_H_

#include "RawFelsenthal.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>Felsenthal</i> power index (`FT`) based on the <i>absolute Felsenthal</i> power index.
 *
 * Let \f$W^s\f$ be the set of minimal winning coalitions of smallest cardinality and \f$W_i^s \subseteq W^s\f$ the set of minimal winning coalitions of smallest cardinality containing player \f$i\f$.
 * Then the <i>Felsenthal</i> index of player \f$i\f$ is defined as
 * \f[
 *		PI_i = \frac{1}{|W^s|} \sum_{S \in W_i^s} \frac{1}{|S|}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>prints the total number of minimal winning coalitions of smallest cardinality as well as the number of minimal winning coalitions of smallest cardinality each player belongs to (absolute Felsenthal).
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation and assigns them the index 0
 * </table>
 *
 * <b>Implementation note</b>:
 * If the mGame contains player of weight zero, they get excluded from the calculation. At the end the corresponding players get assigned the index `0` (<i>null player</i> property).
 */
class FelsenthalIndex : public RawFelsenthal {
public:
	/**
	 * Construct the FelsenthalIndex object
	 *
	 * @param g The Game for which the FelsenthalIndex index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to ItfLargeNumberCalculator::new_calculator()).
	 *
	 * Scales the results calculated by the RawFelsenthal index.
	 */
	FelsenthalIndex(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation = DEFAULT);

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
	longUInt getMemoryRequirement() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_FELSENTHALINDEX_H_ */
