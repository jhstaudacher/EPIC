#ifndef EPIC_IO_DATAINPUT_H_
#define EPIC_IO_DATAINPUT_H_

#include "types.h"

#include <string>
#include <vector>

namespace epic::io {

class DataInput {
public:
	DataInput() = delete;

	/**
	 * Read values line by line (one value per line) as integers from a given .csv file.
	 *
	 * @param filePath The .csv file name (including the path to it)
	 * @param testFlag If set the first line gets skipped (contains the quota)
	 * @return A vector of longUInt integer values read from the .csv file
	 */
	static std::vector<longUInt> inputCSV(const std::string& filePath, std::vector<std::vector<int>>& precoalitions, bool testFlag, bool pFlag);

	/**
	 * Read values line by line (one value per line) as floats from a given .csv file.
	 *
	 * @param filePath The .csv file name (including the path to it)
	 * @param testFlag If set the first line gets skipped (conatins the quota)
	 * @return A vector of float values read from the .csv file
	 */
	static std::vector<double> inputFloatCSV(const std::string& filePath, std::vector<std::vector<int>>& precoalitions, bool testFlag, bool pFlag);

	/**
	 * Read the quota from a given .csv file
	 *
	 * @param filePath The .csv file name (including the path to it)
	 * @return The quota read from the .csv file (as float)
	 */
	static double getQuotaFromCSV(const std::string& filePath);

}; /* class DataInput */

} /* namespace epic::io */

#endif /* EPIC_IO_DATAINPUT_H_ */
