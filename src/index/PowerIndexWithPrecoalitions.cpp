#include "index/PowerIndexWithPrecoalitions.h"

epic::index::PowerIndexWithPrecoalitions::PowerIndexWithPrecoalitions(Game& g)
	: ItfPowerIndex(g) {
	mNbPart = mGame.getPrecoalitions().size();
	mPartW = new longUInt[mNbPart]();
	mMaxPartSize = 0;
	for (longUInt i = 0; i < mNbPart; ++i) {
		longUInt partSize = mGame.getPrecoalitions()[i].size();
		for (longUInt p = 0; p < partSize; ++p) {
			mPartW[i] += mGame.getWeights()[mGame.getPrecoalitions()[i][p]];
		}

		if (partSize > mMaxPartSize) {
			mMaxPartSize = partSize;
		}
	}
}

epic::index::PowerIndexWithPrecoalitions::~PowerIndexWithPrecoalitions() {
	delete[] mPartW;
}

void epic::index::PowerIndexWithPrecoalitions::coalitionsContainingPlayerFromAbove(ArrayOffset<lint::LargeNumber>& cw, ArrayOffset<lint::LargeNumber>& cc, longUInt wi) {
	for (longUInt i = mGame.getQuota(); i <= mGame.getWeightSum(); ++i) {
		mCalculator->assign(cw[i], cc[i]);
	}

	for (longUInt i = mGame.getWeightSum() - wi; i >= mGame.getQuota(); --i) {
		mCalculator->minus(cw[i], cc[i], cw[i + wi]);
	}
}

void epic::index::PowerIndexWithPrecoalitions::coalitionsWithoutPlayerFromBelow(lint::LargeNumber cw[], lint::LargeNumber cc[], longUInt wi) {
	//longUInt min = std::min(quota + winternal[ii] - 1, totalWeight);
	//std::min(mGame.getQuota(), wi)
	for (longUInt i = 0; i < mGame.getQuota()-1; ++i) {
		mCalculator->assign(cw[i], cc[i]);
		//log::out << "i: " << i << log::endl;
		//log::out << "rep i: " << i << log::endl;
	}

	if (wi <= (mGame.getQuota())){
		for (longUInt x = wi; x < (mGame.getQuota()); ++x){
			if((x-wi) <=0 ){
				lint::LargeNumber tmp;
				mCalculator->assign_one(tmp);
				mCalculator->minus(tmp, cc[x-1], tmp); //(cw[x]-1)
				//log::out << "x-1: " << x-1 << log::endl;
				mCalculator->assign(cw[x-1], tmp);
				mCalculator->free_largeNumber(tmp);
			}
			else{
				mCalculator->minus(cw[x-1], cc[x-1], cw[x-wi-1]);
				//log::out << "x-1: " << x-1 << log::endl;
				//log::out << "x-wi-1: " << x-wi-1 << log::endl;
			}
		}		
	}
}

void epic::index::PowerIndexWithPrecoalitions::generalizedBackwardCountingPerWeight(ArrayOffset<lint::LargeNumber>& c, longUInt* weights, longUInt n) {
	for (longUInt i = 0; i < n; ++i) {
		for (longUInt x = mGame.getQuota() + weights[i]; x <= mGame.getWeightSum(); ++x) {
			mCalculator->plusEqual(c[x - weights[i]], c[x]);
		}
	}
}

void epic::index::PowerIndexWithPrecoalitions::generalizedForwardCountingPerWeight(lint::LargeNumber c[], longUInt* weights, longUInt n, longUInt c0) {
	for (longUInt i = 0; i < n; ++i) {
		longUInt x = mGame.getQuota();
		while (x > weights[i]){
			x = x - 1;
			if ((x-weights[i])==0){
				mCalculator->plusEqual(c[x-1], c0);
			}
			else {
				mCalculator->plusEqual(c[x-1], c[x-weights[i]-1]);
			}
		}
	}
}

void epic::index::PowerIndexWithPrecoalitions::coalitionsCardinalityContainingPlayerFromAbove(Array2dOffset<lint::LargeNumber>& cw, Array2dOffset<lint::LargeNumber>& cc, longUInt n, longUInt p, longUInt* weights) {
	for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); ++x) {
		for (longUInt y = 0; y < n; ++y) {
			mCalculator->assign(cw(x, y), cc(x, y));
		}
	}

	for (longUInt x = mGame.getWeightSum() - weights[p]; x >= mGame.getQuota(); --x) {
		for (longUInt s = 1; s < n; ++s) {
			mCalculator->minus(cw(x, n - s - 1), cc(x, n - s - 1), cw(x + weights[p], n - s));
		}
	}
}

void epic::index::PowerIndexWithPrecoalitions::coalitionsCardinalityWithoutPlayerFromBelow(Array2d<lint::LargeNumber>& cwo2, Array2d<lint::LargeNumber>& cwoi, longUInt n, longUInt p, longUInt* weights, longUInt offset, int s) {
	//replicate cwo2 onto cwoi
	for (longUInt i = 0; i < mGame.getQuota(); ++i){
		for (longUInt ii = 0; ii < n; ++ii){
			mCalculator->assign(cwoi(i, ii), cwo2(i, ii));
		}
	}
	
	for (longUInt x = weights[p]; x < mGame.getQuota(); ++x) {
		if(x-weights[p] <= 0){
			if(s == -1){
				lint::LargeNumber tmp;
				mCalculator->assign_one(tmp);
				mCalculator->minus(tmp, cwo2(x-1,offset), tmp);
				mCalculator->assign(cwoi(x-1,offset), tmp);
				mCalculator->free_largeNumber(tmp);
			}
		}
		else{
			for (longUInt s = 2; s <= (n+1); ++s){
				mCalculator->minus(cwoi(x-1,s-1), cwo2(x-1, s-1), cwoi(x-weights[p]-1,s-2));
			}
		}		
	}	
}

void epic::index::PowerIndexWithPrecoalitions::generalizedBackwardCountingPerWeightCardinality(Array2dOffset<lint::LargeNumber>& cc, longUInt* weights, longUInt n) {
	for (longUInt i = 0; i < n; ++i) {
		for (longUInt x = mGame.getQuota() + weights[i]; x <= mGame.getWeightSum(); ++x) {
			for (longUInt m = 1; m < n; ++m) {
				mCalculator->plusEqual(cc(x - weights[i], m - 1), cc(x, m));
			}
		}
	}
}

void epic::index::PowerIndexWithPrecoalitions::generalizedForwardCountingPerWeightCardinality(Array2d<lint::LargeNumber>& cc, longUInt* weights, longUInt n, longUInt c0, longUInt offset, bool flag) {
	for (longUInt i = 0; i < n; ++i) {
		for (longUInt x = mGame.getQuota()-1; x >= weights[i]; x=x-1) {
			if(x-weights[i] == 0){
				if (offset != (longUInt)1){
					mCalculator->plusEqual(cc(x-1,0+offset), c0);
				}
				if (flag){
					mCalculator->plusEqual(cc(x-1,0+offset), c0);
				}
			}
			else{
				for (longUInt s = 2; s <= (n+offset); ++s){
					mCalculator->plusEqual(cc(x-1,s-1), cc(x-weights[i]-1, s-2));
				}
			}
		}
	}
}
