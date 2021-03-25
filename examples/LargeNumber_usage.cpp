#include "LargeNumber.h"

using namespace epic::lint;

/*
 * Before using a LargeNumber object an object implementing the ItfLargeNumberCalculator interface is needed.
 */
ItfLargeNumberCalculator* calc = ItfLargeNumberCalculator::new_calculator(420000, Operation::addition);

/*
 * Having this calculator object we are ready to use LargeNumber objects. All operations (including the allocation) get applied using this calculator object.
 * Note that only LargeNumbers created using the same calculator object are compatible for calculations.
 */

//Create simple LargeNumber object
LargeNumber num;

// allocate memory
calc->alloc_largeNumber(num);
// or calc->allocInit_largeNumber(num) for additional value initialization.

// Create an array of LargeNumber objects
int n = 7;

// allocate an array of LargeNumber objects
LargeNumber* num_array = new LargeNumber[n];

// allocate memory for each entry
calc->allocInit_largeNumberArray(num_array, n);
// here only one memory allocation gets done for the whole array saving time and memory.

/*
 * Initialize the LargeNumber object(s) if needed
 */

calc->assign_one(num);
calc->assign(num_array[0], 42);

/*
 * Do your calculations.
 * The value of each LargeNumber object must stay smaller than the max_value passed to the new_calculator() method!
 */
calc->plus(num_array[1], num, num_array[0]);
calc->increment(num_array[1]);

/*
 * Convert the calculated values
 */
BigInt result;
calc->to_bigInt(&result, num);

// For debug purposes the string conversion might me useful.
std::string str = calc->to_string(num);

/*
 * Cleanup
 */

// free the allocated memory
calc->free_largeNumber(num);
calc->free_largeNumberArray(num_array);

// delete the array
delete[] num_array;

// delete the calculator object
ItfLargeNumberCalculator::delete_calculator(calc);
