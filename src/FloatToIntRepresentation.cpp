#include "FloatToIntRepresentation.h"

#include <vector>

float UpscaleFloatToIntAndReturnMultiplicator(std::vector<float>& floats, std::vector<epic::longUInt>& ints) {
	ints.resize(floats.size());

	bool factorIsFound = false;

	// definitions for rounding accuracy
	float multiplicator = 1.0f;
	// how much the multiplicator increases after each check
	float incrementor = 0.5f;
	// maximum difference for each float from the next full number
	float maxDifference = 0.1f;
	// placeholder variable for computing differences
	float difference = 0.0;

	for (int i = 0; i < 100000 && !factorIsFound; i++) {
		multiplicator += incrementor;
		factorIsFound = true;
		for (auto it : floats) {
			difference = (it * multiplicator) - (float)((int)(it * multiplicator));
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
