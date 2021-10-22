#include "index/BanzhafOwen.h"
#include "Logging.h"
#include <cmath>

epic::index::BanzhafOwen::BanzhafOwen(Game& g, ItfUpperBoundApproximation* approx, IntRepresentation int_representation) : ItfPowerIndex(g) {
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

epic::index::BanzhafOwen::~BanzhafOwen() {
	/*
	 * Delete the mCalculator object
	 */
	lint::ItfLargeNumberCalculator::delete_calculator(mCalculator);

	/**
	 * If needed do additional cleanup below
	 */
}

std::vector<epic::bigFloat> epic::index::BanzhafOwen::calculate() {
	/*
	 * Use mGame and mCalculator to calculate the power index and return the results.
	 */
	int nbPlayers = mGame.getNumberOfPlayers();
	longUInt nbPartitions = mGame.getPrecoalitions().size();
	longUInt totalWeight = mGame.getWeightSum();
	std::vector<bigFloat> solution(nbPlayers);
	std::vector<lint::LargeNumber> banzhafsExternalGame(nbPartitions);
	std::vector<longUInt> partitionWeights;

	for(auto& row:mGame.getPrecoalitions()){
		longUInt weight = 0;
		for(auto& col:row){
			  weight += mGame.getWeights().at(col);
		}
		partitionWeights.push_back(weight);
	}

	ArrayOffset<lint::LargeNumber> c(totalWeight + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(c.getArrayPointer(), c.getNumberOfElements());
	mCalculator->assign_one(c[totalWeight]);

	for (longUInt i = 0; i < nbPartitions; i++) {
		int wi = partitionWeights[i];
		for (longUInt k = mGame.getQuota() + wi; k <= totalWeight; ++k) {
			mCalculator->plusEqual(c[k - wi], c[k]);
		}
	}

	ArrayOffset<lint::LargeNumber> cw(totalWeight + 1, mGame.getQuota());
	mCalculator->allocInit_largeNumberArray(cw.getArrayPointer(), cw.getNumberOfElements());
	
	//replicate vector c onto cw
	for (longUInt i = mGame.getQuota(); i < (totalWeight + 1); i++){
		cw[i] = c[i];
	}

	// Work out number of swings on level of precoalitions
	for (longUInt i = 0; i < nbPartitions; i++){
		for (longUInt ii = mGame.getQuota(); ii < (totalWeight + 1); ii++){
			cw[ii] = c[ii];
		}
		for(longUInt ii = (totalWeight - partitionWeights[i]); ii >= mGame.getQuota(); ii = ii - 1){
			mCalculator->minus(cw[ii], c[ii], cw[ii + partitionWeights[i]]);
		}

		//for testing
		//log::out << "cw: " << log::endl;
		//for (longUInt iii = mGame.getQuota(); iii < (mGame.getWeightSum() + 1); iii++){
		//	log::out << cw[iii].uint << log::endl;			
		//}

		//get sum of vector
		for (longUInt iii = mGame.getQuota(); iii < (mGame.getQuota() + partitionWeights[i]); iii++){
			mCalculator->plusEqual(banzhafsExternalGame[i], cw[iii]);
		}
		//log::out << "banzhaf sum: " << banzhafsExternalGame[i].uint << log::endl;

		//create cw2
		ArrayOffset<lint::LargeNumber> cw2(totalWeight + 1, mGame.getQuota());
		mCalculator->allocInit_largeNumberArray(cw2.getArrayPointer(), cw2.getNumberOfElements());
		//replicate vector c onto cw
		for (longUInt i = mGame.getQuota(); i < (totalWeight + 1); i++){
			cw2[i] = cw[i];
		}
		

		int nbPlayersInParti = mGame.getPrecoalitions()[i].size();		
		bigFloat ExternalMultiplier = 1 / (pow(2, (nbPartitions)-1));
		bigFloat InternalMultiplier = 1;
		if (nbPlayersInParti > 1){
			std::vector<lint::LargeNumber> banzhafsInternal(mGame.getPrecoalitions()[i].size());
			
			InternalMultiplier = 1/(pow(2, nbPlayersInParti-1));
			for (int ii = 0; ii < nbPlayersInParti; ii++){
				longUInt wii = mGame.getWeights()[mGame.getPrecoalitions()[i][ii]];
				for (longUInt iii = (mGame.getQuota() + wii); iii <= totalWeight; iii++){
					mCalculator->plusEqual(cw2[iii - wii], cw2[iii]);					
				}
			}
			//log::out << "testing cw2" << log::endl;
			//
			//for (longUInt i = mGame.getQuota(); i < (totalWeight + 1); i++){
			//	log::out << cw2[i].uint << log::endl;
			//}

			ArrayOffset<lint::LargeNumber> cwi(totalWeight + 1, mGame.getQuota());
			mCalculator->allocInit_largeNumberArray(cwi.getArrayPointer(), cwi.getNumberOfElements());

			for (int ii = 0; ii < nbPlayersInParti; ii++){
				longUInt wii = mGame.getWeights()[mGame.getPrecoalitions()[i][ii]];
				//replicate vector cw2 onto cwi
				for (longUInt i = mGame.getQuota(); i < (totalWeight + 1); i++){
					cwi[i] = cw2[i];
				}
				for(longUInt x = (totalWeight - wii); x >= mGame.getQuota(); x = x - 1){
					mCalculator->minus(cwi[x], cw2[x], cwi[x + wii]);
				}

				//test cwi
				//log::out << "cwi: " << log::endl;
				//for (longUInt iii = mGame.getQuota(); iii < (mGame.getWeightSum() + 1); iii++){
				//	log::out << cwi[iii].uint << log::endl;;
				//}

				for (longUInt iii = mGame.getQuota(); iii <= (mGame.getQuota() + wii - 1); iii++){
					mCalculator->plusEqual(banzhafsInternal[ii], cwi[iii]);
				}
				//log::out << "banzhafsInternal: " << banzhafsInternal[ii].uint << log::endl;

				solution[mGame.getPrecoalitions()[i][ii]] = ExternalMultiplier * InternalMultiplier * banzhafsInternal[ii].uint;
			}

		}
		else{
			solution[mGame.getPrecoalitions()[i][0]] = ExternalMultiplier * banzhafsExternalGame[i].uint;
			log::out << "banzhafsExternalGame[i].uint: " << banzhafsExternalGame[i].uint << log::endl;
		}

	}
	return solution;
}

std::string epic::index::BanzhafOwen::getFullName() {
	/*
	 * Return the full name of the implemented power index.
	 */
	return "BanzhafOwen";
}

epic::longUInt epic::index::BanzhafOwen::getMemoryRequirement() {
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
