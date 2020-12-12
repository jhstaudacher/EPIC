#ifndef EPIC_LOG_LOGGING_H_
#define EPIC_LOG_LOGGING_H_

#include <iostream>
#include <string>
#include <array>
#include <map>

namespace epic::log {

/**
 * Log-Message for the case that the `-f` flag was set although it is not compatible with the selected index.
 */
static const std::string invalidFlagF = "This power index is null player sensitive! Retry without the '-f' flag.";

/**
 * Log-Message for the case that the `-f` flag was not set although the index (and the given weights) require it.
 */
static const std::string missingFlagF = "This power index is not defined for null players! Retry with the '-f' flag.";


/**
 * An enumeration type of all supported logging-levels. The > relation is defined as follows:
 * info < warning < error
 */
enum level {
	info = 0,
	warning,
	error
};

using std::endl; // enables the usage of log::endl

/**
 * A class allowing logging with different levels of message severity (see level enum). By setting the Logger objects level, all messages with a lower severity get not printed anywhere and does not cost much time (to print nothing) in that case.
 */
class Logger {
private:
	/**
	 * The number of available levels (i.e. number of level enum entries).
	 */
	static constexpr int cLevelCount = level::error + 1;

	/**
	 * Stores the name for each log level. This name gets printed when a level object gets shifted into a Logger instance (e.g. Logger << warning).
	 */
	static const std::array<std::string, cLevelCount> mLevelNames;

	/**
	 * The white space that gets printed at first every time when shifting and object (other than level) to a Logger instance (e.g. Logger << "log message"). This is intended to align all messages.
	 */
	static const std::string mWhiteSpace;

	/**
	 * The stream objects to write the messages of certain severity to (if mMsgLevel >= mLevel)
	 */
	std::array<std::ostream*, cLevelCount> mStreams;

	/**
	 * An stream object writing nothing (if mMsgLevel < mLevel)
	 */
	std::ostream* mEmptyStream;

	/**
	 * Stores either mStreams[log-level] or mEmptyStream pointer for each log-level (depending on the log level mLevel).
	 */
	std::array<std::ostream*, cLevelCount> mUsedStream;

	/**
	 * The logging bound. If the mMsgLevel is below mLevel the logger does nothing.
	 */
	level mLevel;

	/**
	 * The level of the current log-message (needed for new lines). This is the level that was shifted to the object at last.
	 */
	level mMsgLevel;

public:
	/**
	 * @param o_stream The stream object where the log messages should get written to.
	 * @param log_level The level bound for printing messages.
	 *
	 * @note The current log level will be set to log_level.
	 */
	explicit Logger(std::array<std::ostream*, cLevelCount> o_streams = {&std::cout, &std::cout, &std::cerr}, level log_level = info);
	~Logger();

	/**
	 * Setting the logging bound. Messages with a level less important than log_level will not get printed.
	 *
	 * @param log_level The logging bound
	 */
	void setLogLevel(level log_level);

	/**
	 * Get the current log bound
	 *
	 * @return The log bound
	 */
	level getLogLevel() const;

	/**
	 * The method for shifting log-levels to the Logger class. This starts a new log-message of the given log-level l.
	 *
	 * @param l The log level
	 * @return A std::ostream object (As set by the constructor or an empty ostream if l < getLogLevel()).
	 *
	 * @note Since this method returns an std::ostream object everything getting passed to this object is out of control of the Logger class! Therefore the Logger class itself should be used (for shift operators) after each endl.
	 */
	std::ostream& operator<<(level l);

	/**
	 * The shift operation for any type of object. This method is intended to be used after a endl command as it prints whitespaces bevor the val to get the messages aligned.
	 *
	 * @tparam T The type of the object that gets shifted
	 * @param val The object of type T that gets shifted
	 * @return A std::ostream object (As set by the constructor or an empty ostream if the current mMsgLevel < getLogLevel()).
	 *
	 * @note Since this method returns an std::ostream object everything getting passed to this object is out of control of the Logger class! Therefore the Logger class itself should be used (for shift operators) after each endl.
	 */
	template<typename T>
	std::ostream& operator<<(const T& val) {
		*mUsedStream[mMsgLevel] << mWhiteSpace << val;
		return *mUsedStream[mMsgLevel];
	}
};

/**
 * A global Logger instance to be used everywhere in the project. Nonetheless it is of course possible to create additional instances if needed.
 */
static Logger out;

} // namespace epic::log

#endif //EPIC_LOG_LOGGING_H_
