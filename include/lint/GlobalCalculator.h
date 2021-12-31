#ifndef EPIC_LINT_GLOBALCALCULATOR_H_
#define EPIC_LINT_GLOBALCALCULATOR_H_

#include "ItfLargeNumberCalculator.h"

namespace epic {
/**
	 * Global ItfLargeNumberCalculator-object allowing easy compatibility between LargeNumber-objects used in different indices
	 */
extern lint::ItfLargeNumberCalculator* gCalculator;

namespace lint {
/**
	 * Class to manage the global calculator object gCalculator
	 */
class GlobalCalculator {
public:
	GlobalCalculator() = delete;
	~GlobalCalculator() = delete;

	/**
		 * Initialize the gCalculator object based on the passed CalculatorConfig
		 *
		 * @param config The configuration for the gCalculator
		 *
		 * @note If gCalculator is already initialized, i.e. gCalculator != nullptr, a bad_function_call exception gets thrown.
		 */
	static void init(const CalculatorConfig& config);

	/**
		 * Delete the gCalculator object
		 *
		 * @note Call this function only after a call to init(), as gCalculator can only be deleted after initialization. If it is not initialized, i.e. gCalculator == nullptr, a bad_function_call exception gets thrown.
		 */
	static void remove();
};

} /* namespace lint */
} /* namespace epic */

#endif /* EPIC_LINT_GLOBALCALCULATOR_H_ */
