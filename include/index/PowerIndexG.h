#ifndef EPIC_INDEX_POWERINDEXG_H_
#define EPIC_INDEX_POWERINDEXG_H_

#include "RawPowerIndexG.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>null player free index \f$g^{np}\f$</i> (`PIG`) based on the <i>raw null player free index \f$g^{np}\f$</i>.
 *
 * Let \f$W_i^{np}\f$ denote the set of null player free winning coalitions containing player \f$i\f$.
 * Then the <i>null player free index \f$g^{np}\f$</i> of player \f$i\f$ is defined as
 * \f[
 *		g_i^{np} = \frac{|W_i^{np}|}{\sum_{k = 1}^n |W_k^{np}|}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>print the total number of (null player free) winning coalitions as well as the number of (null player free) winning coalitions each player is a member of (raw null player free index g).
 * 		<tr><td>`--filter-null / -f`	<td>required (if null players exits in the mGame) since the index is not defined for null players. Filtered null player get the index `0`.
 * </table>
 * 
 * <b>Implementation note</b>:
 * The winning coalitions containing player \f$i\f$ are calculated using the Dubey-Shapley identity which is defined as
 *  \f[
 *		W_i^{np} = 0.5 *(\eta_i(\nu) + |W^{np}|)
 * \f]
 * where \f$\nu\f$ be a simple \f$n\f$-player mGame and \f$\eta_i(\nu)\f$ the number of coalitions for which \f$i\f$ is a critical player.
 */
class PowerIndexG : public RawPowerIndexG {
public:
	/**
	 * Construct the PowerIndexG object
	 * This index is basically Public Good, but not calculated with minimal winning coalitions alone, but with all null player free winning coalitions
	 * This index makes only sense, if null players are filtered out. See Alvarez-Mozos, M., Ferreira, F., Alonso-Meijide, J.M. and Pinto, A.A. (2015) Characterizations of power indices based on null player free winning coalitions for more information, especially the index g definition 4.2
	 *
	 * @param g The Game for which the PowerIndexG index should later be calculated.
	 */
	PowerIndexG(Game* g);

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
};

} // namespace epic::index

#endif /* EPIC_INDEX_POWERINDEXG_H_ */
