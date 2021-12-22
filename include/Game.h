#ifndef EPIC_GAME_H_
#define EPIC_GAME_H_

#include "types.h"

#include <string>
#include <utility>
#include <vector>

namespace epic {

class Permutation {
public:
	Permutation() : mP() {};

	Permutation(const std::vector<size_t>& permutation) : mP(permutation) {
		mI.reserve(mP.size());
		for (size_t i = 0; i < mP.size(); ++i) {
			mI[mP[i]] = i;
		}
	}

	size_t applyIndex(size_t index) const {
		return mI[index];
	}

	template<class T> void apply(const T* values, T* destination) const {
		for(size_t i = 0; i < mI.size(); ++i) {
			destination[mI[i]] = values[i];
		}
	}

	template<class T> void apply(const std::vector<T>& values, std::vector<T>& destination, T fill_value = 0) const {
		destination.reserve(mI.size()); // ensure at least the same size as mP

		size_t i = 0;
		size_t min_size = std::min(mI.size(), values.size());
		for (; i < min_size; ++i) {
			destination[mI[i]] = values[i];
		}

		for (; i < mI.size(); ++i) {
			destination[mI[i]] = fill_value;
		}
	}

	size_t inverseIndex(size_t index) const {
		return mP[index];
	}

	template<class T> void reverse(T* values, T* destination) const {
		for(size_t i = 0; i < mP.size(); ++i) {
			destination[mP[i]] = values[i];
		}
	}

	template<class T> void reverse(const std::vector<T>& values, std::vector<T>& destination, T fill_value = 0) const {
		destination.reserve(mP.size()); // ensure at least the same size as mP

		size_t i = 0;
		size_t min_size = std::min(mP.size(), values.size());
		for (; i < min_size; ++i) {
			destination[mP[i]] = values[i];
		}

		for (; i < mP.size(); ++i) {
			destination[mP[i]] = fill_value;
		}
	}

	private:
		std::vector<size_t> mP; // permutation
		std::vector<size_t> mI; // inverse (permutation)
};

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
	Game(longUInt quota, std::vector<longUInt>& weights, bool flag_filterOutNullPlayers);

	/**
	 * Sorting the weights in decreasing order
	 *
	 * @param weights containing the (unsorted) weights
	 * @return the permutation (return[x] = the pre-sorted index of sorted index x)
	 */
	std::vector<longUInt> sortWeights();

	const Permutation& getPermutation() const;

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

	longUInt getNumberOfNonZeroPlayers() const;

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
	 * Converting the player index (sorted weights) into the player number (input ordering).
	 *
	 * @note The indices are in the interval [0,n-1] while the player numbers are in the interval [1,n].
	 *
	 * @param index The player index of sorted weights
	 * @return The player number of original ordering
	 */
	longUInt playerIndexToNumber(longUInt index) const;

private:
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

	longUInt numberOfNonZeroPlayers;

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
	 * A function to sort out the player, who are in no minimum winning coalitions
	 *
	 * @param flag_withoutNullPlayers Defines whether found null players should be removed from the weights vector or not
	 * @return the number of found null players
	 */
	longUInt findNullPlayersFromBelow(bool flag_withoutNullPlayers);

protected:
	/**
	 * A vector of weights, contain the voting weight of each player
	 */
	std::vector<longUInt> weights;

	Permutation mPermutation;
}; /* class Game */

class PrecoalitionGame : public Game {
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
	PrecoalitionGame(longUInt quota, std::vector<longUInt>& weights, bool flag_filterOutNullPlayers, std::vector<std::vector<int>>& precoalitions);

	/**
	 * Sorting the weights in decreasing order
	 *
	 * @param weights containing the (unsorted) weights
	 * @return the permutation (return[x] = the pre-sorted index of sorted index x)
	 */
	void sortPrecoalitions();

	/**
	 * A function to get the precoalition structure
	 *
	 */
	std::vector<std::vector<int>> getPrecoalitions() const;

	std::vector<longUInt> getPrecoalitionWeights() const;

	longUInt getNumberOfPrecoalitions() const;

	longUInt getMaxPrecoalitionSize() const;

private:
	/**
	 * Structure of mPrecoalitions
	 */
	std::vector<std::vector<int>> mPrecoalitions;

	std::vector<longUInt> precoalitionWeights;

	longUInt mNumberOfPrecoalitions;

	longUInt mMaxPrecoalitionSize;
}; /* class PrecoalitionGame */

} /* namespace epic */

#endif /* EPIC_GAME_H_ */
