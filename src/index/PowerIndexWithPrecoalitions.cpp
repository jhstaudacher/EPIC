#include "index/PowerIndexWithPrecoalitions.h"

#include "lint/GlobalCalculator.h"

epic::index::PowerIndexWithPrecoalitions::PowerIndexWithPrecoalitions() : ItfPowerIndex() {}

void epic::index::PowerIndexWithPrecoalitions::coalitionsContainingPlayerFromAbove(PrecoalitionGame* g, ArrayOffset<lint::LargeNumber>& cw, ArrayOffset<lint::LargeNumber>& cc, longUInt wi) {
	for (longUInt i = g->getQuota(); i <= g->getWeightSum(); ++i) {
		gCalculator->assign(cw[i], cc[i]);
	}

	for (longUInt i = g->getWeightSum() - wi; i >= g->getQuota(); --i) {
		gCalculator->minus(cw[i], cc[i], cw[i + wi]);
	}
}

void epic::index::PowerIndexWithPrecoalitions::generalizedBackwardCountingPerWeight(PrecoalitionGame* g, ArrayOffset<lint::LargeNumber>& c, const std::vector<longUInt>& weights, longUInt n) {
	for (longUInt i = 0; i < n; ++i) {
		for (longUInt x = g->getQuota() + weights[i]; x <= g->getWeightSum(); ++x) {
			gCalculator->plusEqual(c[x - weights[i]], c[x]);
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

void epic::index::PowerIndexWithPrecoalitions::generalizedBackwardCountingPerWeightCardinality(PrecoalitionGame* g, Array2dOffset<lint::LargeNumber>& cc, const std::vector<longUInt>& weights, longUInt n) {
	for (longUInt i = 0; i < n; ++i) {
		for (longUInt x = g->getQuota() + weights[i]; x <= g->getWeightSum(); ++x) {
			for (longUInt m = 1; m < n; ++m) {
				gCalculator->plusEqual(cc(x - weights[i], m - 1), cc(x, m));
			}
		}
	}
}
