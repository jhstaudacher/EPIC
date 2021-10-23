#ifndef EPIC_GAME_H_
#define EPIC_GAME_H_

#include "types.h"

#include <string>
#include <utility>
#include <vector>

namespace epic {

/**
 * A class to prepare and store important variables for the computation of power indices
 *
 *
 */
class Game {
public:
	/**
	 * Constructor: Set variables according to the given parameters (set configuration of the mGame)
	 *
	 * @param quota a value between one and the sum of the weights of all players.
	 * @param weights a vector that includes the weights (each weight represents a players weight)
	 * @param flag_filterOutNullPlayers a flag bit which decides whether players with a weight of zero should be sorted out.
	 * @param flag_verbose a flag causing extra verbose output if set.
	 *
	 */
	Game(longUInt quota, std::vector<longUInt>& weights, bool flag_filterOutNullPlayers, std::vector<std::vector<int>> &precoalitions);

	/**
	 * A function to get all weights
	 *
	 * @return A vector of all weights
	 */
	const std::vector<longUInt>& getWeights() const;

	/**
	 * A function to get the quota
	 *
	 */
	longUInt getQuota() const;

	/**
	 * A function to get the sum of all weights
	 *
	 */
	longUInt getWeightSum() const;

	/**
	 * A function to get the number of players
	 *
	 */
	longUInt getNumberOfPlayers() const;

	/**
	 * A function to get the solution
	 *
	 * @return A vector of all results
	 */
	std::vector<bigFloat> getSolution() const;

	/**
	 * A function to set the solution of the index calculation. The solution would be sorted in ascending order according of player index.
	 * 	Players that have been sorted out will be added again.
	 *
	 * @param solution as vector, which contains the results of index calculation
	 *
	 */
	void setSolution(const std::vector<bigFloat>& solution);

	/**
	 * A function to set the solution of a single value calculation (e.g. number of winning coalitions).
	 *
	 * @param solution The calculated value
	 */
	void setSingleValueSolution(const bigFloat& value);

	/**
	 * A function to get the veto player boolean vector
	 *
	 */
	const std::vector<bool>& getVetoPlayerVector() const;

	/**
	 * A function to get the veto player count
	 *
	 */
	longUInt getNumberOfVetoPlayers() const;

	/**
	 * A function to get the null player count (including the players of weight zero)
	 *
	 */
	longUInt getNumberOfNullPlayers() const;

	/**
	 * A function to get the number of players of weight zero
	 */
	longUInt getNumberOfPlayersWithWeight0() const;

	/**
	 * A function to get the null player handling flag
	 *
	 */
	bool getFlagNullPlayerHandling() const;

	/**
	 * A function to get the precoalition structure
	 *
	 */
	std::vector<std::vector<int>> getPrecoalitions() const;

	/**
	 * Converting the player index (sorted weights) into the player number (input ordering).
	 *
	 * @note The indices are in the interval [0,n-1] while the player numbers are in the interval [1,n].
	 *
	 * @param index The player index of sorted weights
	 * @return The player number of original ordering
	 */
	longUInt playerIndexToNumber(longUInt index) const;

private:
	/**
	 * A vector of weights, contain the voting weight of each player
	 */
	std::vector<longUInt> weights;

	std::vector<longUInt> excludedNullPlayer;

	/**
	 * The number of quota (minimal number of votes to win a mGame)
	 */
	longUInt quota;

	/**
	 * The sum of all players/voters voting weights
	 */
	longUInt weightSum;

	/**
	 * The number of players/voters
	 */
	longUInt numberOfPlayers;

	/**
	 * Structure of precoalitions
	 */
	std::vector<std::vector<int>> precoalitions;

	/**
	 * A vector of booleans which holds which players are veto players
	 */
	std::vector<bool> playerIsVetoPlayer;

	/**
	 * An integer holding how many veto players there are
	 */
	longUInt numberOfVetoPlayers;

	/**
	 * An integer holding how many null players there are (including the players of weight 0)
	 */
	longUInt numberOfNullPlayers;

	/**
	 * An integer holding how many players of weight 0 there are
	 */
	longUInt numberOfPlayersOfWeight0;

	/**
	 * A vector, which contain the solution of the computation
	 */
	std::vector<bigFloat> solution;

	/**
	 * If true, null players are to be sorted out.
	 */
	bool flag_null_player_handling;

	/**
	 * Storing the permutation that gets applied to sort the weights by decreasing weights.
	 * sortingPermutation[x] is the original Player number of sorted index x.
	 */
	std::vector<longUInt> sortingPermutation;

	/**
	 * Sorting the weights in decreasing order
	 *
	 * @param weights containing the (unsorted) weights
	 * @return the permutation (return[x] = the pre-sorted index of sorted index x)
	 */
	static std::vector<longUInt> sortWeights(std::vector<longUInt>& weights, std::vector<std::vector<int>>& precoalitions);

	/**
	 * A function to sort out the player, who are in no minimum winning coalitions
	 *
	 * @param flag_withoutNullPlayers Defines whether found null players should be removed from the weights vector or not
	 * @return the number of found null players
	 */
	longUInt findNullPlayersFromBelow(bool flag_withoutNullPlayers);

}; /* class Game */

} /* namespace epic */

#endif /* EPIC_GAME_H_ */
