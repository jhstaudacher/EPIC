#include "io/UserInputHandler.h"

#include "Logging.h"
#include "index/IndexFactory.h"
#include "io/DataInput.h"

#include <getopt.h>
#include <string>

epic::io::UserInputHandler::UserInputHandler(const std::string& index, const std::vector<longUInt>& weights, longUInt quota, OutputType outputType, bool filterNullPlayers) {
	mIndex = index;
	mWeights = weights;
	mQuota = quota;
	mFloatQuota = 0.0;
	mOutputType = outputType;
	mTestFlag = false;
	mPrecoalitionFlag = false;
	mInputFloatWeights = false;
	mFilterNullPlayers = filterNullPlayers;
	mIntRepresentation = DEFAULT;
	mWeightsFile = "";
	mPrecoalitions.clear();
}

epic::io::UserInputHandler::UserInputHandler() {
	mIndex = "Invalid";
	mWeights.clear();
	mQuota = 0;
	mFloatQuota = 0.0;
	mOutputType = screen;
	mTestFlag = false;
	mPrecoalitionFlag = false;
	mInputFloatWeights = false;
	mFilterNullPlayers = false;
	mIntRepresentation = DEFAULT;
	mWeightsFile = "";
	mPrecoalitions.clear();
}

bool epic::io::UserInputHandler::handleWeightsAndQuota(const std::string& fileName) {
	if (mInputFloatWeights) {
		std::vector<double> floatWeights;
		floatWeights = DataInput::inputFloatCSV(fileName, mPrecoalitions, mTestFlag, mPrecoalitionFlag);
		floatWeights.push_back(mFloatQuota);
		UpscaleFloatToIntAndReturnMultiplicator(floatWeights, mWeights);
		mQuota = mWeights.back();
		mWeights.pop_back();
	} else {
		if (static_cast<longUInt>(mFloatQuota) == mFloatQuota) {
			mQuota = static_cast<longUInt>(mFloatQuota);
		} else {
			std::cout << "Float quota specified without the --float flag." << std::endl;
			return false;
		}
		mWeights = DataInput::inputCSV(fileName, mPrecoalitions, mTestFlag, mPrecoalitionFlag);

		//test coalition structure
		//std::vector< std::vector<int> >::iterator row;
		//std::vector<int>::iterator col;
		//for (row = mPrecoalitions.begin(); row != mPrecoalitions.end(); row++) {
		//    for (col = row->begin(); col != row->end(); col++) {
		//        std::cout << *col << ", ";
		//    }
		//	std::cout << std::endl;
		//}
	}

	if (mWeights.empty()) {
		log::out << log::warning << "The --weights option requires a non empty csv file." << log::endl;
		return false;
	}

	return true;
}

bool epic::io::UserInputHandler::handleQuotaFromWeightfile(const std::string& fileName) {
	//detect quota from weight file
	mFloatQuota = DataInput::getQuotaFromCSV(fileName);

	if (mFloatQuota <= 0) {
		log::out << log::warning << "The --quota option requires an argument > 0." << log::endl;
		return false;
	}

	mTestFlag = true;
	return true;
}

bool epic::io::UserInputHandler::handlePrecoalitions(const std::string& sPrecoalitions) {
	if (sPrecoalitions.size() == 0) {
		mPrecoalitionFlag = true;
		return true;
	}
	std::string delimiter = ",";
	std::string delimiter2 = "+";
	std::vector<std::string> precoalitions;

	size_t last = 0;
	size_t next = 0;
	while ((next = sPrecoalitions.find(delimiter, last)) != std::string::npos) {
		precoalitions.push_back(sPrecoalitions.substr(last, next - last));

		last = next + 1;
	}
	precoalitions.push_back(sPrecoalitions.substr(last));

	for (auto it = std::begin(precoalitions); it != std::end(precoalitions); ++it) {
		std::vector<int> pushVector;
		size_t last = 0;
		size_t next = 0;
		while ((next = (*it).find(delimiter2, last)) != std::string::npos) {
			pushVector.push_back(std::stoi((*it).substr(last, next - last)) - 1);
			last = next + 1;
		}
		pushVector.push_back(std::stoi((*it).substr(last)) - 1);
		mPrecoalitions.push_back(pushVector);
	}

	return true;
}

bool epic::io::UserInputHandler::handleQuota(char* value) {
	mFloatQuota = std::atof(value);

	if (mFloatQuota > 0) {
		return true;
	} else {
		log::out << log::warning << "The --quota option requires an argument > 0." << log::endl;
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
		log::out << log::warning << "The --index option needs one of the following arguments:" << log::endl;
		index::IndexFactory::printIndexList(std::cout);
	}

	return ret;
}

bool epic::io::UserInputHandler::parseCommandLine(int numberOfArguments, char* vectorOfArguments[]) {
	enum LongOpts {
		OPT_GMP,
		OPT_PRIMES,
		OPT_FLOAT,
		OPT_CSV,
		OPT_QUIET
	};

	static struct option long_options[] = {
		{"index", required_argument, nullptr, 'i'},
		{"weights", required_argument, nullptr, 'w'},
		{"quota", required_argument, nullptr, 'q'},
		{"verbose", no_argument, nullptr, 'v'},
		{"filter-null", no_argument, nullptr, 'f'},
		{"gmp", no_argument, nullptr, OPT_GMP},
		{"primes", no_argument, nullptr, OPT_PRIMES},
		{"float", no_argument, nullptr, OPT_FLOAT},
		{"csv", no_argument, nullptr, OPT_CSV},
		{"quiet", no_argument, nullptr, OPT_QUIET},
		{"help", no_argument, nullptr, 'h'},
		{nullptr, 0, nullptr, 0}};
	int arg_count = 0;

	log::out.setLogLevel(log::warning); // default setting

	while (true) {
		int index = -1;
		//struct option * opt = 0;
		int result = getopt_long(numberOfArguments, vectorOfArguments, ":i:w:q:p:vfmh", long_options, &index);

		if (result == -1) {
			if (arg_count < 3) {
				log::out << log::warning << "Missing required options: -i | --index, -w | --weights, -q | --quota" << log::endl;
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
				//check if -p is available for precoalition indices
				if ((mIndex.compare("SCB") == 0) || (mIndex.compare("BO") == 0) || (mIndex.compare("O") == 0)) {
					std::string argument = "-p";
					if (argument.compare(vectorOfArguments[optind]) != 0) {
						std::cout << "missing argument for precoalition games: -p (which has to be before -w)" << std::endl;
						return false;
					}
				}
				arg_count++;
				break;

			case 'w':
				mWeightsFile = optarg;
				arg_count++;
				break;

			case 'q':
				if (std::stof(optarg) == 0) {
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
				if (log::out.getLogLevel() == log::error) {
					log::out << log::warning << "Incompatible options: -v/--verbose and --quiet" << log::endl;
					return false;
				}
				log::out.setLogLevel(log::info);
				break;

			case OPT_QUIET:
				if (log::out.getLogLevel() == log::info) {
					log::out << log::warning << "Incompatible options: -v/--verbose and --quiet" << log::endl;
					return false;
				}
				log::out.setLogLevel(log::error);
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

			case 'p':
				if (std::stof(optarg) == 0) {
					//No precoalitions specified -> set flag to search for it in weightfile
					mPrecoalitionFlag = true;
					arg_count++;
					break;
				} else {
					handlePrecoalitions(optarg);
					arg_count++;
					break;
				}

			case OPT_GMP:
				if (mIntRepresentation == DEFAULT) {
					mIntRepresentation = GMP;
				} else {
					log::out << log::warning << "Incompatible options: --gmp and --primes" << log::endl;
					return false;
				}
				break;

			case OPT_PRIMES:
				if (mIntRepresentation == DEFAULT) {
					mIntRepresentation = PRIMES;
				} else {
					log::out << log::warning << "Incompatible options: --gmp and --primes" << log::endl;
					return false;
				}
				break;

			case OPT_FLOAT:
				mInputFloatWeights = true;
				break;

			case OPT_CSV:
				mOutputType = csv;
				break;

			case '?':
				log::out << log::warning << "Unknown option: " << vectorOfArguments[optind - 1] << log::endl;
				return false;

			case ':':
				log::out << log::warning << "Missing arg for: " << vectorOfArguments[optind - 1] << log::endl;
				return false;

			default:
				return false;
		}
	}

	if (!handleWeightsAndQuota(mWeightsFile)) {
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

std::vector<std::vector<int>>& epic::io::UserInputHandler::getPrecoalitions() {
	return mPrecoalitions;
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

std::string epic::io::UserInputHandler::getWeightsFileName() const {
	return mWeightsFile;
}
