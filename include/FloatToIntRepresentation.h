#ifndef EPIC_FLOATTOINTREPRESENTATION_H_
#define EPIC_FLOATTOINTREPRESENTATION_H_

#include "types.h"

#include <vector>

/**
 * Utility to find an integer representation of a given floating-point problem, in no way minimal or optimal
 *
 * The function UpscaleFloatToIntAndReturnMultiplicator takes in a float and an int vector and overwrites the int * vector after calculating the necessary multiplicator with the correct rescaling of the weights
 * The function always enlarges the results. E.g. if you have a vector vector1 = {50.0, 50.0, 10.0} as input, you will get an integer vector of the following form back: vector2 = {75, 75, 15}
 * There are 100,000 steps of trying to find a common factor, if after that time no common factor is found, the currently approximated result will be returned
 *
 * @param floats the float vector containing the values to be rescaled to integers
 * @param ints the int vector containing the rescaled results
*/
double UpscaleFloatToIntAndReturnMultiplicator(std::vector<double>& floats, std::vector<epic::longUInt>& ints);

#endif
