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
	std::vector<bigFloat> scbIndicesCheck(nbPlayers);
	std::vector<lint::LargeNumber> banzhafsExternalGame(nbPartitions);
	std::vector<longUInt> partitionWeights;
	longUInt maxMembersInPartition = 1;

	//log::out << "weights: " << log::endl;
	for(auto& row:mGame.getPrecoalitions()){
		longUInt weight = 0;
		for(auto& col:row){
			  weight += mGame.getWeights().at(col-1);
			  if (maxMembersInPartition < row.size()){
				  maxMembersInPartition = row.size();
			  }
		}
		partitionWeights.push_back(weight);
		//log::out << weight << log::endl;
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

	bigFloat ExternalMultiplier = 1 / (pow(2, (nbPartitions)-1));
	int player = 1;
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
		//log::out << "cw2: " << log::endl;
		//for (longUInt iii = mGame.getQuota(); iii < (mGame.getWeightSum() + 1); iii++){
		//	log::out << cw2(iii, nbPlayersInParti-1).uint << log::endl;			
		//}

		//z110
		//log::out << "nbPlayersInParti: " << nbPlayersInParti << log::endl;
		if (nbPlayersInParti > 1){
			//winternal = mGame.getWeights
			std::vector<bigFloat> shapleysInternal(mGame.getPrecoalitions()[i].size());
			std::vector<bigFloat> rawShapleysInternal(mGame.getPrecoalitions()[i].size());

			//cw2(mGame.getWeightSum(), nbPlayersInParti).uint = 1;
			mCalculator->assign_one(cw2(mGame.getWeightSum(), nbPlayersInParti));
			for (int ii = 0; ii < nbPlayersInParti; ii++){
				longUInt x = mGame.getQuota() + mGame.getWeights()[mGame.getPrecoalitions()[i][ii]-1];
				//log::out << "capitcalC: " << mGame.getWeightSum() << log::endl;
				//log::out << "x: " << x << log::endl;
				//log::out << "mGame.getWeights()[mGame.getPrecoalitions()[i][ii]]: " << mGame.getWeights()[mGame.getPrecoalitions()[i][ii]] << log::endl;
				//log::out << "mGame.getQuota(): " << mGame.getQuota() << log::endl;
				while (x <= mGame.getWeightSum()){
					int m = nbPlayersInParti;
					while (m>1){
						//cw2[x-winternal[ii],m-1] = cw2[x,m] + cw2[x-winternal[ii],m-1]
						//cw2(x - mGame.getWeights()[mGame.getPrecoalitions()[i][ii]], m-1).uint = cw2(x, m).uint + cw2(x-mGame.getWeights()[mGame.getPrecoalitions()[i][ii]], m-1).uint;
						mCalculator->plus(cw2(x - mGame.getWeights()[mGame.getPrecoalitions()[i][ii]-1], m-1), cw2(x, m), cw2(x-mGame.getWeights()[mGame.getPrecoalitions()[i][ii]-1], m-1));	
						//log::out << "x-winternal[ii]: " << x - mGame.getWeights()[mGame.getPrecoalitions()[i][ii]-1] << log::endl;
						//log::out << "m-1: " << m-1 << log::endl;
						//log::out << "x: " << x << log::endl;
						
						m = m - 1;
					}
					//log::out << "x gets + 1" << log::endl;
					x = x + 1;
				}
				//log::out << "--------------------" << log::endl;

			}
			//z137
			//log::out << "cw2 after: " << log::endl;
			//for (longUInt iii = mGame.getQuota(); iii < (mGame.getWeightSum() + 1); iii++){
			//	log::out << cw2(iii, nbPlayersInParti-1).uint << log::endl;			
			//}
			//z150
			Array2dOffset<lint::LargeNumber> cwi;
			cwi.alloc(mGame.getWeightSum() + 1, nbPlayersInParti + 1, mGame.getQuota(), 0);
			mCalculator->allocInit_largeNumberArray(cwi.getArrayPointer(), cwi.getNumberOfElements());

			for (int ii = 0; ii < nbPlayersInParti; ii++){
				//replicate cw2 onto cwi
				for (longUInt x = mGame.getQuota(); x <= mGame.getWeightSum(); x++){
					for (int xx = 0; xx <= nbPlayersInParti; xx++){
						cwi(x, xx) = cw2(x, xx);
					}
				}
				//if ((sum(w)-winternal[ii]) >= q){
				if ((mGame.getWeightSum()-mGame.getWeights()[mGame.getPrecoalitions()[i][ii]-1]) >= mGame.getQuota()){
					//for (x in seq(from = (sum(w)-winternal[ii]), to=q, by=-1)){
					for(longUInt x = (mGame.getWeightSum() - mGame.getWeights()[mGame.getPrecoalitions()[i][ii]-1]); x >= mGame.getQuota(); x = x - 1){
						int sinternal = 1;
						//log::out << "x: " << x << log::endl;
						while (sinternal < nbPlayersInParti){
							//cwi(x, nbPlayersInparti-sinternal) = cw2(x, nbPlayersInParti-sinternal) - cwi(x+mGame.getWeights()[mGame.getPrecoalitions()[i][ii]-1], nbPlayersInParti-sinternal+1);
							mCalculator->minus(cwi(x, nbPlayersInParti-sinternal), cw2(x, nbPlayersInParti-sinternal), cwi(x+mGame.getWeights()[mGame.getPrecoalitions()[i][ii]-1], nbPlayersInParti-sinternal+1));
							sinternal = sinternal + 1;
						}
					}
				}
				//for testing cwi
				//log::out << "cwi: " << log::endl;
				//for (longUInt iii = mGame.getQuota(); iii < (mGame.getWeightSum() + 1); iii++){
				//	log::out << cwi(iii, nbPlayersInParti-1).uint << log::endl;			
				//}

				//z167
				//for (s in 0:(nbPlayersInParti-1)){
				for (int s = 0; s <= (nbPlayersInParti-1); s++){
					//factor = factorial(s)*factorial(nbPlayersInParti-s-1)
					bigInt factorial_s; // = n!
					//calc factorial
					{
						if (s <= 1){
							factorial_s = 1;
						}
						else{
							// factorial[x]: x!
							auto factorial = new bigInt[s + 1];
							factorial[0] = 1;
							factorial[1] = 1;

							for (int i = 2; i <= s; ++i) {
								factorial[i] = factorial[i - 1] * i;
							}

							factorial_s = factorial[s];
							delete[] factorial;
						}
					}
					bigInt factorial_nb; // = n!
					//calc factorial
					{
						if ((nbPlayersInParti-s-1) <= 1){
							factorial_nb = 1;
						}
						else{
							// factorial[x]: x!
							auto factorial = new bigInt[(nbPlayersInParti-s-1) + 1];
							factorial[0] = 1;
							factorial[1] = 1;

							for (int i = 2; i <= (nbPlayersInParti-s-1); ++i) {
								factorial[i] = factorial[i - 1] * i;
							}

							factorial_nb = factorial[(nbPlayersInParti-s-1)];
							delete[] factorial;
						}
					}
					//log::out << "factorial_s: " << factorial_s << log::endl;
					//log::out << "factorial_nb: " << factorial_nb << log::endl;

					//lint::LargeNumber factor = factorial_nb * factorial_s;
					bigInt factor;
					factor = factorial_nb * factorial_s;
					//shapleysInternal[ii] = shapleysInternal[ii] + (factor * sum(cwi[(q:(q+winternal[ii]-1)),s+1]))
					bigInt sum = 0;
					for (longUInt iii = mGame.getQuota(); iii < (mGame.getQuota() + mGame.getWeights()[mGame.getPrecoalitions()[i][ii]-1]); iii++){
						//mCalculator->plus(sum, sum, cwi(iii, s+1));
						sum = sum + cwi(iii, s+1).uint;
						//log::out << "------cwi: " << cwi(iii, s+1).uint << log::endl;
					}
					log::out << "sum: " << sum << log::endl;
					//mCalculator->mul(sum, sum, factor);
					//mCalculator->plus(shapleysInternal[ii], shapleysInternal[ii], sum);
					sum = sum * factor;
					//log::out << "factor: " << factor << log::endl;
					//log::out << "winternal: " << mGame.getWeights()[mGame.getPrecoalitions()[i][ii]-1] << log::endl;
					shapleysInternal[ii] = shapleysInternal[ii] + sum;
					rawShapleysInternal[ii] = shapleysInternal[ii];
					log::out << "shapleysInternal[ii]: " << shapleysInternal[ii] << log::endl;

				}

				bigInt factorial_nbPlayersInParti; // = n!
				//calc factorial
				{
					if ((nbPlayersInParti) <= 1){
						factorial_nbPlayersInParti = 1;
					}
					else{
						// factorial[x]: x!
						auto factorial = new bigInt[(nbPlayersInParti) + 1];
						factorial[0] = 1;
						factorial[1] = 1;
						for (int i = 2; i <= (nbPlayersInParti); ++i) {
							factorial[i] = factorial[i - 1] * i;
						}
						factorial_nbPlayersInParti = factorial[(nbPlayersInParti)];
						delete[] factorial;
					}
				}				
				solution[mGame.getPrecoalitions()[i][ii]-1] = ExternalMultiplier * shapleysInternal[ii] / factorial_nbPlayersInParti;
				log::out << "player: " << player << log::endl;
				player++;
				log::out << "--------------------" << log::endl;
			}
			bigFloat sumRawShapleysInternal;
			for (const auto& value: rawShapleysInternal){
				sumRawShapleysInternal = sumRawShapleysInternal + value;
			}

			//z193
			for (int ii = 0; ii < nbPlayersInParti; ii++){
				scbIndicesCheck[mGame.getPrecoalitions()[i][ii]-1] = ExternalMultiplier * banzhafsExternalGame[i].uint * rawShapleysInternal[ii] / sumRawShapleysInternal;
			}
		}
		else{
			solution[mGame.getPrecoalitions()[i][0]-1] = ExternalMultiplier * banzhafsExternalGame[i].uint;
			scbIndicesCheck[mGame.getPrecoalitions()[i][0]-1] = ExternalMultiplier * banzhafsExternalGame[i].uint;
		}

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
