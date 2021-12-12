#ifndef EPIC_INDEX_RAWPUBLICHELPTHETASD_H_
#define EPIC_INDEX_RAWPUBLICHELPTHETASD_H_

#include "ItfUpperBoundApproximation.h"
#include "RawBanzhaf.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>absolute Public Help \f$\theta\f$</i> power index (`PHASD`) based on the <i>raw Banzhaf</i> power index.
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
 */
class RawPublicHelpThetaSD : public RawBanzhaf {
public:
	RawPublicHelpThetaSD(Game* g);

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;

protected:
	/**
	 * Calculating the number of winning coalitions each player is a member of using the Shapley-Dubey Identity.
	 * 	 
	 * @param wci A return array. wci[x] will be the number of winning coalitions player x is a member of when the function returns. The array must have enough memory for at least numberOfPlayers entries.
	 */
	void winningCoalitionsForPlayer(Game* g, bigInt wci[]);
};

} // namespace epic::index

#endif /* EPIC_INDEX_RAWPUBLICHELPTHETASD_H_ */
