#ifndef EPIC_INDEX_POWERINDEXWITHPRECOALITIONS_H_
#define EPIC_INDEX_POWERINDEXWITHPRECOALITIONS_H_

#include "ItfPowerIndex.h"
#include "ItfUpperBoundApproximation.h"
#include "Array.h"

namespace epic::index {

class PowerIndexWithPrecoalitions : public ItfPowerIndex {
public:
	explicit PowerIndexWithPrecoalitions(Game& g);
	~PowerIndexWithPrecoalitions();

protected:
	/**
	 * Array containing the precoalition weights
	 *
	 * mPartW[x]: the weight of precoalition x
	 */
	longUInt* mPartW;

	/**
	 * The amount of Precoalitions in the current game
	 */
	longUInt mNbPart;

	/**
	 * The amount of players contained in the largest precoalition.
	 */
	longUInt mMaxPartSize;


	/**
	 * Calculating the number of coalitions containing player <i>i</i>
	 *
	 * @param cw return array containing the calculated results
	 * @param cc number of coalitions per weight (like calculated by generalizedBackwardCountingPerWeight()-method)
	 * @param wi the weight of player <i>i</i>
	 */
	void coalitionsContainingPlayerFromAbove(ArrayOffset<lint::LargeNumber>& cw, ArrayOffset<lint::LargeNumber>& cc, longUInt wi);

	/**
	 * Calculating the number of coalitions per weight
	 *
	 * c[x]: number of coalitions of weight <i>x</i>
	 *
	 * @param c return array containing the calculated results
	 * @param weights array of player weights - must be allocated and filled in the range [0, n-1]
	 * @param n player amount
	 */
	void generalizedBackwardCountingPerWeight(ArrayOffset<lint::LargeNumber>& c, longUInt* weights, longUInt n);

	/**
	 * Calculates the number of coalitions and cardinalities containing player <i>i</i>.
	 *
	 * @param cw return matrix containing the calculated results
	 * @param cc number of coalitions per weight and cardinality (like calculated by generalizedBackwardCountingPerWeightAndCardinality()-method)
	 * @param n player amount
	 * @param p player index - must be in the range [0; n-1]
	 * @param weights array of player weights - must be allocated and filled in the range [0; n-1]
	 *
	 * @note This method can be used for external games as well. In this case <i>n</i> is the precoalition amount, <i>p</i> the precoalition index and <i>weights</i> an array of precoalition weights.
	 */
	void coalitionsCardinalityContainingPlayerFromAbove(Array2dOffset<lint::LargeNumber>& cw, Array2dOffset<lint::LargeNumber>& cc, longUInt n, longUInt p, longUInt* weights);

	/**
	 * Calculates the number of coalitions per weight and cardinality
	 *
	 * cc(x, y): number of coalitions of weight <i>x</i> and cardinality <i>y</i>
	 *
	 * @param cc return matrix containing the calculated results
	 * @param weights array of player weights - must be allocated and filled in the range [0; n-1]
	 * @param n player amount
	 */
	void generalizedBackwardCountingPerWeightCardinality(Array2dOffset<lint::LargeNumber>& cc, longUInt* weights, longUInt n);
};

} /* namespace epic::index */

#endif /* EPIC_INDEX_POWERINDEXWITHPRECOALITIONS_H_ */
