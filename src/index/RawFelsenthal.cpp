#include "index/RawFelsenthal.h"

#include "lint/GlobalCalculator.h"

epic::index::RawFelsenthal::RawFelsenthal(Game* g) : ItfPowerIndex() {
	// total_csum: cumulative sum of weights
	bigInt total_csum;

	// determine number for minsize (minsize = (number of players subsumed since >= quota))
	for (longUInt i = 0; i < g->getNumberOfPlayers(); i++) {
		total_csum += g->getWeights()[i];
		if (total_csum >= g->getQuota()) {
			mMinsize = i + 1;
			break;
		}
	}
}

void epic::index::RawFelsenthal::minimal_winning_coalitions_of_least_size(Game* g, lint::LargeNumber mwcs[]) {
	Array2d<lint::LargeNumber> f;
	f.alloc(mMinsize + 1, g->getQuota() + 1);
	gCalculator->allocInit_largeNumberArray(f.getArrayPointer(), f.getNumberOfElements());

	// initialise the empty coalition with 1, since we know, that the empty coalition is always losing
	gCalculator->assign_one(f(0, 0));

	// compute formula f for player n-2 and save that in f-(2D)-array
	for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers() - 1; ++i) {
		longInt wi = g->getWeights()[i];

		for (longUInt k = (i < mMinsize) ? i + 1 : mMinsize; k > 0; --k) {
			for (longInt x = g->getQuota() + 1; x > wi; --x) {
				gCalculator->plusEqual(f(k, x - 1), f(k - 1, x - wi - 1));
			}
		}
	}

	// compute unscaled Felsenthal (<=> number of minimal winning coalitions) for each player!
	// note: very similar to deegan packel algorithm
	{
		Array2d<lint::LargeNumber> b;
		b.alloc(mMinsize + 1, g->getQuota() + 1);
		gCalculator->allocInit_largeNumberArray(b.getArrayPointer(), b.getNumberOfElements());

		//tmp_mwcs: temporary largeNumber for interim results
		lint::LargeNumber tmp_mwcs;
		gCalculator->allocInit_largeNumber(tmp_mwcs);

		// wi: weight of player i
		longUInt wi = 0;

		// wi: weight of player i + 1
		longUInt wi_plus = 0;

		// last player
		{
			longUInt last_player = g->getNumberOfNonZeroPlayers() - 1;
			wi = g->getWeights()[last_player];

			// b is already zero initialized ==> only initialize the 1-entries
			for (longUInt x = 1; x <= wi; ++x) {
				gCalculator->assign_one(b(1, x));
				gCalculator->mul(tmp_mwcs, f(mMinsize - 1, g->getQuota() - x), b(1, x));
				gCalculator->plusEqual(mwcs[last_player], tmp_mwcs);
			}
		}

		// all other players (except the last one)
		for (longUInt i = g->getNumberOfNonZeroPlayers() - 2; i < g->getNumberOfNonZeroPlayers(); --i) {
			wi = g->getWeights()[i];
			wi_plus = g->getWeights()[i + 1];

			for (longUInt k = mMinsize; k > 0; --k) {
				for (longUInt x = g->getQuota() + 1; x > wi; --x) {
					gCalculator->plus(b(k, x - 1), b(k, x - wi + wi_plus - 1), b(k - 1, x - wi - 1));
				}
			}

			for (longUInt k = 0; k < mMinsize; ++k) {
				for (longUInt x = wi; x < g->getQuota() + 1; ++x) {
					gCalculator->minusEqual(f(k + 1, x), f(k, x - wi));
				}
			}

			longUInt l = (i + 1 < mMinsize) ? i + 2 : mMinsize;
			for (longUInt x = wi_plus + 1; x < wi + 1; ++x) {
				gCalculator->assign_one(b(1, x));
				for (longUInt k = 2; k <= l; k++) {
					gCalculator->assign_zero(b(k, x));
				}
			}

			for (longUInt x = 0; x <= g->getQuota(); ++x) {
				for (longUInt k = 0; k < mMinsize; ++k) {
					gCalculator->mul(tmp_mwcs, f(k, x), b(mMinsize - k, g->getQuota() - x));
					gCalculator->plusEqual(mwcs[i], tmp_mwcs);
				}
			}
		}

		// delete
		gCalculator->free_largeNumber(tmp_mwcs);
		gCalculator->free_largeNumberArray(b.getArrayPointer());
		b.free();
	}

	gCalculator->free_largeNumberArray(f.getArrayPointer());
	f.free();
}

std::vector<epic::bigFloat> epic::index::RawFelsenthal::calculate(Game* g) {
	// if a veto player exists, that player gets assigned 1 while all other players get assigned 0
	// note: the weights have to be sorted decreasingly
	if (g->getWeights()[0] >= g->getQuota()) {
		std::vector<bigFloat> solution(g->getNumberOfPlayers());
		{
			solution[0] = 1.0;

			for (longUInt i = 1; i < g->getNumberOfPlayers(); ++i) {
				solution[i] = 0.0;
			}
		}
		return solution;
	}

	// mwcs: number of minimal winning coalitions for each player i
	auto mwcs = new lint::LargeNumber[g->getNumberOfNonZeroPlayers()];
	gCalculator->allocInit_largeNumberArray(mwcs, g->getNumberOfNonZeroPlayers());
	minimal_winning_coalitions_of_least_size(g, mwcs);

	std::vector<bigFloat> solution(g->getNumberOfPlayers());
	{
		bigInt int_tmp;

		// compute Felsenthal-index: Number of minimal winning coalitions of player i
		for (longUInt i = 0; i < g->getNumberOfNonZeroPlayers(); ++i) {
			gCalculator->to_bigInt(&int_tmp, mwcs[i]);
			solution[i] = static_cast<bigFloat>(int_tmp);
		}

		for (longUInt i = g->getNumberOfNonZeroPlayers(); i < g->getNumberOfPlayers(); ++i) {
			solution[i] = 0;
		}
	}

	/*
	 * DELETE
	 */
	gCalculator->free_largeNumberArray(mwcs);
	delete[] mwcs;

	return solution;
}

std::string epic::index::RawFelsenthal::getFullName() {
	return "RawFelsenthal";
}

epic::longUInt epic::index::RawFelsenthal::getMemoryRequirement(Game* g) {
	bigInt memory = (mMinsize + 1) * (g->getQuota() + 1) * gCalculator->getLargeNumberSize() * 2; // f, b
	memory += g->getNumberOfNonZeroPlayers() * gCalculator->getLargeNumberSize();								 // mwcs
	memory /= cMemUnit_factor;

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}

epic::bigInt epic::index::RawFelsenthal::getMaxValueRequirement(ItfUpperBoundApproximation* approx) {
	return approx->upperBound_totalNumberOfSwingPlayer();
}

epic::lint::Operation epic::index::RawFelsenthal::getOperationRequirement() {
	return lint::Operation::multiplication;
}
