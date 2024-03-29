#ifndef EPIC_LINT_ITFLARGENUMBERCALCULATOR_H_
#define EPIC_LINT_ITFLARGENUMBERCALCULATOR_H_

#include "LargeNumber.h"
#include "types.h"

namespace epic::lint {

/**
 * Defining a mathematical operation. These are the operations that are supported by the LargeNumber classes.
 */
enum Operation {
	addition,
	multiplication
};

/**
 * A struct holding the parameters to decide what calculator to use
 */
struct CalculatorConfig {
	/**
	 * Constructor simply initializes the internal variables wth the passed values
	 *
	 * @param max_value The maximum value that should be representable using the LargeNumber (via the calculator object)
	 * @param op The most complex operation that needs to get applied by the calculator object
	 * @param rep Defining the int representation and therefore, which calculator needed (if not equal DEFAULT)
	 */
	CalculatorConfig(bigInt max_value, Operation op, IntRepresentation rep) {
		maxValue = max_value;
		this->op = op;
		intRep = rep;
	};

	bigInt maxValue;
	Operation op;
	IntRepresentation intRep = DEFAULT;
};

/**
 * Interface class for the Chinese Remainder Theorem
 * 
 * It offers operations to the set of numbers according to the Chinese Remainder Theorem:
 * 	- allocation and freeing
 * 	- mathematical operations (plus, minus, ...)
 * 
 * In addition to that this class offers two factory-methods creating objects of a derived class depending on the needed mathematical Operation.
 */
class ItfLargeNumberCalculator {
public:
	virtual ~ItfLargeNumberCalculator() = default;

	/**
	 * Getting the memory size a ChineseNumber object needs if it gets allocated using this ItfLargeNumberCalculator object
	 *
	 * @return The memory size in Bytes
	 */
	virtual longUInt getLargeNumberSize() = 0;

	/**
	* Applying the increment operator
	*
	* dest++ => dest = dest + 1
	*
	* @param dest The sum (of dest and 1)
	*/
	virtual void increment(LargeNumber& dest) = 0;

	/**
	 * Applying the addition operator according to the chinese lint theorem
	 *
	 * dest = op1 + op2
	 *
	 * @param dest The sum (of op1 and op2)
	 * @param op1 The first operand
	 * @param op2 The second operand
	 */
	virtual void plus(LargeNumber& dest, const LargeNumber& op1, const LargeNumber& op2) = 0;

	/**
	 * Applying the addition equal operator according to the chinese lint theorem
	 *
	 * dest += op => dest = dest + op
	 *
	 * @param dest The sum (of dest and op) and first operand
	 * @param op The second operand
	 */
	virtual void plusEqual(LargeNumber& dest, const longUInt& op) = 0;

	/**
	 * Applying the addition equal operator according to the chinese lint theorem
	 *
	 * dest += op => dest = dest + op
	 *
	 * @param dest The sum (of dest and op) and first operand
	 * @param op The second operand
	 */
	virtual void plusEqual(LargeNumber& dest, const LargeNumber& op) = 0;

	/**
	 * Applying the subtraction operator according to the chinese lint theorem
	 *
	 * dest = ap1 - op2
	 *
	 * @param dest The difference (of ap1 and op2)
	 * @param op1 The first operand
	 * @param op2 The second operand
	 */
	virtual void minus(LargeNumber& dest, const LargeNumber& op1, const LargeNumber& op2) = 0;

	/**
	 * Applying the subtraction equal operator according to the chinese lint theorem
	 *
	 * dest -= op => dest = dest - op
	 *
	 * @param dest The difference (of dest and op) and first operand
	 * @param op The second operand
	 */
	virtual void minusEqual(LargeNumber& dest, const LargeNumber& op) = 0;

	/**
	 * Applying the multiplication operator according to the chinese lint theorem
	 *
	 * dest = op1 * op2
	 *
	 * @param dest The product (of op1 and op2)
	 * @param op1 The first operand
	 * @param op2 The second operand
	 */
	virtual void mul(LargeNumber& dest, const LargeNumber& op1, const longUInt& op2) = 0;

	/**
	 * Applying the multiplication operator according to the chinese lint theorem
	 *
	 * dest = op1 * op2
	 *
	 * @param dest The product (of op1 and op2)
	 * @param op1 The first operand
	 * @param op2 The second operand
	 */
	virtual void mul(LargeNumber& dest, const LargeNumber& op1, const LargeNumber& op2) = 0;

	/**
	 * Applying the multiplication equal operator according to the chinese lint theorem
	 *
	 * dest *= op => dest = dest * op
	 *
	 * @param dest The product (of dest and op) and first operand
	 * @param op The second operand
	 */
	virtual void mulEqual(LargeNumber& dest, const LargeNumber& op) = 0;

	/**
	 * Applying the assignment operator according to the chinese lint theorem
	 *
	 * dest = value
	 *
	 * @param dest The assignment target
	 * @param value The assignment value
	 */
	virtual void assign(LargeNumber& dest, const LargeNumber& value) = 0;

	/**
	 * Applying the assignment operator according to the chinese lint theorem
	 *
	 * dest = value
	 *
	 * @param dest The assignment target
	 * @param value The assignment value
	 */
	virtual void assign(LargeNumber& dest, const longUInt& value) = 0;

	/**
	 * Assigning the constant c=0 to the LargeNumber dest.
	 *
	 * @param dest The assignment target
	 * @remark This method has the same effect as assign(dest, 0) but is faster since no further checks on dest are needed!
	 */
	virtual void assign_zero(LargeNumber& dest) = 0;

	/**
	 * Assigning the constant c=1 to the largeNumber dest
	 *
	 * @param dest The assignment target
	 * @remark This method has the same effect as assign(dest, 1) but is faster since no further checks on dest are needed!
	 */
	virtual void assign_one(LargeNumber& dest) = 0;

	/**
	 * Converting the value represented by value to an object of type bigInt.
	 *
	 * dest = value
	 *
	 * @remark To save temporary allocations of bigInt variables, the dest object must get passed.
	 *
	 * @param dest An pointer to the conversion target
	 * @param value The value to write to dest
	 */
	virtual void to_bigInt(bigInt* dest, const LargeNumber& value) = 0;

	/**
	 * Getting the number represented by val as string.
	 *
	 * @param val The value that should get converted to string
	 *
	 * @return A string representation of the number stored in val.
	 * @warning This method is not designed to be efficient (neither in terms of space nor in terms of time). It is intended to simplify the output for debug purposes.
	 */
	virtual std::string to_string(LargeNumber& val) = 0;

	/**
	 * Allocating the LargeNumber objects of a C-style array.
	 *
	 * This is the same functionality as calling the alloc_largeNumber() method for each array entry.
	 *
	 * @note Since this method allocates all needed memory at once, it needs less memory (and probably time) than allocating each entry separately as each dynamic memory allocation "may supply unspecified overhead [...]" (https://en.cppreference.com/w/cpp/language/new).
	 * @note To free the allocated memory use the free_largeNumberArray() method!
	 *
	 * @param array The C-style Array containing the LargeNumber objects.
	 * @param number_of_elements The number of elements contained in the array.
	 */
	virtual void alloc_largeNumberArray(LargeNumber* array, longUInt number_of_elements) = 0;

	/**
	 * Allocating and initializing the LargeNumber objects of a C-style array.
	 *
	 * This is the same functionality as calling the allocInit_largeNumber() method for each array entry.
	 *
	 * @note Since this method allocates all needed memory at once, it needs less memory (and probably time) than allocating each entry separately as each dynamic memory allocation "may supply unspecified overhead [...]" (https://en.cppreference.com/w/cpp/language/new).
	 * @note To free the allocated memory use the free_largeNumberArray() method!
	 *
	 * @param array The C-style Array containing the LargeNumber objects.
	 * @param number_of_elements The number of elements contained in the array.
	 */
	virtual void allocInit_largeNumberArray(LargeNumber* array, longUInt number_of_elements) = 0;

	/**
	 * Freeing the LargeNumber objects of a C-style array.
	 *
	 * @note This method must only get used if the array entries were allocated using either alloc_largeNumberArray() or allocInit_largeNumberArray()!
	 *
	 * @param array The C-style Array containing the allocated LargeNumber objects.
	 */
	virtual void free_largeNumberArray(LargeNumber* array) = 0;

	/**
	 * Allocating a LargeNumber object
	 *
	 * To represent a big value a LargeNumber object must contain as much integers as the ChineseRemainder object contains primes. Since the only one who knows how many primes are used is the ChineseRemainder object, this object needs to do the allocation.
	 *
	 * @note To free the allocated memory use the free_largeNumber() method!
	 *
	 * @param big_number The LargeNumber object that should get allocated
	 */
	virtual void alloc_largeNumber(LargeNumber& big_number) = 0;

	/**
	 * Allocating and initializing a LargeNumber object
	 *
	 * To represent a big value a LargeNumber object must contain as much integers as the ChineseRemainder object contains primes. Since the only one who knows how many primes are used is the ChineseRemainder object, this object needs to do the allocation. In addition to the allocation all entries inside this array are getting value initialized.
	 *
	 * @note To free the allocated memory use the free_largeNumber() method!
	 *
	 * @param big_number The LargeNumber object that should get allocated
	 */
	virtual void allocInit_largeNumber(LargeNumber& big_number) = 0;

	/**
	 * Free a LargeNumber object
	 *
	 * This function simply deletes the array of a previously allocated LargeNumber object.
	 *
	 * @note This method must only get used if the LargeNumber object was allocated using either alloc_largeNumber() or allocInit_largeNumber()!
	 *
	 * @param big_number The LargeNumber object getting freed. It must be allocated before this function call!
	 */
	virtual void free_largeNumber(LargeNumber& big_number) = 0;

	/**
	 * A Factory-Method allocating an object of type ItfLargeNumberCalculator
	 *
	 * Depending on the required configuration the correct object gets created.
	 *
	 * @param config The configuration specifying the requirements on the ItfLargeNumberCalculator object
	 *
	 * @return A new allocated object of type ItfLargeNumberCalculator is able to represent at least config.maxValue and calculate the config.op operations.
	 *
	 * @note The returned object should be deleted using delete_calculator() function.
	 */
	static ItfLargeNumberCalculator* new_calculator(const CalculatorConfig& config);

	/**
	 * A Factory-Method deleting an object of type ItfLargeNumberCalculator
	 *
	 * @param calculator The object to delete. This should have previously been allocated using the new_calculator() function.
	 */
	static void delete_calculator(ItfLargeNumberCalculator* calculator);
};

} /* namespace epic::lint */

#endif /* EPIC_LINT_ITFLARGENUMBERCALCULATOR_H_ */
