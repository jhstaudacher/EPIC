#include "index/ColemanPreventive.h"

#include "Logging.h"

epic::index::ColemanPreventive::ColemanPreventive(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: RawBanzhaf(g, approx, int_representation) {
}

std::vector<epic::bigFloat> epic::index::ColemanPreventive::calculate() {
	// n_wc[x]: number of winning coalitions of weight x.
	ArrayOffset<lint::LargeNumber> n_wc(mGame.getWeightSum() + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(n_wc.getArrayPointer(), n_wc.getNumberOfElements());
	numberOfWinningCoalitionsPerWeight(n_wc);

	lint::LargeNumber total_wc;
	mCalculator->allocInit_largeNumber(total_wc);
	numberOfWinningCoalitions(n_wc, total_wc);

	// n_sp[x]: number of times player x is a swing player
	auto n_sp = new lint::LargeNumber[mNonZeroPlayerCount];
	mCalculator->allocInit_largeNumberArray(n_sp, mNonZeroPlayerCount);
	numberOfTimesPlayerIsSwingPlayer(n_wc, n_sp);

	mCalculator->free_largeNumberArray(n_wc.getArrayPointer());

	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	{
		bigInt big_total_wc;
		mCalculator->to_bigInt(&big_total_wc, total_wc);
		bigInt wc;
		bigInt sp;

		if (log::out.getLogLevel() <= log::info) {
			bigInt factor = bigInt(1) << mGame.getNumberOfPlayersWithWeight0();

			log::out << log::info << "Total number of winning coalitions: " << big_total_wc * factor << log::endl;
			log::out << log::info << "Number of times each player is a swing player:" << log::endl;

			for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
				mCalculator->to_bigInt(&sp, n_sp[i]);
				solution[i] = sp;
				solution[i] /= big_total_wc;
				log::out << "Player " << mGame.playerIndexToNumber(i) << ": " << sp * factor << log::endl;
			}
			for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) {
				log::out << "Player " << mGame.playerIndexToNumber(i) << ": 0" << log::endl;
				// solution[i] = 0; (already zero-initialized)
			}
		} else {
			for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
				mCalculator->to_bigInt(&sp, n_sp[i]);
				solution[i] = sp;
				solution[i] /= big_total_wc;
			}
			// for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) solution[i] = 0; (already zero-initialized)
		}
	}

	mCalculator->free_largeNumberArray(n_sp);
	delete[] n_sp;
	mCalculator->free_largeNumber(total_wc);

	return solution;
}

std::string epic::index::ColemanPreventive::getFullName() {
	return "ColemanPreventive";
}
