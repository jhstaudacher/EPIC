#include "index/SymmetricCoalitionalBanzhaf.h"
#include "Logging.h"
#include <cmath>

epic::index::SymmetricCoalitionalBanzhaf::SymmetricCoalitionalBanzhaf(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation) : ItfPowerIndex(g) {
	/*
	 * Initialize the protected ItfPowerIndex::mCalculator object. This gets used to do all large integer calculations later.
	 * For an approximation of the maximum value needed for the calculations the approx object can get used.
	 */
	bigInt max_value = approx->upperBound_totalNumberOfSwingPlayer();
	mCalculator = lint::ItfLargeNumberCalculator::new_calculator(max_value, lint::Operation::addition, int_representation);

	/*
	 * If needed add additional initializations below
	 */
}

epic::index::SymmetricCoalitionalBanzhaf::~SymmetricCoalitionalBanzhaf() {
	/*
	 * Delete the mCalculator object
	 */
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);

	/**
	 * If needed do additional cleanup below
	 */
}

std::vector<epic::bigFloat> epic::index::SymmetricCoalitionalBanzhaf::calculate() {
	/*
	 * Use mGame and mCalculator to calculate the power index and return the results.
	 */
	int nbPlayers = mGame.getNumberOfPlayers();
	longUInt nbPartitions = mGame.getPrecoalitions().size();
	longUInt totalWeight = mGame.getWeightSum();
	std::vector<bigFloat> solution(nbPlayers);
	std::vector<lint::LargeNumber> banzhafsExternalGame(nbPartitions);
	std::vector<longUInt> partitionWeights;
	longUInt maxMembersInPartition = 1;

	log::out << "weights: " << log::endl;
	for(auto& row:mGame.getPrecoalitions()){
		longUInt weight = 0;
		for(auto& col:row){
			  weight += mGame.getWeights().at(col-1);
			  if (maxMembersInPartition < row.size()){
				  maxMembersInPartition = row.size();
			  }
		}
		partitionWeights.push_back(weight);
		log::out << weight << log::endl;
	}

	ArrayOffset<lint::LargeNumber> cc(totalWeight + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(cc.getArrayPointer(), cc.getNumberOfElements());
	mCalculator->assign_one(cc[totalWeight]);

	for (longUInt i = 0; i < nbPartitions; i++) {
		int wi = partitionWeights[i];
		for (longUInt k = mGame.getQuota() + wi; k <= totalWeight; ++k) {
			mCalculator->plusEqual(cc[k - wi], cc[k]);
		}
	}

	ArrayOffset<lint::LargeNumber> cw(totalWeight + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(cw.getArrayPointer(), cw.getNumberOfElements());
	
	//replicate vector cc onto cw
	for (longUInt i = mGame.getQuota(); i < (totalWeight + 1); i++){
		cw[i] = cc[i];
	}

	for (longUInt i = 0; i < nbPartitions; i++){
		for (longUInt ii = mGame.getQuota(); ii < (totalWeight + 1); ii++){
			cw[ii] = cc[ii];
		}
				
		//if ((length(cc)-w[i]) >= q){
		if ((totalWeight-partitionWeights[i]) >= mGame.getQuota()){
			for(longUInt ii = (totalWeight - partitionWeights[i]); ii >= mGame.getQuota(); ii = ii - 1){
				mCalculator->minus(cw[ii], cc[ii], cw[ii + partitionWeights[i]]);
			}
		}
		//for testing
		//log::out << "cw: " << "i: " << i << log::endl;
		//for (longUInt iii = mGame.getQuota(); iii < (mGame.getWeightSum() + 1); iii++){
		//	log::out << "iii: " << iii << "   " << cw[iii].uint << log::endl;			
		//}

		//get sum of vector
		for (longUInt iii = mGame.getQuota(); iii < (mGame.getQuota() + partitionWeights[i]); iii++){
			mCalculator->plusEqual(banzhafsExternalGame[i], cw[iii]);
		}

		//z101 weiter
		int nbPlayersInParti = mGame.getPrecoalitions()[i].size();
		Array2dOffset<lint::LargeNumber> cw2;
		cw2.alloc(mGame.getWeightSum() + 1, nbPlayersInParti + 1, mGame.getQuota(), 0);
		mCalculator->allocInit_largeNumberArray(cw2.getArrayPointer(), cw2.getNumberOfElements());

		//replicate vector cw onto cw2[]
		for (longUInt i = mGame.getQuota(); i < (totalWeight + 1); i++){
			cw2(i, nbPlayersInParti) = cw[i];
		}


		//for testing
		log::out << "cw2: " << log::endl;
		for (longUInt iii = mGame.getQuota(); iii < (mGame.getWeightSum() + 1); iii++){
			log::out << cw2(iii, nbPlayersInParti).uint << log::endl;			
		}

		//z110

	}

	
	return solution;
}

std::string epic::index::SymmetricCoalitionalBanzhaf::getFullName() {
	/*
	 * Return the full name of the implemented power index.
	 */
	return "SymmetricCoalitionalBanzhaf";
}

epic::longUInt epic::index::SymmetricCoalitionalBanzhaf::getMemoryRequirement() {
	/*
	 * Return an approximated value of the amount of memory needed for the power index calculation.
	 */
	bigInt memory = 42; // write the needed memory into this variable

	longUInt ret = 0;
	if (memory.fits_ulong_p()) {
		ret = memory.get_ui();
	}
	return ret;
}
