#ifndef EPIC_INDEX_RAWPOWERINDEXG_H_
#define EPIC_INDEX_RAWPOWERINDEXG_H_

#include "ItfUpperBoundApproximation.h"
#include "index/RawBanzhaf.h"
#include "lint/ItfLargeNumberCalculator.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>raw null player free index \f$g^{np}\f$</i> (`RPIG`) based on the <i>raw Banzhaf</i> power index.
 *
 * Let \f$W_i^{np}\f$ denote the set of null player free winning coalitions containing player \f$i\f$.
 * Then the <i>raw null player free index \f$g^{np}\f$</i> of player \f$i\f$ is defined as
 * \f[
 *		g_i^{np} = |W_i^{np}|
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>no extra output
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
class RawPowerIndexG : public RawBanzhaf {
public:
	/**
	 * Construct the RawPowerIndexG object
	 * This index is basically Public Good, but not calculated with minimal winning coalitions alone, but with all null player free winning coalitions
	 * This index makes only sense, if null players are filtered out. See Alvarez-Mozos, M., Ferreira, F., Alonso-Meijide, J.M. and Pinto, A.A. (2015) Characterizations of power indices based on null player free winning coalitions for more information, especially the index g definition 4.2
	 *
	 * @param g The Game for which the RawPowerIndexG index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to ItfLargeNumberCalculator::new_calculator()).
	 */
	RawPowerIndexG(Game* g);

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
};

} // namespace epic::index

#endif /* EPIC_INDEX_RAWPOWERINDEXG_H_ */
