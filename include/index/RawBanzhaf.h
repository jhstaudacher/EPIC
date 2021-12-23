#ifndef EPIC_INDEX_RAWBANZHAF_H_
#define EPIC_INDEX_RAWBANZHAF_H_

#include "ItfUpperBoundApproximation.h"
#include "PowerIndexWithWinningCoalitions.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>raw Banzhaf</i> power index (`RBZ`).
 *
 * Let \f$\nu\f$ be a simple \f$n\f$-player mGame and \f$\eta_i(\nu)\f$ the number of coalitions for which \f$i\f$ is a critical player.
 * Then the <i>raw Banzhaf</i> index of player \f$i\f$ is defined as
 * \f[
 *		\beta_i^{raw} = \eta_i(\nu)
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>no extra output
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation and assigns them the index 0
 * </table>
 *
 * <b>Implementation notes</b>:
 * - calculation gets done from above (weightsum to quota) instead of from below (0 to quota) to save time and memory
 * - If the mGame contains player of weight zero, they get excluded from the calculation. At the end the corresponding players get assigned the index `0` (<i>null player</i> property) and the indices of the other players getting properly scaled as if the players of weight zero were part of the calculation.
 */
class RawBanzhaf : public PowerIndexWithWinningCoalitions {
public:
	RawBanzhaf();

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game* g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;

protected:
	/**
	 * Calculating how often each player is a swing player.
	 *
	 * @param g The Game object for the current calculation
	 * @param n_sp A return array. n_sp[x] will be the number how often player x is a swing player. The array must have enough memory for at least g.getNumberOfNonZeroPlayers() entries. Each entry must be initialized with zero!
	 */
	void numberOfTimesPlayerIsSwingPlayer(Game* g, lint::LargeNumber n_sp[]);

	/**
	 * Calculating how often each player is a swing player.
	 *
	 * This function is the same as numberOfTimesPlayerIsSwingPlayer(lint::ChineseNumber n_sp[]) but here you can pass the n_wc array. This is useful if the n_wc values are needed outside this function as well and is therefore already calculated.
	 * The n_wc array must be filled in the range [quota, weightsum].
	 *
	 * @param g The Game object for the current calculation
	 * @param n_wc The array containing the number of winning coalitions. n_wc[x]: number of winning coalitions of weight x. The array must be filled in the range [quota, weightsum]!
	 * @param n_sp A return array. n_sp[x] will be the number how often player x is a swing player. The array must have enough memory for at least g.getNumberOfNonZeroPlayers() entries. Each entry must be initialized with zero!
	 */
	void numberOfTimesPlayerIsSwingPlayer(Game* g, ArrayOffset<lint::LargeNumber>& n_wc, lint::LargeNumber n_sp[]);

	/**
	 * Calculating the total number of all swings
	 *
	 * This function sums up the array calculated by one of the numberOfTimesPlayerIsSwingPlayer() functions.
	 *
	 * @param g The Game object for the current calculation
	 * @param n_sp An Array containing the number of swing players. n_sp[x]: number of times player x is a swing player. This array must be filled in the range [0, g.getNumberOfNonZeroPlayers()].
	 * @param total_sp A return parameter containing the calculated sum
	 */
	void numberOfSwingPlayer(Game* g, lint::LargeNumber n_sp[], lint::LargeNumber& total_sp);
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_RAWBANZHAF_H_ */
