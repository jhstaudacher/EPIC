#ifndef EPIC_INDEX_RAWDEEGANPACKEL_H_
#define EPIC_INDEX_RAWDEEGANPACKEL_H_

#include "Array.h"
#include "ItfPowerIndex.h"
#include "ItfUpperBoundApproximation.h"

namespace epic::index {

/**
 * Implementing the ItfPowerIndex interface to calculate the <i>raw Deegan Packel</i> power index (`RDP`).
 *
 * Let \f$W_i^m\f$ be the set of minimal winning coalitions containing player \f$i\f$.
 * Then the <i>raw Deegan Packel</i> index of player \f$i\f$ is defined as
 * \f[
 *		DP_i^{raw} = \sum_{S \in W_i^m} \frac{1}{|S|}
 * \f]
 *
 * <table>
 * 		<caption>Behaviour on index specific CLI-flags</caption>
 * 		<tr><th>Flag<th>Behaviour
 * 		<tr><td>`--verbose / -v`		<td>no extra output
 * 		<tr><td>`--filter-null / -f`	<td>excludes all null-player from the calculation and assigns them the index 0
 * </table>
 *
 * <b>Implementation note</b>:
 * If the mGame contains player of weight zero, they get excluded from the calculation. At the end the corresponding players get assigned the index `0` (<i>null player</i> property).
 */
class RawDeeganPackel : public ItfPowerIndex {
public:
	/*
	 * Construct the RawDeeganPackel object
	 *
	 * @param g The Game for which the RawDeeganPackel index should be calculated.
	 * @param approx A specialized approximation object to approximate the largest needed numbers.
	 * @param low_memory A Flag to use a less memory consuming (but slower!) integer representation
	 *
	 * 	first iteration: formula f was compute for player n-1 already; other iteration: 'decrement' f for player n-2 ...
	 *  first iteration: formula b for player n-1 will be initialize; other iteration: 'increment' b for player n-2
	 *  each iteration: f(i-1,k,x) <=> f(k,x); b(i,k,x) <=> b(k,y) -> compute unscaled-DP[i] = f(i-1,k,x)*b(i,k,q-x) <=> f(k,x)* b(k,y)
	 *  DP[i]= unscaled-DP[i] / total-number-of-minimal-winning-coalitions
	 */
	RawDeeganPackel();

	std::vector<bigFloat> calculate(Game& g) override;
	std::string getFullName() override;
	longUInt getMemoryRequirement(Game& g) override;
	bigInt getMaxValueRequirement(ItfUpperBoundApproximation* approx) override;
	lint::Operation getOperationRequirement() override;
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_RAWDEEGANPACKEL_H_ */
