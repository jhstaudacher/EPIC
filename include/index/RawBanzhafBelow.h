#ifndef EPIC_INDEX_RAWBANZHAFBELOW_H_
#define EPIC_INDEX_RAWBANZHAFBELOW_H_

#include "Array.h"
#include "ItfPowerIndex.h"
#include "ItfUpperBoundApproximation.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>raw Banzhaf</i> power index (`RBZB`) from below.
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
 * - calculation gets done from below (0 to quota) instead of from above (quota to weightsum)
 * - If the mGame contains player of weight zero, they get excluded from the calculation. At the end the corresponding players get assigned the index `0` (<i>null player</i> property) and the indices of the other players getting properly scaled as if the players of weight zero were part of the calculation.
 */
class RawBanzhafBelow : public ItfPowerIndex {
public:
	RawBanzhafBelow(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation = DEFAULT);
	~RawBanzhafBelow() override;

	std::vector<bigFloat> calculate() override;
	std::string getFullName() override;
	longUInt getMemoryRequirement() override;

protected:
	/**
	 * Calculating how often each player is a swing player.
	 *
	 * @param n_sp A return array. n_sp[x] will be the number how often player x is a swing player. The array must be allocated and zero-initialized at least in the range [0, mNonZeroPlayerCount - 1]!
	 */
	void numberOfTimesPlayerIsSwingPlayer(lint::LargeNumber n_sp[]);

	/**
	 * Calculating how often each player is a swing player.
	 *
	 * This function is the same as numberOfTimesPlayerIsSwingPlayer(lint::LargeNumber n_sp[]) but here you can pass the n_lc array. This is useful if the n_lc values are needed outside this function as well and is therefore already calculated.
	 * The n_lc array must be filled in the range [0, quota - 1].
	 *
	 * @param n_lc The array containing the number of losing coalitions. n_lc[x]: number of losing coalitions of weight x. The array must be filled in the range [0, quota - 1]!
	 * @param n_sp A return array. n_sp[x] will be the number how often player x is a swing player. The array must be allocated and zero-initialized at least in the range [0, mNonZeroPlayerCount - 1]!
	 */
	void numberOfTimesPlayerIsSwingPlayer(lint::LargeNumber n_lc[], lint::LargeNumber n_sp[]);

	/**
	 * Calculating the total number of all swing players
	 *
	 * This function calculates the sum over the n_sp array writing the result to total_sp.
	 *
	 * @param n_sp An Array containing the number of swing players. n_sp[x]: number of times player x is a swing player. This array must be filled in the range [0, mNonZeroPlayerCount - 1].
	 * @param total_sp A return variable containing the calculated sum
	 */
	void numberOfSwingPlayer(lint::LargeNumber n_sp[], lint::LargeNumber& total_sp);

private:
	/**
	 * Calculating the number of losing coalitions per coalition-weight.
	 *
	 * @param n_lc An array to store the calculation results. This array must be allocated and zero-initialized at least in the range [0, quota - 1]!
	 */
	void numberOfLosingCoalitionsPerWeight(lint::LargeNumber n_lc[]);
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_RAWBANZHAFBELOW_H_ */
