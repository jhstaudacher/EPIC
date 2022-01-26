#ifndef EPIC_GAME_H_
#define EPIC_GAME_H_

#include "types.h"

#include <string>
#include <utility>
#include <vector>

namespace epic {

/**
 * A class representing a permutation.
 *
 * Given a permutation at construction time, this class allow the application and reverse application of that permutation on other arrays. Even single array-indices can be converted into the corresponding index of the permuted array.
 */
class Permutation {
public:
	/**
	 * Default constructor representing a empty permutation, i.e. a permutation of length zero.
	 */
	Permutation()
		: mP(){};

	/**
	 * Constructor to represent the permutation given as parameter.
	 *
	 * @param permutation The permutation to represent.
	 *
	 * @note This class does not only hold a copy of the given permutation-vector but also a second vector of the same size for storing the inverse permutation.
	 */
	Permutation(const std::vector<size_t>& permutation)
		: mP(permutation) {
		mI.resize(mP.size());
		for (size_t i = 0; i < mP.size(); ++i) {
			mI[mP[i]] = i;
		}
	}

	/**
	 * Given the array-index of the original array a, this function returns the array-index for the permuted array b, both pointing to the same element, i.e. a[index] = b[applyIndex(index)].
	 *
	 * @param index An array-index of the original array
	 * @return The corresponding array-index of the permuted array
	 */
	size_t applyIndex(size_t index) const {
		return mI[index];
	}

	/**
	 * Permuting the values array and storing the result in the destination array.
	 *
	 * @tparam T The array's content type
	 * @param values The original array to be permuted (stays unchanged)
	 * @param destination The return array to store the result
	 *
	 * @note Both, the values and destination array must at least be as large as the permutation itself, i.e. |values| >= getSize() and |destination| >= getSize().  Even if the arrays are larger, only the first getSize() elements get processed.
	 */
	template<class T> void apply(const T* values, T* destination) const {
		for (size_t i = 0; i < mI.size(); ++i) {
			destination[mI[i]] = values[i];
		}
	}

	/**
	 * Permuting the values vector and storing the result in the destination vector.
	 *
	 * @tparam T The vector's content type
	 * @param values The original vector to be permuted (stays unchanged)
	 * @param destination The return vector to store the result
	 *
	 * @note Both, the values and destination vector must at least be as large as the permutation itself, i.e. values.size() >= getSize() and destination.size() >= getSize(). Only the first getSize() elements get processed.
	 */
	template<class T> void apply(const std::vector<T>& values, std::vector<T>& destination) const {
		for (size_t i = 0; i < getSize(); ++i) {
			destination[mI[i]] = values[i];
		}
	}

	/**
	 * Given the array-index of the permuted array b, this function returns the array-index for the original array a, both pointing to the same element, i.e. a[inverseIndex(index)] = b[index].
	 *
	 * @param index An array-index of the permuted array
	 * @return The corresponding array-index of the original array
	 */
	size_t inverseIndex(size_t index) const {
		return mP[index];
	}

	/**
	 * Inverse permuting the values array and storing the result in the destination array.
	 *
	 * @tparam T The array's content type
	 * @param values The original array to be permuted (stays unchanged)
	 * @param destination The return array to store the results
	 *
	 * @note Both, the values and destination array must at least be as large as the permutation itself, i.e. |values| >= getSize() and |destination| >= getSize(). Even if the arrays are larger, only the first getSize() elements get processed.
	 */
	template<class T> void reverse(const T* values, T* destination) const {
		for (size_t i = 0; i < mP.size(); ++i) {
			destination[mP[i]] = values[i];
		}
	}

	/**
	 * Inverse Permuting the values vector and storing the result in the destination vector.
	 *
	 * @tparam T The vector's content type
	 * @param values The original vector to be permuted (stays unchanged)
	 * @param destination The return vector to store the result
	 *
	 * @note Both, the values and destination vector must be as large as the permutation itself, i.e. value.size >= getSize() and destination.size() >= getSize(). Only the first getSize() elements get processed.
	 */
	template<class T> void reverse(const std::vector<T>& values, std::vector<T>& destination) const {
		for (size_t i = 0; i < getSize(); ++i) {
			destination[mP[i]] = values[i];
		}
	}

	/**
	 * Get the permutation size, i.e. the size of the array this permutation can get applied to.
	 *
	 * @return The permutation size
	 */
	size_t getSize() const {
		return mP.size();
	}

private:
	std::vector<size_t> mP; // permutation
	std::vector<size_t> mI; // inverse (permutation)
};

/**
 * A class to store and prepare important values for the computation of power indices
 */
class Game {
public:
	/**
	 * Constructor: Set variables according to the given parameters (set configuration of the mGame)
	 *
	 * @param quota A value between one and the sum of the weights of all players.
	 * @param weights A vector that includes the weights (each element represents a players weight)
	 * @param flag_filterOutNullPlayers A flag which decides whether null players should be sorted out, i.e. ignored during the power index computation (assign power index zero at the end)
	 */
	Game(longUInt quota, const std::vector<longUInt>& weights, bool flag_filterOutNullPlayers);

	/**
	 * Get the Permutation-object representing the sorting of the players weight (compared to the order given at construction time.
	 *
	 * @return The Permutation-object
	 */
	const Permutation& getPermutation() const;

	/**
	 * A function to get all player weights
	 *
	 * @return A vector of all weights
	 */
	const std::vector<longUInt>& getWeights() const;

	/**
	 * A function to get the quota
	 *
	 * @return The quota
	 */
	longUInt getQuota() const;

	/**
	 * A function to get the sum of all weights
	 *
	 * @return The sum of all weights
	 */
	longUInt getWeightSum() const;

	/**
	 * A function to get the number of players
	 *
	 * @return The amount of all players
	 */
	longUInt getNumberOfPlayers() const;

	/**
	 * A function to get the solution
	 *
	 * @return A vector of the calculation results
	 *
	 * @note The solution returned by this function is sorted to match the original order of the weights passed to the constructor.
	 * @note If the setSolution() function was not called before, this function returns a vector of size 0.
	 */
	std::vector<bigFloat> getSolution() const;

	/**
	 * A function to set the solution of the index calculation.
	 *
	 * The solution gets sorted in ascending order according of player index (matching the order of the weights passed to the constructor). Players that have been sorted out will be added again.
	 *
	 * @param solution A vector containing the results of index calculation (matching the order of the getWeights() vector)
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
	 * @return veto-player vector: getVetoPlayerVector()[i] == true <==> "player i is a veto player"
	 */
	const std::vector<bool>& getVetoPlayerVector() const;

	/**
	 * A function to get the number of veto players
	 *
	 * @return The amount of veto players
	 */
	longUInt getNumberOfVetoPlayers() const;

	/**
	 * A function to get the number of null players (including the players of weight zero)
	 *
	 * @return The amount of null players
	 */
	longUInt getNumberOfNullPlayers() const;

	/**
	 * Get the number of all players of weight greater than zero.
	 *
	 * @return The amount of players of weight greater than zero, i.e. getNumberOfPlayers() - getNumberOfPlayersWithWeight0()
	 */
	longUInt getNumberOfNonZeroPlayers() const;

	/**
	 * A function to get the number of players of weight zero
	 *
	 * @return The amount of players of weight zero
	 */
	longUInt getNumberOfPlayersWithWeight0() const;

	/**
	 * A function to get the null player handling flag
	 *
	 * @return Boolean value iff the null-player-handling flag is set
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
	std::vector<longUInt> mExcludedNullPlayer; // a list of null-player weights of that were filtered out

	longUInt mQuota; // The quota, i.e. minimal number of votes to win a Game

	longUInt mWeightSum; // The sum of all weights

	std::vector<bool> mPlayerIsVetoPlayer; // boolean vector holding which players are veto players (mPlayerIsVetoPlayer[i] == true <==> "player i is a veto player"

	longUInt mNumberOfVetoPlayers; // Total amount of veto players

	longUInt mNumberOfNullPlayers; // Total amount of null players (including those of weight zero)

	longUInt mNumberOfNonZeroPlayers; // Total amount of players of weight greater than zero

	std::vector<bigFloat> mSolution; // A vector to store the solution of the game

	bool mFlagNullPlayerHandling; // true iff the null-players are filtered out

	/**
	 * Sorting the internal weights (mWeights) in decreasing order
	 *
	 * @return The sorting permutation (sortWeights()[x] = the pre-sorted index of sorted index x)
	 */
	std::vector<size_t> sortWeights();

	/**
	 * A function to sort out the player, who are in no minimum winning coalitions
	 *
	 * @param flag_withoutNullPlayers Defines whether found null players should be removed from the weights vector or not
	 * @return the number of found null players
	 */
	longUInt findNullPlayersFromBelow(bool flag_withoutNullPlayers);

protected:
	std::vector<longUInt> mWeights; // The player's weights

	Permutation mPermutation; // The permutation describing the sorting process from unsorted to descending sorted weights
};							  /* class Game */

/**
 * Extending the Game class with important values for the computation of power indices with precoalitions.
 */
class PrecoalitionGame : public Game {
public:
	/**
	 * Constructor: Set variables according to the given parameters (set configuration of the mGame)
	 *
	 * @param quota a value between one and the sum of the weights of all players.
	 * @param weights a vector that includes the weights (each weight represents a players weight)
	 * @param flag_filterOutNullPlayers a flag bit which decides whether players with a weight of zero should be sorted out.
	 * @param precoalitions 2d vector representing the precoalitions, i.e. precoalitions[p][i] is the index (of the weights vector) of player i of precoalition p, having weight weights[precoalitions[p][i]]
	 *
	 */
	PrecoalitionGame(longUInt quota, std::vector<longUInt>& weights, bool flag_filterOutNullPlayers, std::vector<std::vector<int>>& precoalitions);

	/**
	 * A function to get the precoalition structure (2d vector, as passed to the constructor but updated to match the sorted weights of getWeights()-function)
	 */
	std::vector<std::vector<int>> getPrecoalitions() const;

	/**
	 * Get a vector of precoalition weights.
	 *
	 * The precoalition weight is the sum of all player's weights contained in that precoalition.
	 *
	 * @return The vector of the precoalition weights
	 */
	std::vector<longUInt> getPrecoalitionWeights() const;

	/**
	 * Get the number of precoalitions
	 *
	 * @return Total amount of precoalitions
	 */
	longUInt getNumberOfPrecoalitions() const;

	/**
	 * Get the number of players contained in the largest precoalition, i.e. containing the most players
	 *
	 * @return The maximum amount of players contained in a single precoalition
	 */
	longUInt getMaxPrecoalitionSize() const;

private:
	std::vector<std::vector<int>> mPrecoalitions; // precoalition structure (updated to match the sorted mWeights)

	std::vector<longUInt> mPrecoalitionWeights; // the (summed up) weights of each precoalition

	longUInt mMaxPrecoalitionSize; // maximum amount of players in a single precoalition

	/**
	 * Updaing the precoalitions vector to fit the sorted mWeights vector of the base class
	 */
	void sortPrecoalitions();
}; /* class PrecoalitionGame */

} /* namespace epic */

#endif /* EPIC_GAME_H_ */
