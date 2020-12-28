#ifndef EPIC_LINT_LARGENUMBER_H_
#define EPIC_LINT_LARGENUMBER_H_

#include "types.h"

namespace epic::lint {

/**
 * Storing the values representing a large integer number (the representation depends on the used ItfLargeNumberCalculator).
 *
 * This union can only be used in combination of an ItfLargeNumberCalculator object. Depending on that object either the uint, array or bint member get used.
 * First of all the union must be initialized using the ItfLargeNumberCalculator::alloc_largeNumber() or ItfLargeNumberCalculator::allocInit_largeNumber() method. After allocation/initialization  different mathematical operations can get applied to the ChineseNumber object using the ItfLargeNumberCalculator methods (e.g. assign(), plus(), plusEqual(), etc.). After usage the object must be freed using the ItfLargeNumberCalculator::free_largeNumber() method.
 *
 * @note ALL operations to a LargeNumber object must get applied using one single instance of ItfLargeNumberCalculator.
 * @note Before using a LargeNumber it must get allocated (and afterwards freed)! When the chinese lint theorem is used the array member gets allocated.
 */
union LargeNumber {
	longUInt uint;	 // single integer: used if no primes (chinese remainder theorem) is needed to represent the value
	longUInt* array; // an array of integers (same number as the primes) representing a big number
	bigInt* bint;	 // pointer to an BigInt object (needed to save memory for very large problems since bigInt only allocates as much memory as it needs)
};

} /* namespace epic::lint */

#endif /* EPIC_LINT_LARGENUMBER_H_ */
