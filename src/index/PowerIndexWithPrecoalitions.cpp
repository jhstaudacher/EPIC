#include "index/PowerIndexWithPrecoalitions.h"
#include "Logging.h"

#include "lint/GlobalCalculator.h"

epic::index::PowerIndexWithPrecoalitions::PowerIndexWithPrecoalitions()
	: ItfPowerIndex() {}

void epic::index::PowerIndexWithPrecoalitions::coalitionsContainingPlayerFromAbove(PrecoalitionGame* g, ArrayOffset<lint::LargeNumber>& cw, ArrayOffset<lint::LargeNumber>& cc, longUInt wi) {
	for (longUInt i = g->getQuota(); i <= g->getWeightSum(); ++i) {
		gCalculator->assign(cw[i], cc[i]);
	}

	for (longUInt i = g->getWeightSum() - wi; i >= g->getQuota(); --i) {
		gCalculator->minus(cw[i], cc[i], cw[i + wi]);
	}
}

void epic::index::PowerIndexWithPrecoalitions::coalitionsWithoutPlayerFromBelow(PrecoalitionGame* g, lint::LargeNumber cw[], lint::LargeNumber cc[], longUInt wi) {
	//longUInt min = std::min(quota + winternal[ii] - 1, totalWeight);
	//std::min(mGame.getQuota(), wi)
	for (longUInt i = 0; i < g->getQuota()-1; ++i) {
		gCalculator->assign(cw[i], cc[i]);
		//log::out << "i: " << i << log::endl;
		//log::out << "rep i: " << i << log::endl;
	}

	if (wi <= (g->getQuota())){
		for (longUInt x = wi; x < (g->getQuota()); ++x){
			if((x-wi) <=0 ){
				lint::LargeNumber tmp;
				gCalculator->assign_one(tmp);
				gCalculator->minus(tmp, cc[x-1], tmp); //(cw[x]-1)
				//log::out << "x-1: " << x-1 << log::endl;
				gCalculator->assign(cw[x-1], tmp);
				gCalculator->free_largeNumber(tmp);
			}
			else{
				gCalculator->minus(cw[x-1], cc[x-1], cw[x-wi-1]);
				//log::out << "x-1: " << x-1 << log::endl;
				//log::out << "x-wi-1: " << x-wi-1 << log::endl;
			}
		}		
	}
}

void epic::index::PowerIndexWithPrecoalitions::generalizedBackwardCountingPerWeight(PrecoalitionGame* g, ArrayOffset<lint::LargeNumber>& c, const std::vector<longUInt>& weights, longUInt n) {
	for (longUInt i = 0; i < n; ++i) {
		for (longUInt x = g->getQuota() + weights[i]; x <= g->getWeightSum(); ++x) {
			gCalculator->plusEqual(c[x - weights[i]], c[x]);
		}
	}
}


void epic::index::PowerIndexWithPrecoalitions::generalizedForwardCountingPerWeight(PrecoalitionGame* g, lint::LargeNumber c[], const std::vector<longUInt>& weights, longUInt n, longUInt c0) {
	for (longUInt i = 0; i < n; ++i) {
		longUInt x = g->getQuota();
		while (x > weights[i]){
			x = x - 1;
			if ((x-weights[i])==0){
				gCalculator->plusEqual(c[x-1], c0);
			}
			else {
				gCalculator->plusEqual(c[x-1], c[x-weights[i]-1]);
			}
		}
	}
}

void epic::index::PowerIndexWithPrecoalitions::coalitionsCardinalityContainingPlayerFromAbove(PrecoalitionGame* g, Array2dOffset<lint::LargeNumber>& cw, Array2dOffset<lint::LargeNumber>& cc, longUInt n, longUInt p, const std::vector<longUInt>& weights) {
	for (longUInt x = g->getQuota(); x <= g->getWeightSum(); ++x) {
		for (longUInt y = 0; y < n; ++y) {
			gCalculator->assign(cw(x, y), cc(x, y));
		}
	}

	for (longUInt x = g->getWeightSum() - weights[p]; x >= g->getQuota(); --x) {
		for (longUInt s = 1; s < n; ++s) {
			gCalculator->minus(cw(x, n - s - 1), cc(x, n - s - 1), cw(x + weights[p], n - s));
		}
	}
}

void epic::index::PowerIndexWithPrecoalitions::coalitionsCardinalityWithoutPlayerFromBelow(PrecoalitionGame* g, Array2d<lint::LargeNumber>& cwo2, Array2d<lint::LargeNumber>& cwoi, longUInt n, longUInt p, const std::vector<longUInt>& weights, longUInt offset, int s) {
	//replicate cwo2 onto cwoi
	for (longUInt i = 0; i < g->getQuota(); ++i){
		for (longUInt ii = 0; ii < n; ++ii){
			gCalculator->assign(cwoi(i, ii), cwo2(i, ii));
		}
	}
	
	for (longUInt x = weights[p]; x < g->getQuota(); ++x) {
		if(x-weights[p] <= 0){
			if(s == -1){
				lint::LargeNumber tmp;
				gCalculator->assign_one(tmp);
				gCalculator->minus(tmp, cwo2(x-1,offset), tmp);
				gCalculator->assign(cwoi(x-1,offset), tmp);
				gCalculator->free_largeNumber(tmp);
			}
		}
		else{
			for (longUInt s = 2; s <= (n+1); ++s){
				gCalculator->minus(cwoi(x-1,s-1), cwo2(x-1, s-1), cwoi(x-weights[p]-1,s-2));
			}
		}		
	}	
}

void epic::index::PowerIndexWithPrecoalitions::generalizedBackwardCountingPerWeightCardinality(PrecoalitionGame* g, Array2dOffset<lint::LargeNumber>& cc, const std::vector<longUInt>& weights, longUInt n) {
	for (longUInt i = 0; i < n; ++i) {
		for (longUInt x = g->getQuota() + weights[i]; x <= g->getWeightSum(); ++x) {
			for (longUInt m = 1; m < n; ++m) {
				gCalculator->plusEqual(cc(x - weights[i], m - 1), cc(x, m));
			}
		}
	}
}

void epic::index::PowerIndexWithPrecoalitions::generalizedForwardCountingPerWeightCardinality(PrecoalitionGame* g, Array2d<lint::LargeNumber>& cc, const std::vector<longUInt>& weights, longUInt n, longUInt c0, longUInt offset, bool flag) {
	for (longUInt i = 0; i < n; ++i) {
		for (longUInt x = g->getQuota()-1; x >= weights[i]; x=x-1) {
			if(x-weights[i] == 0){
				if (offset != (longUInt)1){
					gCalculator->plusEqual(cc(x-1,0+offset), c0);
				}
				if (flag){
					gCalculator->plusEqual(cc(x-1,0+offset), c0);
				}
			}
			else{
				for (longUInt s = 2; s <= (n+offset); ++s){
					gCalculator->plusEqual(cc(x-1,s-1), cc(x-weights[i]-1, s-2));
				}
			}
		}
	}
}
