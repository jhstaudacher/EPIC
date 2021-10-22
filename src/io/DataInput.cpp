#include "io/DataInput.h"

#include <algorithm>
#include <fstream>
#include <iostream>

std::vector<epic::longUInt> epic::io::DataInput::inputCSV(const std::string& filePath, std::vector<std::vector<int>>& precoalitions, bool testFlag, bool pFlag) {
	std::vector<longUInt> weights;

	std::ifstream fstream(filePath);
	if (!fstream) {
		throw std::invalid_argument("Could not open input file");
	}

	std::string tmp_str;
	longUInt tmp_long;

	// if testFlag is set, the first line contains the quota. Therefore skip the first line.
	if (testFlag) {
		fstream >> tmp_str;
	}

	//no precoalitions
	if (!pFlag){
		for (longUInt line_no = 1; fstream >> tmp_str; ++line_no) {
			try {
				tmp_long = std::stoull(tmp_str);
			} catch (std::exception& ex) {
				throw std::invalid_argument("Unknown character(s) in input file at line: " + std::to_string(line_no) + " (\"" + tmp_str + "\")");
			}

			weights.push_back(tmp_long);
		}
	}
	//with precoalitions
	else {
		int playerNr = 0;
		std::vector <int> appendablePrecoalition;
		for (longUInt line_no = 1; fstream >> tmp_str; ++line_no) {			
			//cut multiple numbers seperated with "," into single numbers
			std::string delimiter = ",";
			size_t last = 0; 
			size_t next = 0;
			while ((next = tmp_str.find(delimiter, last)) != std::string::npos){				
				//turn string to number
				try {
					tmp_long = std::stoull(tmp_str.substr(last, next-last));
				} catch (std::exception& ex) {
					throw std::invalid_argument("Unknown character(s) in input file at line: " + std::to_string(line_no) + " (\"" + tmp_str.substr(last, next-last) + "\")");
				}
				
				weights.push_back(tmp_long);
				appendablePrecoalition.push_back(playerNr);

				last = next + 1; 
				playerNr++;
			} 
			//turn string to number
			try {
				tmp_long = std::stoull(tmp_str.substr(last));
			} catch (std::exception& ex) {
				throw std::invalid_argument("Unknown character(s) in input file at line: " + std::to_string(line_no) + " (\"" + tmp_str.substr(last) + "\")");
			}
			
			weights.push_back(tmp_long);
			appendablePrecoalition.push_back(playerNr);
			precoalitions.push_back(appendablePrecoalition);
			appendablePrecoalition.clear();
			playerNr++;
		}
	}
	return weights;
}

std::vector<double> epic::io::DataInput::inputFloatCSV(const std::string& filePath, std::vector<std::vector<int>>& precoalitions, bool testFlag, bool pFlag) {
	std::vector<double> weights;

	std::ifstream fstream(filePath);
	if (!fstream) {
		throw std::invalid_argument("Could not open input file");
	}

	std::string tmp_str;
	double tmp_float;

	// if testFlag is set, the first line contains the quota. Therefore skip the first line.
	if (testFlag) {
		fstream >> tmp_str;
	}

	//no precoalitions
	if (!pFlag){
		for (longUInt line_no = 1; fstream >> tmp_str; ++line_no) {
			try {
				tmp_float = std::atof(tmp_str.c_str());
			} catch (std::exception& ex) {
				throw std::invalid_argument("Unknown character(s) in input file at line: " + std::to_string(line_no) + " (\"" + tmp_str + "\")");
			}

			weights.push_back(tmp_float);
		}
	}
	//with precoalitions
	else {
		int playerNr = 0;
		std::vector <int> appendablePrecoalition;
		for (longUInt line_no = 1; fstream >> tmp_str; ++line_no) {
			//cut multiple numbers seperated with "," into single numbers
			std::string delimiter = ",";
			size_t last = 0; 
			size_t next = 0;
			while ((next = tmp_str.find(delimiter, last)) != std::string::npos){   				
				//turn string to number
				try {
					tmp_float = std::atof(tmp_str.substr(last, next-last).c_str());
				} catch (std::exception& ex) {
					throw std::invalid_argument("Unknown character(s) in input file at line: " + std::to_string(line_no) + " (\"" + tmp_str.substr(last, next-last) + "\")");
				}
				
				weights.push_back(tmp_float);
				appendablePrecoalition.push_back(playerNr);

				last = next + 1; 
				playerNr++;
			} 
			//turn string to number
			try {
				tmp_float = std::atof(tmp_str.substr(last).c_str());
			} catch (std::exception& ex) {
				throw std::invalid_argument("Unknown character(s) in input file at line: " + std::to_string(line_no) + " (\"" + tmp_str.substr(last) + "\")");
			}
			
			weights.push_back(tmp_float);
			appendablePrecoalition.push_back(playerNr);
			precoalitions.push_back(appendablePrecoalition);
			appendablePrecoalition.clear();
			playerNr++;
		}
	}
	return weights;
}

double epic::io::DataInput::getQuotaFromCSV(const std::string& filePath) {
	double floatQuota;

	std::ifstream fstream(filePath);
	if (!fstream) {
		throw std::invalid_argument("Could not open input file");
	}

	std::string tmp_str;
	fstream >> tmp_str;

	try {
		floatQuota = std::atof(tmp_str.c_str());
	} catch (std::exception& ex) {
		throw std::invalid_argument("Unknown character(s) in input file at first line (\"" + tmp_str + "\")");
	}

	return floatQuota;
}
