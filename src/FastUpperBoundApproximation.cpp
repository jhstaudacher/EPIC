#include "FastUpperBoundApproximation.h"


epic::FastUpperBoundApproximation::FastUpperBoundApproximation(Game& g)
	: mGame(g) {}

epic::bigInt epic::FastUpperBoundApproximation::upperBound_numberOfWinningCoalitionsPerWeight() {
	/*
	 * the maximal number of winning coalitions of a certain weight must be smaller than the total number of possible coalitions (2^n).
	 */
	bigInt ret = 2;
	mpz_pow_ui(ret.get_mpz_t(), ret.get_mpz_t(), mGame.getNumberOfPlayers());
	return ret;
}

epic::bigInt epic::FastUpperBoundApproximation::upperBound_totalNumberOfSwingPlayer() {
	/*
	 * If a player is a veto player he is a swing player in each winning coalition. If all players could be veto players, the total number would be number_of_winning_coalitions * number_of_players.
	 * Since it is not possible that every player is a veto player, the real number must be smaller than that.
	 */
	return upperBound_numberOfWinningCoalitionsPerWeight() * mGame.getNumberOfPlayers();
}

epic::bigInt epic::FastUpperBoundApproximation::upperBound_swingPlayerPerCardinality() {
	/*
	 * The coalition cardinality with the most possible coalitions is: cardinality = 0.5 * numberOfPlayers.
	 * A player could at most be a swing player in each of this coalitions.
	 *
	 * The number of this coalitions can be calculated using the binomial coefficient n over k; with n: numberOfPlayers, k: n/2
	 * n over k = n!/(k! (n - k)!) = n! / ((n/2)! (n - n/2)!) = n!/((n/2)!^2)
	 *
	 * This value can get approximated using n!/((n/2)!^2) < 2^n
	 */
	bigInt ret = 2;
	mpz_pow_ui(ret.get_mpz_t(), ret.get_mpz_t(), mGame.getNumberOfPlayers());
	return ret;
}
