#include "io/DataInput.h"

#include <algorithm>
#include <fstream>
#include <iostream>

std::vector<epic::longUInt> epic::io::DataInput::inputCSV(const std::string& filePath, bool testFlag) {
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

	for (longUInt line_no = 1; fstream >> tmp_str; ++line_no) {
		try {
			tmp_long = std::stoull(tmp_str);
		} catch (std::exception& ex) {
			throw std::invalid_argument("Unknown character(s) in input file at line: " + std::to_string(line_no) + " (\"" + tmp_str + "\")");
		}

		weights.push_back(tmp_long);
	}

	return weights;
}

std::vector<float> epic::io::DataInput::inputFloatCSV(const std::string& filePath, bool testFlag) {
	std::vector<float> weights;

	std::ifstream fstream(filePath);
	if (!fstream) {
		throw std::invalid_argument("Could not open input file");
	}

	std::string tmp_str;
	float tmp_float;

	// if testFlag is set, the first line contains the quota. Therefore skip the first line.
	if (testFlag) {
		fstream >> tmp_str;
	}

	for (longUInt line_no = 1; fstream >> tmp_str; ++line_no) {
		try {
			tmp_float = std::stof(tmp_str);
		} catch (std::exception& ex) {
			throw std::invalid_argument("Unknown character(s) in input file at line: " + std::to_string(line_no) + " (\"" + tmp_str + "\")");
		}

		weights.push_back(tmp_float);
	}

	return weights;
}

float epic::io::DataInput::getQuotaFromCSV(const std::string& filePath) {
	float floatQuota;

	std::ifstream fstream(filePath);
	if (!fstream) {
		throw std::invalid_argument("Could not open input file");
	}

	std::string tmp_str;
	fstream >> tmp_str;

	try {
		floatQuota = std::stof(tmp_str);
	} catch (std::exception& ex) {
		throw std::invalid_argument("Unknown character(s) in input file at first line (\"" + tmp_str + "\")");
	}

	return floatQuota;
}
