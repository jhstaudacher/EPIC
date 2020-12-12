#include "Logging.h"

#include "../include/Logging.h"

const std::array<std::string, epic::log::Logger::cLevelCount> epic::log::Logger::mLevelNames = {
	"INFO:    ",
	"WARNING: ",
	"ERROR:   "
};

const std::string epic::log::Logger::mWhiteSpace = "         ";

epic::log::Logger::Logger(std::array<std::ostream*, cLevelCount> o_streams, level log_level) {
	mStreams = o_streams;
	mEmptyStream = new std::ostream(nullptr);
	mLevel = log_level;
	mMsgLevel = log_level;

	mUsedStream = mStreams;
	for (int l = 0; l < mLevel; ++l) {
		mUsedStream[l] = mEmptyStream;
	}
}

epic::log::Logger::~Logger() {
	for (auto it : mUsedStream) {
		it->flush();
	}
	delete mEmptyStream;
}

void epic::log::Logger::setLogLevel(level log_level) {
	mLevel = log_level;

	mUsedStream = mStreams;
	for (int l = 0; l < mLevel; ++l) {
		mUsedStream[l] = mEmptyStream;
	}
}

epic::log::level epic::log::Logger::getLogLevel() const {
	return mLevel;
}

std::ostream& epic::log::Logger::operator<<(level l) {
	mMsgLevel = l;
	*mUsedStream[l] << mLevelNames[l];
	return *mUsedStream[l];
}
