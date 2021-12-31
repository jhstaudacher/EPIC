#include "Game.h"

#include "Logging.h"

#include <algorithm>
#include <stdexcept>

epic::Game::Game(longUInt quota, const std::vector<longUInt>& weights, bool flag_withoutNullPlayers) {
	mQuota = quota;
	mSolution = {};
	mFlagNullPlayerHandling = flag_withoutNullPlayers;

	//sort players by weight
	mWeights = weights;
	mPermutation = Permutation(sortWeights());

	mNumberOfNullPlayers = findNullPlayersFromBelow(flag_withoutNullPlayers);

	mWeightSum = 0;
	for (auto it : mWeights) {
		mWeightSum += it;
	}

	longUInt nWeightZero = mWeights.end() - std::find(mWeights.begin(), mWeights.end(), 0);
	mNumberOfNonZeroPlayers = mWeights.size() - nWeightZero;

	// Check if weight_sum is smaller than the quota, otherwise throw error
	if (mWeightSum < mQuota) {
		throw std::invalid_argument("The sum of all weights smaller than quota. Please, check your input.");
	}
	// Check if quota is greater or equal than half the weight sum, otherwise present a warning
	if (mQuota < (mWeightSum + 1) / 2) {
		log::out << log::warning << "The quota is less than half of the weight sum." << log::endl;
	}

	// Find out if there are veto players and if there are which
	mNumberOfVetoPlayers = 0;
	mPlayerIsVetoPlayer.resize(mWeights.size());
	for (longUInt i = 0; i < mWeights.size(); ++i) {
		if (mWeights[i] >= quota) {
			mPlayerIsVetoPlayer[i] = true;
			++mNumberOfVetoPlayers;
		} else {
			mPlayerIsVetoPlayer[i] = false;
		}
	}
}

const epic::Permutation& epic::Game::getPermutation() const {
	return mPermutation;
}

const std::vector<epic::longUInt>& epic::Game::getWeights() const {
	return mWeights;
}

epic::longUInt epic::Game::getQuota() const {
	return mQuota;
}

bool epic::Game::getFlagNullPlayerHandling() const {
	return mFlagNullPlayerHandling;
}

epic::longUInt epic::Game::getWeightSum() const {
	return mWeightSum;
}

epic::longUInt epic::Game::getNumberOfPlayers() const {
	return mWeights.size();
}

const std::vector<bool>& epic::Game::getVetoPlayerVector() const {
	return mPlayerIsVetoPlayer;
}

epic::longUInt epic::Game::getNumberOfVetoPlayers() const {
	return mNumberOfVetoPlayers;
}

std::vector<epic::bigFloat> epic::Game::getSolution() const {
	return mSolution;
}

void epic::Game::setSolution(const std::vector<bigFloat>& pre_solution) {
	mSolution.resize(mWeights.size() + mExcludedNullPlayer.size(), bigFloat(0));
	mPermutation.reverse(pre_solution, mSolution);
}

void epic::Game::setSingleValueSolution(const bigFloat& value) {
	mSolution.resize(1);
	mSolution[0] = value;
}

epic::longUInt epic::Game::getNumberOfNullPlayers() const {
	return mNumberOfNullPlayers;
}

epic::longUInt epic::Game::getNumberOfNonZeroPlayers() const {
	return mNumberOfNonZeroPlayers;
}

epic::longUInt epic::Game::getNumberOfPlayersWithWeight0() const {
	return mWeights.size() - mNumberOfNonZeroPlayers;
}

epic::longUInt epic::Game::playerIndexToNumber(longUInt index) const {
	return mPermutation.inverseIndex(index) + 1;
}

std::vector<epic::longUInt> epic::Game::sortWeights() {
	std::vector<std::pair<longUInt, longUInt>> weight_index_pair(mWeights.size());
	for (longUInt i = 0; i < mWeights.size(); ++i) {
		weight_index_pair[i] = std::make_pair(mWeights[i], i);
	}
	std::sort(weight_index_pair.begin(), weight_index_pair.end(), std::greater<>());

	std::vector<longUInt> p(mWeights.size());
	for (longUInt i = 0; i < mWeights.size(); ++i) {
		mWeights[i] = weight_index_pair[i].first;
		p[i] = weight_index_pair[i].second;
	}

	return p;
}

epic::longUInt epic::Game::findNullPlayersFromBelow(bool flag_withoutNullPlayers) {
	int n = mWeights.size();

	// Helper variable for adding up the weights
	longUInt cs_tmp = 0;
	// Cumulative sum variable
	auto cs = new longUInt[n];
	int minsize = 0;
	// Find the minimal winning coalitions of least size
	for (int i = 0; i < n; i++) {
		cs_tmp += mWeights[i];
		cs[i] = cs_tmp;
	}
	// The first minsize players are definitely not null players.
	for (int i = 0; cs[i] < mQuota; i++) {
		minsize = i + 1;
	}
	delete[] cs;

	bool nullPlayerIndexFound = false;
	longUInt nullPlayerIndex = mWeights.size() + 1;
	auto upper = new longUInt[n]();
	longUInt w_sum = 0;

	for (int j = 0; j < n; j++) {
		w_sum += mWeights[j];
		if (mQuota < w_sum) {
			upper[j] = mQuota;
		} else {
			upper[j] = w_sum;
		}
	}

	auto cv = new longUInt[mQuota]();
	longUInt alpha_iM1 = 0;

	for (int i = 0; i < n && !nullPlayerIndexFound; i++) {
		for (longUInt x = upper[i] + 1; x > mWeights[i]; x--) {
			if (mWeights[i] != 0) {
				if (x - 1 - mWeights[i] == 0) {
					cv[x - 2] += 1;
				} else {
					cv[x - 2] += cv[(x - 2) - mWeights[i]];
				}
			}
		}
		if (i > minsize) {
			if (alpha_iM1 + w_sum < mQuota) {
				nullPlayerIndexFound = true;
				nullPlayerIndex = i;
			}
		}

		for (longUInt j = mQuota - 2; j < mQuota; --j) {
			if (cv[j] > 0) {
				alpha_iM1 = j + 1;
				break;
			}
		}
		w_sum -= mWeights[i];
	}

	delete[] upper;
	delete[] cv;

	if (!nullPlayerIndexFound) {
		return 0;
	} else {
		longUInt n_null_players = mWeights.size() - nullPlayerIndex;

		if (flag_withoutNullPlayers) {
			mExcludedNullPlayer.reserve(n_null_players);
			while (nullPlayerIndex < mWeights.size()) {
				longUInt weight = mWeights[nullPlayerIndex];
				mWeights.erase(mWeights.begin() + nullPlayerIndex);
				mExcludedNullPlayer.push_back(weight);
			}
		}

		log::out << log::info << n_null_players << " null player(s) found!" << log::endl;

		return n_null_players;
	}
}

/*
 *
 * ##########################
 * #	PrecoalitionGame	#
 * ##########################
 *
 */

epic::PrecoalitionGame::PrecoalitionGame(longUInt quota, std::vector<longUInt>& untreated_weights, bool flag_withoutNullPlayers, std::vector<std::vector<int>>& precoalitions)
	: Game(quota, untreated_weights, flag_withoutNullPlayers) {
	mPrecoalitions = precoalitions;

	sortPrecoalitions();

	mPrecoalitionWeights.resize(mPrecoalitions.size());
	mMaxPrecoalitionSize = 0;
	for (longUInt i = 0; i < mPrecoalitions.size(); ++i) {
		longUInt precSize = mPrecoalitions[i].size();
		mPrecoalitionWeights[i] = 0;

		for (longUInt p = 0; p < precSize; ++p) {
			mPrecoalitionWeights[i] += mWeights[mPrecoalitions[i][p]];
		}

		if (precSize > mMaxPrecoalitionSize) {
			mMaxPrecoalitionSize = precSize;
		}
	}
}

void epic::PrecoalitionGame::sortPrecoalitions() {
	for (auto& prec : mPrecoalitions) {
		for (auto& it : prec) {
			it = mPermutation.applyIndex(it);
		}
	}
}

std::vector<std::vector<int>> epic::PrecoalitionGame::getPrecoalitions() const {
	return mPrecoalitions;
}

std::vector<epic::longUInt> epic::PrecoalitionGame::getPrecoalitionWeights() const {
	return mPrecoalitionWeights;
}

epic::longUInt epic::PrecoalitionGame::getNumberOfPrecoalitions() const {
	return mPrecoalitions.size();
}

epic::longUInt epic::PrecoalitionGame::getMaxPrecoalitionSize() const {
	return mMaxPrecoalitionSize;
}
