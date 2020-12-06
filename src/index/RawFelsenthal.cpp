#include "index/RawFelsenthal.h"

epic::index::RawFelsenthal::RawFelsenthal(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation)
	: ItfPowerIndex(g) {
	bigInt max_value = approx->upperBound_totalNumberOfSwingPlayer();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::multiplication, int_representation);

	// total_csum: cumulative sum of weights
	bigInt total_csum;

	// determine number for minsize (minsize = (number of players subsumed since >= quota))
	for (longUInt i = 0; i < mGame.getNumberOfPlayers(); i++) {
		total_csum += mGame.getWeights()[i];
		if (total_csum >= mGame.getQuota()) {
			mMinsize = i + 1;
			break;
		}
	}
}

epic::index::RawFelsenthal::~RawFelsenthal() {
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);
}

void epic::index::RawFelsenthal::minimal_winning_coalitions_of_least_size(lint::LargeNumber mwcs[]) {
	Array2d<lint::LargeNumber> f;
	f.alloc(mMinsize + 1, mGame.getQuota() + 1);
	mCalculator->allocInit_largeNumberArray(f.getArrayPointer(), f.getNumberOfElements());

	// initialise the empty coalition with 1, since we know, that the empty coalition is always losing
	mCalculator->assign_one(f(0, 0));

	// compute formula f for player n-2 and save that in f-(2D)-array
	for (longUInt i = 0; i < mNonZeroPlayerCount - 1; ++i) {
		longInt wi = mGame.getWeights()[i];

		for (longUInt k = (i < mMinsize) ? i + 1 : mMinsize; k > 0; --k) {
			for (longInt x = mGame.getQuota() + 1; x > wi; --x) {
				mCalculator->plusEqual(f(k, x - 1), f(k - 1, x - wi - 1));
			}
		}
	}

	// compute unscaled Felsenthal (<=> number of minimal winning coalitions) for each player!
	// note: very similar to deegan packel algorithm
	{
		Array2d<lint::LargeNumber> b;
		b.alloc(mMinsize + 1, mGame.getQuota() + 1);
		mCalculator->allocInit_largeNumberArray(b.getArrayPointer(), b.getNumberOfElements());

		//tmp_mwcs: temporary largeNumber for interim results
		lint::LargeNumber tmp_mwcs;
		mCalculator->allocInit_largeNumber(tmp_mwcs);

		// wi: weight of player i
		longUInt wi = 0;

		// wi: weight of player i + 1
		longUInt wi_plus = 0;

		// last player
		{
			longUInt last_player = mNonZeroPlayerCount - 1;
			wi = mGame.getWeights()[last_player];

			// b is already zero initialized ==> only initialize the 1-entries
			for (longUInt x = 1; x <= wi; ++x) {
				mCalculator->assign_one(b(1, x));
				mCalculator->mul(tmp_mwcs, f(mMinsize - 1, mGame.getQuota() - x), b(1, x));
				mCalculator->plusEqual(mwcs[last_player], tmp_mwcs);
			}
		}

		// all other players (except the last one)
		for (longUInt i = mNonZeroPlayerCount - 2; i < mNonZeroPlayerCount; --i) {
			wi = mGame.getWeights()[i];
			wi_plus = mGame.getWeights()[i + 1];

			for (longUInt k = mMinsize; k > 0; --k) {
				for (longUInt x = mGame.getQuota() + 1; x > wi; --x) {
					mCalculator->plus(b(k, x - 1), b(k, x - wi + wi_plus - 1), b(k - 1, x - wi - 1));
				}
			}

			for (longUInt k = 0; k < mMinsize; ++k) {
				for (longUInt x = wi; x < mGame.getQuota() + 1; ++x) {
					mCalculator->minusEqual(f(k + 1, x), f(k, x - wi));
				}
			}

			longUInt l = (i + 1 < mMinsize) ? i + 2 : mMinsize;
			for (longUInt x = wi_plus + 1; x < wi + 1; ++x) {
				mCalculator->assign_one(b(1, x));
				for (longUInt k = 2; k <= l; k++) {
					mCalculator->assign_zero(b(k, x));
				}
			}

			for (longUInt x = 0; x <= mGame.getQuota(); ++x) {
				for (longUInt k = 0; k < mMinsize; ++k) {
					mCalculator->mul(tmp_mwcs, f(k, x), b(mMinsize - k, mGame.getQuota() - x));
					mCalculator->plusEqual(mwcs[i], tmp_mwcs);
				}
			}
		}

		// delete
		mCalculator->free_largeNumber(tmp_mwcs);
		mCalculator->free_largeNumberArray(b.getArrayPointer());
		b.free();
	}

	mCalculator->free_largeNumberArray(f.getArrayPointer());
	f.free();
}

std::vector<epic::bigFloat> epic::index::RawFelsenthal::calculate() {
	// if a veto player exists, that player gets assigned 1 while all other players get assigned 0
	// note: the weights have to be sorted decreasingly
	if (mGame.getWeights()[0] >= mGame.getQuota()) {
		std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
		{
			solution[0] = 1.0;

			for (longUInt i = 1; i < mGame.getNumberOfPlayers(); ++i) {
				solution[i] = 0.0;
			}
		}
		return solution;
	}

	// mwcs: number of minimal winning coalitions for each player i
	auto mwcs = new lint::LargeNumber[mNonZeroPlayerCount];
	mCalculator->allocInit_largeNumberArray(mwcs, mNonZeroPlayerCount);
	minimal_winning_coalitions_of_least_size(mwcs);

	std::vector<bigFloat> solution(mGame.getNumberOfPlayers());
	{
		bigInt int_tmp;

		// compute Felsenthal-index: Number of minimal winning coalitions of player i
		for (longUInt i = 0; i < mNonZeroPlayerCount; ++i) {
			mCalculator->to_bigInt(&int_tmp, mwcs[i]);
			solution[i] = static_cast<bigFloat>(int_tmp);
		}

		for (longUInt i = mNonZeroPlayerCount; i < mGame.getNumberOfPlayers(); ++i) {
			solution[i] = 0;
		}
	}

	/*
	 * DELETE
	 */
	mCalculator->free_largeNumberArray(mwcs);
	delete[] mwcs;

	return solution;
}

std::string epic::index::RawFelsenthal::getFullName() {
	return "RawFelsenthal";
}

epic::longUInt epic::index::RawFelsenthal::getMemoryRequirement() {
	bigInt memory = (mMinsize + 1) * (mGame.getQuota() + 1) * mCalculator->getLargeNumberSize() * 2; // f, b
	memory += mNonZeroPlayerCount * mCalculator->getLargeNumberSize();								 // mwcs

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}

	return ret;
}
