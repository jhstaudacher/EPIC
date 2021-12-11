#ifndef EPIC_INDEX_RAWPOWERINDEXF_H_
#define EPIC_INDEX_RAWPOWERINDEXF_H_

#include "ItfUpperBoundApproximation.h"
#include "SwingsPerPlayerAndCardinality.h"
#include "lint/ItfLargeNumberCalculator.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>raw null player free index \f$f^{np}\f$</i> (`RPIF`).
 *
 * Let \f$W_i^{np}\f$ denote the set of null player free winning coalitions containing player \f$i\f$.
 * Then the <i>raw null player free index \f$f^{np}\f$</i> of player \f$i\f$ is defined as
 * \f[
 *		f_i^{np} = \sum_{S \in W_i^{np}} \frac{1}{|S|}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>no extra output
 * 		<tr><td>`--filter-null / -f`	<td>required (if null players exits in the mGame) since the index is not defined for null players. Filtered null player get the index `0`.
 * </table>
 */
class RawPowerIndexF : public SwingsPerPlayerAndCardinality {
public:
	/**
	 * Construct the RawPowerIndexF object
	 * This index is basically Deegan-Packel, but not calculated with minimal winning coalitions alone, but with all null player free winning coalitions
	 * This index makes only sense, if null players are filtered out. See Alvarez-Mozos, M., Ferreira, F., Alonso-Meijide, J.M. and Pinto, A.A. (2015) Characterizations of power indices based on null player free winning coalitions for more information, especially the index f definition 4.1
	 *
	 * @param g The Game for which the RawPowerIndexF index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to ItfLargeNumberCalculator::new_calculator()).
	 */
	RawPowerIndexF(Game& g);

	std::vector<bigFloat> calculate(Game& g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game& g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;
};

} // namespace epic::index

#endif /* EPIC_INDEX_RAWPOWERINDEXF_H_ */
