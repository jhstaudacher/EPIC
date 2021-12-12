#ifndef EPIC_INDEX_POWERINDEXWITHWINNINGCOALITIONS_H_
#define EPIC_INDEX_POWERINDEXWITHWINNINGCOALITIONS_H_

#include "Array.h"
#include "ItfPowerIndex.h"

namespace epic::index {

class PowerIndexWithWinningCoalitions : public ItfPowerIndex {
protected:
	/**
	 * Calculating the number of winning coalitions for each players weight.
	 *
	 * Since this is the calculation from above, it only fills the values from n_wc[quota] to n_wc[weightsum].
	 * This is done by counting the possible emerging coalitions by looking at each players weight and go through the
	 * relevant ranges (see Algorithm 2 of Sascha Kurz's paper).
	 *
	 * @param n_wc An array of ChineseNumbers to store the calculation results. This array must be allocated and zero initialized at least in the range [quota, weightsum]!
	 */
	void numberOfWinningCoalitionsPerWeight(Game* g, ArrayOffset<lint::LargeNumber>& n_wc);

	/**
	 * Calculating the total number of possible winning coalitions.
	 *
	 * This method simply sums up all elements of the n_wc array.
	 *
	 * @param n_wc Number of winning coalitions per weight. The array must be filled in the range [quota, weightsum].
	 * @param total_wc Total number of all winning coalitions. The return parameter
	 */
	void numberOfWinningCoalitions(Game* g, ArrayOffset<lint::LargeNumber>& n_wc, lint::LargeNumber& total_wc);
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_POWERINDEXWITHWINNINGCOALITIONS_H_ */
