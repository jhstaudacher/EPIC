#ifndef EPIC_INDEX_POWERINDEXF_H_
#define EPIC_INDEX_POWERINDEXF_H_

#include "ItfUpperBoundApproximation.h"
#include "SwingsPerPlayerAndCardinality.h"
#include "lint/ItfLargeNumberCalculator.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>null player free index \f$f^{np}\f$</i> (`PIF`) based on the <i>raw null player free index \f$f^{np}\f$</i>.
 *
 * Let \f$W^{np}\f$ denote the set of null player free winning coalitions and \f$W_i^{np} \subseteq W^{np}\f$ the set of null player free winning coalitions containing player \f$i\f$.
 * Then the <i>null player free index \f$f^{np}\f$</i> of player \f$i\f$ is defined as
 * \f[
 *		f_i^{np} = \frac{1}{|W^{np}|} \sum_{S \in W_i^{np}} \frac{1}{|S|}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>print the total number of (null player free) winning coalitions
 * 		<tr><td>`--filter-null / -f`	<td>required (if null players exits in the mGame) since the index is not defined for null players. Filtered null player get the index `0`.
 * </table>
 */
class PowerIndexF : public SwingsPerPlayerAndCardinality {
public:
	/**
	 * Construct the PowerIndexF object
	 * This index is basically Deegan-Packel, but not calculated with minimal winning coalitions alone, but with all null player free winning coalitions
	 * This index makes only sense, if null players are filtered out. See Alvarez-Mozos, M., Ferreira, F., Alonso-Meijide, J.M. and Pinto, A.A. (2015) Characterizations of power indices based on null player free winning coalitions for more information, especially the index f definition 4.1
	 *
	 * @param g The Game for which the PowerIndexF index should later be calculated.
	 */
	PowerIndexF(Game* g);

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game* g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;
};

} // namespace epic::index

#endif /* EPIC_INDEX_POWERINDEXF_H_ */
