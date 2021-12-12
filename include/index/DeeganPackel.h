#ifndef EPIC_INDEX_DEEGANPACKEL_H_
#define EPIC_INDEX_DEEGANPACKEL_H_

#include "Array.h"
#include "ItfUpperBoundApproximation.h"
#include "RawDeeganPackel.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>Deegan Packel</i> power index (`DP`) based on the <i>raw Deegan Packel</i> power index.
 *
 * Let \f$W^m\f$ be the set of minimal winning coalitions and \f$W_i^m \subseteq W^m\f$ the set of minimal winning coalitions containing player \f$i\f$.
 * Then the <i>Deegan Packel</i> index of player \f$i\f$ is defined as
 * \f[
 *		DP_i = \frac{1}{|W^m|} \sum_{S \in W_i^m} \frac{1}{|S|}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>prints the total number of minimal winning coalitions
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation and assigns them the index 0
 * </table>
 */
class DeeganPackel : public RawDeeganPackel {
public:
	/*
	 * Construct the DeeganPackel object
	 *
	 * @param g The Game for which the DeeganPackel index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param low_memory A Flag to use a less memory consuming (but slower!) integer representation
	 *
	 * 	first iteration: formula f was compute for player n-1 already; other iteration: 'decrement' f for player n-2 ...
	 *  first iteration: formula b for player n-1 will be initialize; other iteration: 'increment' b for player n-2
	 *  each iteration: f(i-1,k,x) <=> f(k,x); b(i,k,x) <=> b(k,y) -> compute unscaled-DP[i] = f(i-1,k,x)*b(i,k,q-x) <=> f(k,x)* b(k,y)
	 *  DP[i]= unscaled-DP[i] / total-number-of-minimal-winning-coalitions
	 */
	DeeganPackel();

	std::vector<bigFloat> calculate(Game* g) override;
	std::string getFullName() override;

protected:
	/*
	 * A function to compute the total number of minimal winning coalitions
	 *
	 * @param total_mwc The total number of minimal winning coalitions
	 */
	void numberOfMinimalWinningCoalitions(Game* g, bigInt* total_mwc);
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_DEEGANPACKEL_H_ */
