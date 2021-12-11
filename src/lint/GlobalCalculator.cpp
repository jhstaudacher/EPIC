#include "lint/GlobalCalculator.h"

#include <functional>

epic::lint::ItfLargeNumberCalculator* epic::gCalculator = nullptr;

void epic::lint::GlobalCalculator::init(const CalculatorConfig& config) {
	if (gCalculator != nullptr) {
		throw std::bad_function_call(); // "The global Calculator-object is already initialized!"
	}

	gCalculator = ItfLargeNumberCalculator::new_calculator(config);
	//mConfig = config;
}

void epic::lint::GlobalCalculator::remove() {
	if (gCalculator == nullptr) {
		throw std::bad_function_call(); // "The global Calculator-object is not initialized!"
	}

	ItfLargeNumberCalculator::delete_calculator(gCalculator);
	gCalculator = nullptr;
}

//epic::lint::CalculatorConfig epic::lint::GlobalCalculator::getConfig() {
//	return mConfig;
//}
