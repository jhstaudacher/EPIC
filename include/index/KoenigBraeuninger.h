#ifndef EPIC_INDEX_KOENIGBRAEUNINGER_H_
#define EPIC_INDEX_KOENIGBRAEUNINGER_H_

#include "ItfUpperBoundApproximation.h"
#include "RawBanzhaf.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>König Bräuninger</i> power index (`KB`) based on the <i>raw Banzhaf</i> power index.
 *
 * Let \f$W\f$ denote the set of winning coalitions and \f$W_i \subseteq W\f$ the set of winning coalitions containing player \f$i\f$.
 * Then the <i>König Bräuninger</i> index of player \f$i\f$ is defined as
 * \f[
 *		KB_i = \frac{|W_i|}{|W|}
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
 * If the mGame contains player of weight zero, they get excluded from the calculation. At the end the corresponding players get assigned the index `0.5`.
 * 
 * The winning coalitions containing player \f$i\f$ are calculated using the Dubey-Shapley identity which is defined as
 *  \f[
 *		W_i = 0.5 *(\eta_i(\nu) + |W|)
 * \f]
 * where \f$\nu\f$ be a simple \f$n\f$-player mGame and \f$\eta_i(\nu)\f$ the number of coalitions for which \f$i\f$ is a critical player.
 */
class KoenigBraeuninger : public RawBanzhaf {
public:
	/**
	 * @param g The Game for which the KönigBräuninger index should later be calculated.
	 */
	KoenigBraeuninger(Game* g);

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_KOENIGBRAEUNINGER_H_ */
