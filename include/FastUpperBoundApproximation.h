#ifndef EPIC_FASTUPPERBOUNDAPPROXIMATION_H_
#define EPIC_FASTUPPERBOUNDAPPROXIMATION_H_

#include "Game.h"
#include "ItfUpperBoundApproximation.h"

namespace epic {

/**
 * A class implementing the ItfUpperBoundApproximation interface with speed as first goal.
 *
 * The approximations done by this class are usually very fast but not as precise at they could possibly be.
 */
class FastUpperBoundApproximation : public ItfUpperBoundApproximation {
public:
	explicit FastUpperBoundApproximation(Game& g);

	bigInt upperBound_numberOfWinningCoalitionsPerWeight() override;
	bigInt upperBound_totalNumberOfSwingPlayer() override;
	bigInt upperBound_swingPlayerPerCardinality() override;

private:
	Game& mGame;
};

} /* namespace epic */

#endif /* EPIC_FASTUPPERBOUNDAPPROXIMATION_H_ */
