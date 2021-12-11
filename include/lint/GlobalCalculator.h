#ifndef EPIC_LINT_GLOBALCALCULATOR_H_
#define EPIC_LINT_GLOBALCALCULATOR_H_

#include "ItfLargeNumberCalculator.h"

namespace epic {
	extern lint::ItfLargeNumberCalculator* gCalculator;

namespace lint {
	class GlobalCalculator {
	private:
		//static CalculatorConfig mConfig;
		//static ItfLargeNumberCalculator* mCalculator;

	public:
		GlobalCalculator() = delete;
		~GlobalCalculator() = delete;

		static void init(const CalculatorConfig& config);
		static void remove();
		//static CalculatorConfig getConfig();
	};

} /* namespace lint */
} /* namespace epic */

#endif /* EPIC_LINT_GLOBALCALCULATOR_H_ */
