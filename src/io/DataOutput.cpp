#include "io/DataOutput.h"

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>

void epic::io::DataOutput::outputToCSV(Game& game, const std::string& index_name, const std::string& input_file) {
	std::vector<bigFloat> results = game.getSolution();

	std::stringstream filename;
	std::time_t timestamp = std::time(nullptr);
	filename << index_name << "_computation_results_" << timestamp << ".csv";
	std::ofstream file(filename.str());

	file << "Player count: " << game.getNumberOfPlayers() << std::endl;
	file << "Quota: " << game.getQuota() << std::endl;
	file << "Weights file: " << input_file << std::endl;

	file << results[0];

	for (size_t i = 1; i < results.size(); ++i) {
		file << "," << results[i];
	}
}

void epic::io::DataOutput::outputToScreen(Game& game) {
	std::vector<bigFloat> results = game.getSolution();

	if (results.size() == 1) { // single value result
		std::cout << results[0] << std::endl;
	} else {
		for (size_t i = 0; i < results.size(); ++i) {
			std::cout << "P" << i + 1 << ": " << results[i] << std::endl;
		}
	}
}

std::map<std::string, std::vector<std::string>> epic::io::DataOutput::outputToR(Game& game, const std::string& index) {
	std::vector<bigFloat> results = game.getSolution();
	std::map<std::string, std::vector<std::string>> formattedResults;
	std::vector<std::string> r_results;

	for (auto& result : results) {
		std::ostringstream stringstream;
		stringstream << result;

		r_results.push_back(stringstream.str());
	}

	formattedResults[index] = r_results;

	return formattedResults;
}
