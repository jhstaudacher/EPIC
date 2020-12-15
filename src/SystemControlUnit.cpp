#include "SystemControlUnit.h"

#include "HardwareInfo.h"
#include "index/IndexFactory.h"
#include "types.h"
#include "Logging.h"

#include <chrono>
#include <cmath>
#include <map>
#include <vector>

//Constructor to handle application
epic::SystemControlUnit::SystemControlUnit(int numberOfInputArguments, char* vectorOfInputArguments[]) {
	mGame = nullptr;
	mUserInputHandler = new io::UserInputHandler();

	//handle User input
	handleInput(numberOfInputArguments, vectorOfInputArguments);

	//check from input or graph?
	createGamefromInputAndMinimiseWeights();

	//Estimate the time needed to calculate the index
	estimateTime();

	std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
	//calculate Index
	calculateIndex();
	std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
	log::out << log::info << "Calculation completed (" << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "µs)" << log::endl << log::endl;

	//handle output
	handleOutput();
}

//Constructor to handle application from R
epic::SystemControlUnit::SystemControlUnit(io::UserInputHandler* a_userInputHandler) {
	mGame = nullptr;
	mUserInputHandler = a_userInputHandler;

	//check from input or graph?
	createGamefromInputAndMinimiseWeights();

	//calculate Index
	calculateIndex();
}

epic::SystemControlUnit::~SystemControlUnit() {
	delete mUserInputHandler;
	delete mGame;
}

//specify and compute index
void epic::SystemControlUnit::calculateIndex() {
	index::ItfPowerIndex* index = index::IndexFactory::new_powerIndex(mUserInputHandler->getIndexToCompute(), *mGame, mUserInputHandler->getIntRepresentation());
	if (checkHardware(index)) {
		log::out << log::info << "Start computation (" << index->getFullName() << ")" << log::endl
				  << log::endl;
		mGame->setSolution(index->calculate());
	}

	index::IndexFactory::delete_powerIndex(index);
}

//create mGame from console input
void epic::SystemControlUnit::createGamefromInputAndMinimiseWeights() {
	// Preprocessing the weights to be as minimal as possible by application of gcd algorithms
	std::vector<longUInt> weights = mUserInputHandler->getWeights();

	// Push back quota, to calculate gcd with quota
	weights.push_back(mUserInputHandler->getQuota());

	longUInt m_gcd = findGCD(weights);

	// Remove quota from weights array
	weights.pop_back();

	if (m_gcd > 1) {
		log::out << log::info << "Divide weights and quota by gcd = " << m_gcd << log::endl;
		for (auto& weight : weights) {
			weight /= m_gcd;
		}
	}

	mGame = new Game(mUserInputHandler->getQuota() / m_gcd, weights, mUserInputHandler->doFilterNullPlayers());

	log::out << log::info << "Game was created:" << log::endl;
	log::out << " * quota: " << mGame->getQuota() << log::endl;
	log::out << " * weight sum: " << mGame->getWeightSum() << log::endl;
	log::out << " * player amount: " << mGame->getNumberOfPlayers() << log::endl;

	if (mUserInputHandler->doFilterNullPlayers()) {
		log::out << " * filtered null players: " << mGame->getNumberOfNullPlayers() << log::endl;
	} else {
		log::out << " * no null player filter" << log::endl;
	}
}

//handle input from console
void epic::SystemControlUnit::handleInput(int numberOfInputArguments, char* vectorOfInputArguments[]) {
	bool parsing_successful = mUserInputHandler->parseCommandLine(numberOfInputArguments, vectorOfInputArguments);

	if (!parsing_successful) {
		throw std::invalid_argument("Input could not be parsed correctly.");
	}

	log::out << log::info << "Parser was successful." << log::endl;
}

//handle output for console/csv
void epic::SystemControlUnit::handleOutput() {
	switch (mUserInputHandler->getOutputType()) {
		case io::csv:
			io::DataOutput::outputToCSV(*mGame, mUserInputHandler->getIndexToCompute(), mUserInputHandler->getWeightsFileName());
			break;
		case io::screen:
			io::DataOutput::outputToScreen(*mGame);
			break;
		default:
			throw std::invalid_argument("Output target was not specified correctly.");
	}
}

//handle output in R
std::map<std::string, std::vector<std::string>> epic::SystemControlUnit::handleRReturnValue() {
	return io::DataOutput::outputToR(*mGame, mUserInputHandler->getIndexToCompute());
}

bool epic::SystemControlUnit::checkHardware(index::ItfPowerIndex* index_ptr) {
	bool ret = true;

	HardwareInfo hInfo;
	longUInt req = index_ptr->getMemoryRequirement();

	log::out << log::info << "Read RAM capacity (" << hInfo.getTotalRamSize() << " Bytes)" << log::endl;
	log::out << log::info << "Approximated RAM usage: " << req << " Bytes" << log::endl;

	if (req > hInfo.getFreeRamSize()) {
		if (req > hInfo.getTotalRamSize()) {
			log::out << log::warning << "Not enough memory! (total: " << hInfo.getTotalRamSize() << "B, approximately needed: " << req << "B)" << log::endl;
		} else {
			log::out << log::warning << "Not enough memory! (available: " << hInfo.getFreeRamSize() << "B, approximately needed: " << req << "B!" << log::endl;
		}
		log::out << "Nonetheless the calculation may succeed with the use of swapping but will take much longer!" << log::endl;

		std::cout << "Do you want to proceed anyway? [y/n]: ";
		std::string in;
		std::cin >> in;
		while (in != "y" && in != "n") {
			std::cout << "You must chose either y (yes) or n (no)! [y/n]: ";
			std::cin >> in;
		}

		if (in == "n") {
			ret = false;
		}
	} else if (req > hInfo.getFreeRamSize() * 0.75) {
		log::out << log::warning << "High memory usage! (available: " << hInfo.getFreeRamSize() << "B, approximately needed: " << req << "B)" << log::endl;
	}

	return ret;
}

void epic::SystemControlUnit::estimateTime() {
	HardwareInfo hInfo;

	// If verbose output the system specifications to the user, also for testing ofc
	if (hInfo.getCPUFrequency() > 0) {
		log::out << log::info << "Read system specs (total cache: " << hInfo.getCacheSize() / 1000 << "KB, CPU freq: " << hInfo.getCPUFrequency() << "kHz)" << log::endl;
	} else {
		log::out << log::warning << "CPU frequency could not get determined! No time estimation available." << log::endl;
		return;
	}

	std::string estimation;
	// Consistency requirement, so the calculation doesn't fail due to a "divide by zero" error!
	if (mGame->getQuota() > 0 && mGame->getNumberOfPlayers() > 0) {
		std::string str_temp = mUserInputHandler->getIndexToCompute();

		// Time complexion classes:
		// BZ, RBZ, CP
		if (str_temp == "BZ" || str_temp == "RBZ" || str_temp == "CP") {
			// Linear formula (quota): 0.047 + 0.108*(quota/50.000)
			// Power law formula (players): 0.5 * (numberOfPlayers/50) ^ 2,28
			// Exp formula (players): 0.217 * M_E^((numberOfPlayers/50)*1.058)
			estimation = std::to_string(((0.217 * (1700000.0f / hInfo.getCPUFrequency())) * pow(M_E, 1.058 * (mGame->getNumberOfPlayers() / 50)) + (0.5 * (1700000.0f / hInfo.getCPUFrequency())) * pow((double)(mGame->getNumberOfPlayers() / 50), 2.282)) / 2 + 0.11 * (mGame->getQuota() / 50000));
			estimation += "s";
		}
		// PHA, PHT
		else if (str_temp == "PHA" || str_temp == "PHT" || str_temp == "PHTPHA") {
			// Linear formula (quota): 0.098 + 0.147*(quota/50.000)
			// Power law formula (players): 0.784*x^2.331
			// Exp formula (players): 0.317*M_E^(1.084*x)
			estimation = std::to_string(((0.317 * (1700000.0f / hInfo.getCPUFrequency())) * pow(M_E, 1.084 * (mGame->getNumberOfPlayers() / 50)) + (0.784 * (1700000.0f / hInfo.getCPUFrequency())) * pow((double)(mGame->getNumberOfPlayers() / 50), 2.331)) / 2 + 0.147 * (mGame->getQuota() / 50000));
			estimation += "s";
		}
		// PG, PGA
		else if (str_temp == "PG" || str_temp == "PGA") {
			// Linear formula (quota): 0.209 + 0.467*(quota/50.000)
			// Power law formula (players): 2.682*x^1.918
			// Exp formula (players): 1.335 * M_E^0.891*x
			estimation = std::to_string(((1.335 * (1700000.0f / hInfo.getCPUFrequency())) * pow(M_E, 0.891 * (mGame->getNumberOfPlayers() / 50)) + (2.682 * (1700000.0f / hInfo.getCPUFrequency())) * pow((double)(mGame->getNumberOfPlayers() / 50), 1.918)) / 2 + 0.209 + 0.467 * (mGame->getQuota() / 50000));
			estimation += "s";
		}
		// PHX, PIF
		else if (str_temp == "PHX" || str_temp == "PIF" || str_temp == "RPIF") {
			// Might be a bit over the top (these approximations)
			// -> Maybe take out exp formula, as power law formula has already hard growth?
			// Linear formula (quota): 1.326 + 20.074*(quota/50.000)
			// Power law formula (players): 52.601*x^2.795
			// Exp formula (players): 20.461 * M_E^1.266*x
			estimation = std::to_string(((20.0 * (1700000.0f / hInfo.getCPUFrequency())) * pow(M_E, 1.266 * (mGame->getNumberOfPlayers() / 50)) + (52.6 * (1700000.0f / hInfo.getCPUFrequency())) * pow((double)(mGame->getNumberOfPlayers() / 50), 2.795)) / 2 + 1.326 + 20.467 * (mGame->getQuota() / 50000));
			estimation += "s";
		}
		// SH
		else if (str_temp == "SH" || str_temp == "RSH") {
			// Might be a bit over the top (these approximations)
			// -> Maybe take out exp formula, as power law formula has already hard growth?
			// Linear formula (quota): 0.139 + 14.446*(quota/50.000)
			// Power law formula (players): 37.143 * x^2.761
			// Exp formula (players): 14.768*M_E^1.246*x
			estimation = std::to_string(((14.5 * (1700000.0f / hInfo.getCPUFrequency())) * pow(M_E, 1.246 * (mGame->getNumberOfPlayers() / 50)) + (37.143 * (1700000.0f / hInfo.getCPUFrequency())) * pow((double)(mGame->getNumberOfPlayers() / 50), 2.761)) / 2 + 0.139 + 14.446 * (mGame->getQuota() / 50000));
			estimation += "s";
		}
		// DP
		else if (str_temp == "DP" || str_temp == "RDP") {
			// Might be a bit over the top (these approximations)
			// -> Maybe take out exp formula, as power law formula has already hard growth?
			// Linear formula (quota): 3.0 + 30.572*(quota/50.000)
			// Power law formula (players): 39.550*x^3.539
			// Exp formula (players): 12.5 * M_E^(1.586*x)
			estimation = std::to_string(((12.5 * (1700000.0f / hInfo.getCPUFrequency())) * pow(M_E, 1.586 * (mGame->getNumberOfPlayers() / 50)) + (39.55 * (1700000.0f / hInfo.getCPUFrequency())) * pow((double)(mGame->getNumberOfPlayers() / 50), 3.539)) / 2 + 3.0 + 30.5 * (mGame->getQuota() / 50000));
			estimation += "s";
		}
		// FT
		else if (str_temp == "FT") {
			// Might be a bit over the top (these approximations)
			// -> Maybe take out exp formula, as power law formula has already hard growth?
			// Linear formula (quota): 0.0 + 23.498*(quota/50.000)
			// Power law formula (players): 71.354*x^2.2655
			// Exp formula (players): 28.541*M_E^(1.210*x)
			estimation = std::to_string(((28.541 * (1700000.0f / hInfo.getCPUFrequency())) * pow(M_E, 1.21 * (mGame->getNumberOfPlayers() / 50)) + (71.354 * (1700000.0f / hInfo.getCPUFrequency())) * pow((double)(mGame->getNumberOfPlayers() / 50), 2.2655)) / 2 + 0.0 + 23.496 * (mGame->getQuota() / 50000));
			estimation += "s";
		}
		// N, KB, PIG
		else if (str_temp == "N" || str_temp == "KB" || str_temp == "RPIG" || str_temp == "PIG" || str_temp == "PIGPHA" || str_temp == "KBPHA" || str_temp == "NPHA") {
			// Graphs fit well -> use BZ interpolation
			// Linear formula (quota): 0.096 + 0.156*(quota/50.000)
			// Power law formula (players): 0.748*x^2.331
			// Exp formula (players): 0.317*M_E^(1.084*x)
			estimation = std::to_string(((0.317 * (1700000.0f / hInfo.getCPUFrequency())) * pow(M_E, 1.084 * (mGame->getNumberOfPlayers() / 50)) + (0.748 * (1700000.0f / hInfo.getCPUFrequency())) * pow((double)(mGame->getNumberOfPlayers() / 50), 2.331)) / 2 + 0.096 + 0.156 * (mGame->getQuota() / 50000));
			estimation += "s";
		}
		// R, CI
		else if (str_temp == "R" || str_temp == "CI") {
			// Graphs fit well -> use BZ interpolation
			// Linear formula (quota): 0.043 + 0.11*(quota/50.000)
			// Power law formula (players): 0.503*x^2.278
			// Exp formula (players): 0.219*M_E^(1.057*x)
			estimation = std::to_string(((0.219 * (1700000.0f / hInfo.getCPUFrequency())) * pow(M_E, 1.057 * (mGame->getNumberOfPlayers() / 50)) + (0.503 * (1700000.0f / hInfo.getCPUFrequency())) * pow((double)(mGame->getNumberOfPlayers() / 50), 2.278)) / 2 + 0.043 + 0.11 * (mGame->getQuota() / 50000));
			estimation += "s";
		}
		// CC = Banzhaf * 0.56
		else if (str_temp == "CC") {
			// Banzhaf calculation used here, since it fits well into that growth rate of the CC, used here with a constant weight modifier
			estimation = std::to_string((((0.217 * (1700000.0f / hInfo.getCPUFrequency())) * pow(M_E, 1.058 * (mGame->getNumberOfPlayers() / 50)) + (0.5 * (1700000.0f / hInfo.getCPUFrequency())) * pow((double)(mGame->getNumberOfPlayers() / 50), 2.282)) / 2 + 0.05 + 0.11 * (mGame->getQuota() / 50000)) * 0.58);
			estimation += "s";
		} else if (str_temp == "J") {
			// Linear formula (quota): -2898.609 + 8350.0*x^1 + 694.762*x^2+48.285*x^3+-2.048*x^4
			// Power law formula (players): -> Makes no sense
			// Exp formula (players): 193135.252*M_E^(0.214*(numberOfPlayers/10))
			estimation = std::to_string((193135.252 * (1700000.0f / hInfo.getCPUFrequency())) * pow(M_E, 0.214 * (mGame->getNumberOfPlayers() / 10)) + (-2898.609 + 8350.0 * (mGame->getQuota() / 50000) + 694.762 * pow((mGame->getQuota() / 50000), 2) + 48.285 * pow((mGame->getQuota() / 50000), 3) + -2.048 * pow((mGame->getQuota() / 50000), 4)) * (1700000.0f / hInfo.getCPUFrequency()));
			estimation += "s";
		} else {
			estimation = "trivial or not implemented";
		}

		log::out << log::info << "Calculation time estimation: " << estimation <<  " (only accurate without swap-usage)" << log::endl;
	}
}

epic::longUInt epic::SystemControlUnit::gcd(longUInt a, longUInt b) {
	if (b == 0)
		return a;
	return gcd(b, a % b);
}

epic::longUInt epic::SystemControlUnit::findGCD(std::vector<longUInt>& vector) {
	longUInt result = vector[0];
	for (longUInt i = 1; i < vector.size(); i++) {
		result = gcd(vector[i], result);

		if (result == 1) {
			return 1;
		}
	}
	return result;
}
