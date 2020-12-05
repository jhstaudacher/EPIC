#ifndef EPIC_INDEX_ITFPOWERINDEX_H_
#define EPIC_INDEX_ITFPOWERINDEX_H_

#include "Game.h"
#include "lint/ItfLargeNumberCalculator.h"

namespace epic::index {

class ItfPowerIndex {
public:
	explicit ItfPowerIndex(Game& g)
		: mGame(g) {
		mCalculator = nullptr;
		mNonZeroPlayerCount = mGame.getNumberOfPlayers() - mGame.getNumberOfPlayersWithWeight0();
	};
	virtual ~ItfPowerIndex() = default;

	/**
	 * Calculating the power index for each player.
	 */
	virtual std::vector<bigFloat> calculate() = 0;

	/**
	 * A function to get the full name of the index.
	 *
	 */
	virtual std::string getFullName() = 0;

	/**
	 * Getting an approximated size of memory (RAM) needed for computation
	 *
	 * @return The approximated RAM size in Bytes. If 0 is returned the approximated memory is larger than 2^64 Bytes (does not fit in to a longUInt variable).
	 */
	virtual longUInt getMemoryRequirement() = 0;

protected:
	Game& mGame;

	/**
	 * A specialized Calculator object for operating with LargeNumber objects
	 */
	lint::ItfLargeNumberCalculator* mCalculator;

	/**
	 * The number of players in the mGame with weight > zero
	 */
	longUInt mNonZeroPlayerCount;

}; /* ItfPowerIndex */

} /* namespace epic::index */

#endif /* EPIC_INDEX_ITFPOWERINDEX_H_ */
