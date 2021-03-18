#include "FloatToIntRepresentation.h"

double UpscaleFloatToIntAndReturnMultiplicator(std::vector<double>& floats, std::vector<epic::longUInt>& ints) {
	ints.resize(floats.size());

	bool factorIsFound = false;

	// definitions for rounding accuracy
	double multiplicator = 1.0;
	// how much the multiplicator increases after each check
	double incrementor = 0.5;
	// maximum difference for each float from the next full number
	double maxDifference = 0.1;
	// placeholder variable for computing differences
	double difference = 0.0;

	for (int i = 0; i < 100000 && !factorIsFound; i++) {
		multiplicator += incrementor;
		factorIsFound = true;
		for (auto it : floats) {
			difference = (it * multiplicator) - (double)((int)(it * multiplicator));
			if (difference > maxDifference || difference < -maxDifference) {
				factorIsFound = false;
			}
		}
	}

	for (unsigned int i = 0; i < ints.size(); i++) {
		ints[i] = (int)(floats[i] * multiplicator);
	}

	return multiplicator;
}
