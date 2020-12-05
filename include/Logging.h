#ifndef EPIC_LOGGING_H_
#define EPIC_LOGGING_H_

#include <string>

namespace epic::log {

static const std::string invalidFlagF = "This power index is null player sensitive! Retry without the '-f' flag.";
static const std::string missingFlagF = "This power index is not defined for null players! Retry with the '-f' flag.";

} // namespace epic::log

#endif //EPIC_LOGGING_H_
