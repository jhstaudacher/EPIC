#include "Game.h"

#include "Logging.h"

#include <algorithm>
#include <stdexcept>

epic::Game::Game(longUInt quota, std::vector<longUInt>& untreated_weights, bool flag_withoutNullPlayers) {
	this->quota = quota;
	this->solution = {};
	this->flag_null_player_handling = flag_withoutNullPlayers;

	//sort players by weight
	weights = untreated_weights;
	mPermutation = Permutation(sortWeights());

	//handleDummyPlayers(flag_withoutNullPlayers);	//handle dummy-/null-player
	numberOfNullPlayers = findNullPlayersFromBelow(flag_withoutNullPlayers);

	weightSum = 0;
	for (auto it : weights) {
		weightSum += it;
	}
	numberOfPlayers = weights.size();

	numberOfPlayersOfWeight0 = weights.end() - std::find(weights.begin(), weights.end(), 0);
	numberOfNonZeroPlayers = numberOfPlayers - numberOfPlayersOfWeight0;

	// Check if weight_sum is smaller than the quota, otherwise throw error
	if (this->weightSum < this->quota) {
		throw std::invalid_argument("The sum of all weights smaller than quota. Please, check your input.");
	}
	// Check if quota is greater or equal than half the weight sum, otherwise present a warning
	if (this->quota < (this->weightSum + 1) / 2) {
		log::out << log::warning << "The quota is less than half of the weight sum." << log::endl;
	}

	// Find out if there are veto players and if there are which
	numberOfVetoPlayers = 0;
	playerIsVetoPlayer.resize(numberOfPlayers);
	for (longUInt i = 0; i < numberOfPlayers; ++i) {
		if (weights[i] >= quota) {
			playerIsVetoPlayer[i] = true;
			++numberOfVetoPlayers;
		} else {
			playerIsVetoPlayer[i] = false;
		}
	}
}

const epic::Permutation& epic::Game::getPermutation() const {
	return mPermutation;
}

const std::vector<epic::longUInt>& epic::Game::getWeights() const {
	return weights;
}

epic::longUInt epic::Game::getQuota() const {
	return quota;
}

bool epic::Game::getFlagNullPlayerHandling() const {
	return flag_null_player_handling;
}

epic::longUInt epic::Game::getWeightSum() const {
	return weightSum;
}

epic::longUInt epic::Game::getNumberOfPlayers() const {
	return numberOfPlayers;
}

const std::vector<bool>& epic::Game::getVetoPlayerVector() const {
	return playerIsVetoPlayer;
}

epic::longUInt epic::Game::getNumberOfVetoPlayers() const {
	return numberOfVetoPlayers;
}

std::vector<epic::bigFloat> epic::Game::getSolution() const {
	return solution;
}

void epic::Game::setSolution(const std::vector<bigFloat>& pre_solution) {
	solution.resize(weights.size() + excludedNullPlayer.size());
	mPermutation.reverse(pre_solution, solution, bigFloat(0));
}

void epic::Game::setSingleValueSolution(const bigFloat& value) {
	solution.resize(1);
	solution[0] = value;
}

epic::longUInt epic::Game::getNumberOfNullPlayers() const {
	return numberOfNullPlayers;
}

epic::longUInt epic::Game::getNumberOfNonZeroPlayers() const {
	return numberOfNonZeroPlayers;
}

epic::longUInt epic::Game::getNumberOfPlayersWithWeight0() const {
	return numberOfPlayersOfWeight0;
}

epic::longUInt epic::Game::playerIndexToNumber(longUInt index) const {
	return mPermutation.inverseIndex(index) + 1;
}

//---------- private methods -------------

std::vector<epic::longUInt> epic::Game::sortWeights() {
	std::vector<std::pair<longUInt, longUInt>> weight_index_pair(weights.size());
	for (longUInt i = 0; i < weights.size(); ++i) {
		weight_index_pair[i] = std::make_pair(weights[i], i);
	}
	std::sort(weight_index_pair.begin(), weight_index_pair.end(), std::greater<>());

	std::vector<longUInt> p(weights.size());
	for (longUInt i = 0; i < weights.size(); ++i) {
		weights[i] = weight_index_pair[i].first;
		p[i] = weight_index_pair[i].second;
	}

	return p;
}

epic::longUInt epic::Game::findNullPlayersFromBelow(bool flag_withoutNullPlayers) {
	int n = weights.size();

	// Helper variable for adding up the weights
	longUInt cs_tmp = 0;
	// Cumulative sum variable
	auto cs = new longUInt[n];
	int minsize = 0;
	// Find the minimal winning coalitions of least size
	for (int i = 0; i < n; i++) {
		cs_tmp += weights[i];
		cs[i] = cs_tmp;
	}
	// The first minsize players are definitely not null players.
	for (int i = 0; cs[i] < quota; i++) {
		minsize = i + 1;
	}
	delete[] cs;

	bool nullPlayerIndexFound = false;
	longUInt nullPlayerIndex = weights.size() + 1;
	auto upper = new longUInt[n]();
	longUInt w_sum = 0;

	for (int j = 0; j < n; j++) {
		w_sum += weights[j];
		if (quota < w_sum) {
			upper[j] = quota;
		} else {
			upper[j] = w_sum;
		}
	}

	auto cv = new longUInt[quota]();
	longUInt alpha_iM1 = 0;

	for (int i = 0; i < n && !nullPlayerIndexFound; i++) {
		for (longUInt x = upper[i] + 1; x > weights[i]; x--) {
			if (weights[i] != 0) {
				if (x - 1 - weights[i] == 0) {
					cv[x - 2] += 1;
				} else {
					cv[x - 2] += cv[(x - 2) - weights[i]];
				}
			}
		}
		if (i > minsize) {
			if (alpha_iM1 + w_sum < quota) {
				nullPlayerIndexFound = true;
				nullPlayerIndex = i;
			}
		}

		for (longUInt j = quota - 2; j < quota; --j) {
			if (cv[j] > 0) {
				alpha_iM1 = j + 1;
				break;
			}
		}
		w_sum -= weights[i];
	}

	delete[] upper;
	delete[] cv;

	if (!nullPlayerIndexFound) {
		return 0;
	} else {
		longUInt n_null_players = weights.size() - nullPlayerIndex;

		if (flag_withoutNullPlayers) {
			excludedNullPlayer.reserve(n_null_players);
			while (nullPlayerIndex < weights.size()) {
				longUInt weight = weights[nullPlayerIndex];
				weights.erase(weights.begin() + nullPlayerIndex);
				excludedNullPlayer.push_back(weight);
			}
		}

		log::out << log::info << n_null_players << " null player(s) found!" << log::endl;

		return n_null_players;
	}
}






/*
 * PrecoalitionGame
 */

epic::PrecoalitionGame::PrecoalitionGame(longUInt quota, std::vector<longUInt>& untreated_weights, bool flag_withoutNullPlayers, std::vector<std::vector<int>>& precoalitions) : Game(quota, untreated_weights, flag_withoutNullPlayers) {
	mPrecoalitions = precoalitions;

	sortPrecoalitions();

	mNumberOfPrecoalitions = precoalitions.size();
	precoalitionWeights.resize(mNumberOfPrecoalitions);
	mMaxPrecoalitionSize = 0;
	for (longUInt i = 0; i < mNumberOfPrecoalitions; ++i) {
		longUInt precSize = precoalitions[i].size();
		precoalitionWeights[i] = 0;

		for (longUInt p = 0; p < precSize; ++p) {
			precoalitionWeights[i] += weights[mPrecoalitions[i][p]];
		}

		if (precSize > mMaxPrecoalitionSize) {
			mMaxPrecoalitionSize = precSize;
		}
	}
}

void epic::PrecoalitionGame::sortPrecoalitions() {
	for (auto& prec: mPrecoalitions) {
		for (auto& it: prec) {
			it = mPermutation.applyIndex(it);
		}
	}
}

std::vector<std::vector<int>> epic::PrecoalitionGame::getPrecoalitions() const {
	return mPrecoalitions;
}

std::vector<epic::longUInt> epic::PrecoalitionGame::getPrecoalitionWeights() const {
	return precoalitionWeights;
}

epic::longUInt epic::PrecoalitionGame::getNumberOfPrecoalitions() const {
	return mNumberOfPrecoalitions;
}

epic::longUInt epic::PrecoalitionGame::getMaxPrecoalitionSize() const {
	return mMaxPrecoalitionSize;
}
