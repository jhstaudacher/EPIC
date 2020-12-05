#ifndef EPIC_INDEX_SWINGSPERPLAYERANDCARDINALITY_H_
#define EPIC_INDEX_SWINGSPERPLAYERANDCARDINALITY_H_

#include "Array.h"
#include "ItfPowerIndex.h"
#include "lint/LargeNumber.h"

namespace epic::index {

class SwingsPerPlayerAndCardinality : public ItfPowerIndex {
public:
	explicit SwingsPerPlayerAndCardinality(Game& g);

	longUInt getMemoryRequirement() override;

protected:
	/**
	 * Calculating the number of winning coalitions per weight and cardinality.
	 *
	 * @param n_wc A matrix where the calculated values will be stored. n_wc(x, y): number of winning coalitions of weight x and cardinality y.
	 *
	 * @note The n_wc matrix must be allocated and zero initialized at least in the range: [quota, weightsum] x [0, numberOfPlayers + 1]
	 */
	void numberOfWinningCoalitionsPerWeightAndCardinality(Array2dOffset<lint::LargeNumber>& n_wc);

	/**
	 * Calculating the number of winning coalitions per player and cardinality.
	 *
	 * @param raw_ssi The matrix where the calculated values will be stored. raw_ssi(x, y): number of winning coalitions of cardinality y, player x is a member of.
	 * @param only_swingplayer Defines if only swing players are taken into account. If false, all winning coalitions are considered (if the is pivotal or not).
	 *
	 * @note The raw_ssi matrix must be allocated and zero initialized in at least the range [0, numberOfPlayers] x [0, numberOfPlayers + 1]
	 * @note The values for players of weight zero will remain zero (as initialized).
	 */
	void swingsPerPlayerAndCardinality(Array2d<lint::LargeNumber>& raw_ssi, bool only_swingplayer = true);

	/**
	 * Calculating the number of winning coalitions per player and cardinality.
	 *
	 * This method offers the possibility to pass the calculated n_wc matrix (as it gets computed by the numberOfWinningCoalitionsPerWeightAndCardinality() method). This is useful if the n_wc values are needed outside of this method, so it must not get computed again.
	 *
	 * @param n_wc The already computed matrix containing the number of winning coalitions per weight and cardinality (as it is done by the numberOfWinningCoalitionsPerWeightAndCardinality() method).
	 * @param raw_ssi The matrix where the calculated values will be stored. raw_ssi(x, y): number of winning coalitions of cardinality y, player x is a member of.
	 * @param only_swingplayer Defines if only swing players are taken into account. If false, all winning coalitions are considered (if the is pivotal or not).
	 *
	 * @note The raw_ssi matrix must be allocated and zero initialized in at least the range [0, numberOfPlayers] x [0, numberOfPlayers + 1]
	 * @note The values for players of weight zero will remain zero (as initialized).
	 */
	void swingsPerPlayerAndCardinality(Array2dOffset<lint::LargeNumber>& n_wc, Array2d<lint::LargeNumber>& raw_ssi, bool only_swingplayer = true);
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_SWINGSPERPLAYERANDCARDINALITY_H_ */
