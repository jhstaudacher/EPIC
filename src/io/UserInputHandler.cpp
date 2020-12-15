#include "io/UserInputHandler.h"

#include "index/IndexFactory.h"
#include "io/DataInput.h"
#include "Logging.h"

#include <getopt.h>
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
		log::out << log::error << "The --weights option requires a non empty csv file." << log::endl;
		return false;
	}

	return true;
}

bool epic::io::UserInputHandler::handleQuotaFromWeightfile(const std::string& fileName) {
	//detect quota from weight file
	mQuota = DataInput::getQuotaFromCSV(fileName);

	if (mQuota <= 0) {
		log::out << log::error << "The --quota option requires an argument > 0." << log::endl;
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
		log::out << log::error << "The --quota option requires an argument > 0." << log::endl;
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
		log::out << log::error << "The --index option needs one of the following arguments:" << log::endl;
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
				log::out << log::error << "Missing required options: -i | --index, -w | --weights, -q | --quota" << log::endl;
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
					log::out << log::error << "Incompatible options: The options --gmp and --primes can not be used at the same time!" << log::endl;
					return false;
				}
				break;

			case OPT_PRIMES:
				if (mIntRepresentation == DEFAULT) {
					mIntRepresentation = PRIMES;
				} else {
					log::out << log::error << "Incompatible options: The options --gmp and --primes can not be used at the same time!" << log::endl;
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
				log::out << log::error << "Unknown option: " << vectorOfArguments[optind - 1] << log::endl;
				return false;

			case ':':
				log::out << log::error << "Missing arg for: " << vectorOfArguments[optind - 1] << log::endl;
				return false;

			default:
				return false;
		}
	}

	if (!handleWeights(mWeightsFile)) {
		return false;
	}

	while (optind < numberOfArguments) {
		log::out << log::info << "Other parameter: " << vectorOfArguments[optind++] << log::endl;
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
