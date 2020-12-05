#ifndef EPIC_ITFUPPERBOUNDAPPROXIMATION_H_
#define EPIC_ITFUPPERBOUNDAPPROXIMATION_H_

#include "types.h"

namespace epic {

class ItfUpperBoundApproximation {
public:
	virtual ~ItfUpperBoundApproximation() = default;

	/**
	 * Calculating the upper bound of the number of winning coalitions of a single weight.
	 *
	 * @return The approximated value
	 */
	virtual bigInt upperBound_numberOfWinningCoalitionsPerWeight() = 0;

	/**
	 * Calculating the upper bound of the sum how often each player is a swing player.
	 *
	 * @return The approximated value
	 */
	virtual bigInt upperBound_totalNumberOfSwingPlayer() = 0;

	/**
	 * Calculating the upper bound of the the number how often a player is a swing player for a coalition of fixed cardinality.
	 *
	 * @return The approximated value
	 */
	virtual bigInt upperBound_swingPlayerPerCardinality() = 0;
};

} /* namespace epic */

#endif /* EPIC_ITFUPPERBOUNDAPPROXIMATION_H_ */
