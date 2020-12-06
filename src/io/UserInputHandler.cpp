#include "io/UserInputHandler.h"

#include "index/IndexFactory.h"
#include "io/DataInput.h"

#include <cstring>
#include <getopt.h>
#include <iostream>
#include <string>

epic::io::UserInputHandler::UserInputHandler(const std::string& index, const std::vector<longUInt>& weights, longUInt quota, OutputType outputType, bool filterNullPlayers, bool verbose) {
	mIndex = index;
	mWeights = weights;
	mQuota = quota;
	mOutputType = outputType;
	mTestFlag = false;
	mInputFloatWeights = false;
	mFilterNullPlayers = filterNullPlayers;
	mIntRepresentation = DEFAULT;
	mVerbose = verbose;
	mWeightsFile = "";
}

epic::io::UserInputHandler::UserInputHandler() {
	mIndex = "Invalid";
	mWeights.clear();
	mQuota = 0;
	mOutputType = screen;
	mTestFlag = false;
	mInputFloatWeights = false;
	mFilterNullPlayers = false;
	mIntRepresentation = DEFAULT;
	mVerbose = false;
	mWeightsFile = "";
}

bool epic::io::UserInputHandler::handleWeights(const std::string& fileName) {
	if (mInputFloatWeights) {
		std::vector<float> floatWeights;
		floatWeights = DataInput::inputFloatCSV(fileName, mTestFlag);
		float multiplicator = UpscaleFloatToIntAndReturnMultiplicator(floatWeights, mWeights);
		mQuota = multiplicator * mQuota;
	} else {
		mWeights = DataInput::inputCSV(fileName, mTestFlag);
	}

	if (mWeights.empty()) {
		std::cout << "The --weights option requires a non empty csv file." << std::endl;
		return false;
	}

	return true;
}

bool epic::io::UserInputHandler::handleQuotaFromWeightfile(const std::string& fileName) {
	//detect quota from weight file
	mQuota = DataInput::getQuotaFromCSV(fileName);

	if (mQuota <= 0) {
		std::cout << "The --quota option requires an argument > 0." << std::endl;
		return false;
	}

	mTestFlag = true;
	return true;
}

bool epic::io::UserInputHandler::handleQuota(char* value) {
	mQuota = std::atoi(value);

	if (mQuota > 0) {
		return true;
	} else {
		std::cout << "The --quota option requires an argument > 0." << std::endl;
		return false;
	}
}

bool epic::io::UserInputHandler::handleIndex(char* value) {
	bool ret = false;

	std::string i_str = std::string(value);
	if (index::IndexFactory::validateIndex(i_str)) {
		mIndex = i_str;
		ret = true;
	} else {
		std::cout << "The --index option needs one of the following arguments:" << std::endl;
		index::IndexFactory::printIndexList(std::cout);
	}

	return ret;
}

bool epic::io::UserInputHandler::parseCommandLine(int numberOfArguments, char* vectorOfArguments[]) {
	enum LongOpts {
		OPT_GMP,
		OPT_PRIMES,
		FLOAT,
		CSV
	};

	static struct option long_options[] = {
		{"index", required_argument, nullptr, 'i'},
		{"weights", required_argument, nullptr, 'w'},
		{"quota", required_argument, nullptr, 'q'},
		{"verbose", no_argument, nullptr, 'v'},
		{"filter-null", no_argument, nullptr, 'f'},
		{"gmp", no_argument, nullptr, OPT_GMP},
		{"primes", no_argument, nullptr, OPT_PRIMES},
		{"float", no_argument, nullptr, FLOAT},
		{"csv", no_argument, nullptr, CSV},
		{"help", no_argument, nullptr, 'h'},
		{nullptr, 0, nullptr, 0}};
	int arg_count = 0;

	while (true) {
		int index = -1;
		//struct option * opt = 0;
		int result = getopt_long(numberOfArguments, vectorOfArguments, ":i:w:q:vfmh", long_options, &index);

		if (result == -1) {
			if (arg_count < 3) {
				std::cout << "Missing required options: -i | --index, -w | --weights, -q | --quota" << std::endl;
				return false;
			}
			break;
		}

		//opt = (struct option *)&(long_options[index]);
		switch (result) {
			case 'i':
				if (!handleIndex(optarg)) {
					return false;
				}
				arg_count++;
				break;

			case 'w':
				mWeightsFile = optarg;
				arg_count++;
				break;

			case 'q':
				if (std::atoi(optarg) == 0) {
					//No quota specified -> search in weightfile
					if (!handleQuotaFromWeightfile(mWeightsFile)) {
						return false;
					}
				} else {
					if (!handleQuota(optarg)) {
						return false;
					}
				}
				arg_count++;
				break;

			case 'v':
				mVerbose = true;
				break;

			case 'f':
				mFilterNullPlayers = true;
				break;

			case 'h':
				std::cout << "epic [OPTIONS]" << std::endl
						  << std::endl;
				std::cout << "required OPTIONS:" << std::endl
						  << MSG_REQUIRED_OPTS << std::endl;
				std::cout << "optional OPTIONS:" << std::endl
						  << MSG_OPTIONAL_OPTS << std::endl;
				std::cout << "Index abbreviations:" << std::endl;
				index::IndexFactory::printIndexList();
				exit(-1);

			case OPT_GMP:
				if (mIntRepresentation == DEFAULT) {
					mIntRepresentation = GMP;
				} else {
					std::cout << "Incompatible options: The options --gmp and --primes can not be used at the same time!" << std::endl;
					return false;
				}
				break;

			case OPT_PRIMES:
				if (mIntRepresentation == DEFAULT) {
					mIntRepresentation = PRIMES;
				} else {
					std::cout << "Incompatible options: The options --gmp and --primes can not be used at the same time!" << std::endl;
					return false;
				}
				break;

			case FLOAT:
				mInputFloatWeights = true;
				break;

			case CSV:
				mOutputType = csv;
				break;

			case '?':
				std::cout << "Unknown option: " << vectorOfArguments[optind - 1] << std::endl;
				return false;

			case ':':
				std::cout << "Missing arg for: " << vectorOfArguments[optind - 1] << std::endl;
				return false;

			default:
				return false;
		}
	}

	if (!handleWeights(mWeightsFile)) {
		return false;
	}

	while (optind < numberOfArguments) {
		std::cout << "Other parameter: " << vectorOfArguments[optind++] << std::endl;
	}

	return true;
}

std::vector<epic::longUInt>& epic::io::UserInputHandler::getWeights() {
	return mWeights;
}

epic::longUInt epic::io::UserInputHandler::getQuota() const {
	return mQuota;
}

epic::io::OutputType epic::io::UserInputHandler::getOutputType() const {
	return mOutputType;
}

std::string epic::io::UserInputHandler::getIndexToCompute() const {
	return mIndex;
}

bool epic::io::UserInputHandler::doFilterNullPlayers() const {
	return mFilterNullPlayers;
}

epic::IntRepresentation epic::io::UserInputHandler::getIntRepresentation() const {
	return mIntRepresentation;
}

bool epic::io::UserInputHandler::isVerbose() const {
	return mVerbose;
}

std::string epic::io::UserInputHandler::getWeightsFileName() const {
	return mWeightsFile;
}
