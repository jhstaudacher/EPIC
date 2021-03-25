#include "index/Banzhaf.h"

#include "Array.h"
#include "Logging.h"

epic::index::Banzhaf::Banzhaf(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawBanzhaf(g, approx, int_representation) {
}

std::vector<epic::bigFloat> epic::index::Banzhaf::calculate() {
	// n_sp[x]: number of times player x is a swing player
	auto n_sp = new lint::LargeNumber[mNonZeroPlayerCount];
	mCalculator->allocInit_largeNumberArray(n_sp, mNonZeroPlayerCount);
	numberOfTimesPlayerIsSwingPlayer(n_sp);

	// total_n_sp: The sum of all n_sp's
	lint::LargeNumber total_n_sp;
	mCalculator->allocInit_largeNumber(total_n_sp);
	numberOfSwingPlayer(n_sp, total_n_sp);

	// solution[x]: the relative banzhaf index for player x
	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	{
		bigInt tmp;

		mCalculator->to_bigInt(&tmp, total_n_sp);
		bigFloat float_total_n_sp = tmp;

		if (log::out.getLogLevel() <= log::info) {
			bigInt factor = bigInt(1) << mGame.getNumberOfPlayersWithWeight0(); // additional winning coalitions due to players of weight 0

			log::out << log::info << "Total number of swings: " << float_total_n_sp * factor << log::endl;
			log::out << log::info << "Raw Banzhaf:" << log::endl;

			for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
				mCalculator->to_bigInt(&tmp, n_sp[i]);
				log::out << "Player " << mGame.playerIndexToNumber(i) << ": " << tmp * factor << log::endl;
				solution[i] = tmp / float_total_n_sp;
			}
			for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) {
				log::out << "Player " << mGame.playerIndexToNumber(i) << ": 0" << log::endl;
				// solution[i] = 0; (already zero-initialized)
			}
		} else {
			for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
				mCalculator->to_bigInt(&tmp, n_sp[i]);
				solution[i] = tmp / float_total_n_sp;
			}
			// for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) solution[i] = 0; (already zero-initialized)
		}
	}

	/*
	 * DELETE
	 */
	mCalculator->free_largeNumber(total_n_sp);
	mCalculator->free_largeNumberArray(n_sp);
	delete[] n_sp;

	return solution;
}

std::string epic::index::Banzhaf::getFullName() {
	return "Banzhaf";
}
