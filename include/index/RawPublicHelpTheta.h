#ifndef EPIC_INDEX_RAWPUBLICHELPTHETA_H_
#define EPIC_INDEX_RAWPUBLICHELPTHETA_H_

#include "ItfUpperBoundApproximation.h"
#include "PowerIndexWithWinningCoalitions.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>absolute Public Help \f$\theta\f$</i> power index (`PHA`).
 *
 * Let \f$W_i\f$ denote the set of winning coalitions containing player \f$i\f$.
 * Then the <i>absolute Public Help \f$\theta\f$</i> index of player \f$i\f$ is defined as
 * \f[
 *		\theta_i = |W_i|
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>no extra output
 * 		<tr><td>`--filter-null / -f`	<td>INVALID - this power index is null player sensitive
 * </table>
 *
 * <b>Implementation note</b>:
 * If the mGame contains player of weight zero, they get excluded from the calculation. At the end the indices get properly scaled as if the players of weight were part of the calculation.
 * 
 * The winning coalitions containing player \f$i\f$ are calculated using the Dubey-Shapley identity which is defined as
 *  \f[
 *		W_i = 0.5 *(\eta_i(\nu) + |W|)
 * \f]
 * where \f$\nu\f$ be a simple \f$n\f$-player mGame and \f$\eta_i(\nu)\f$ the number of coalitions for which \f$i\f$ is a critical player.
 */
class RawPublicHelpTheta : public PowerIndexWithWinningCoalitions {
public:
	/**
	 * @param g The Game-object for which the RawPublicHelpTheta index should later be calculated
	 */
	RawPublicHelpTheta(Game* g);

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game* g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;

protected:
	/**
	 * Calculating the number of winning coalitions each player is a member of.
	 *
	 * The other major difference is: instead of looping over all coalitions the player would turn into losing ones, we step over all winning coalitions the player is a member of (up to weightsum).
	 *
	 * The used algorithm is nearly the same as in the PowerIndexWithWinningCoalitionsAndSwingPlayers::numberOfTimesPlayerIsSwingPlayer() function. One difference is, that players of weight zero getting calculated separately since each player of them is a member of half of all possible winning coalitions.
	 *
	 * @param g The Game object for the current calculation
	 * @param wci A return array. wci[x] will be the number of winning coalitions player x is a member of when the function returns. The array must have enough memory for at least numberOfPlayers entries.
	 * @param total_wc A optional return parameter. If it is not a nullptr it will contain the total number of winning coalitions when the function returns.
	 */
	void winningCoalitionsForPlayer(Game* g, bigInt wci[], bigFloat* total_wc = nullptr);
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_RAWPUBLICHELPTHETA_H_ */
