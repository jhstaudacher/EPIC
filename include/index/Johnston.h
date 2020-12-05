#ifndef EPIC_INDEX_JOHNSTON_H_
#define EPIC_INDEX_JOHNSTON_H_

#include "RawJohnston.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>Johnston</i> power index (`J`) based on the <i>raw Johnston</i> power index.
 *
 * Let \f$VC\f$ denote the set of vulnerable coalitions and let \f$Cr(S)\f$ be the set of critical players in a coalition \f$S\f$.
 * Then the <i>Johnston</i> index of player \f$i\f$ is defined as
 * \f[
 *		\gamma_i = \frac{\sum_{S \in VC, i \in Cr(S)} \frac{1}{|Cr(S)|}}{\sum_{k = 1}^n \sum_{S \in VC, k \in Cr(S)} \frac{1}{|Cr(S)|}}
 * \f]
 * The index for null player is `0`.
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>prints the total number of vulnerable coalitions as well as the number of vulnerable coalitions each player is a member of (raw Johnston).
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation and assigns them the index 0
 * </table>
 */
class Johnston : public RawJohnston {
public:
	/**
	 * Construct the Johnston object
	 *
	 * @param g The Game for which the Johnston index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to ItfLargeNumberCalculator::new_calculator()).
	 */
	Johnston(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation = DEFAULT);

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_JOHNSTON_H_ */
