#ifndef EPIC_IO_DATAOUTPUT_H_
#define EPIC_IO_DATAOUTPUT_H_

#include "Game.h"

#include <map>
#include <string>
#include <vector>

namespace epic::io {

class DataOutput {
public:
	DataOutput() = delete;

	/**
	 * Write the solution (plus a view extra information) of a given mGame to a .csv file. The output file will have the name: <index_name>_computation_results_<timestamp>.csv
	 *
	 * @param game An instance of Game already containing a solution
	 * @param index_name The name of the calculated index (gets written to the file)
	 * @param input_file The name of the .csv file containing the games weights (gets written to the file)
	 */
	static void outputToCSV(Game& game, const std::string& index_name, const std::string& input_file);

	/**
	 * Write the solution of a given mGame to the console (screen).
	 *
	 * @param game An instance of Game already containing a solution
	 */
	static void outputToScreen(Game& game);

	/**
	 * Create a map containing the solution of a mGame that can then get passed to R.
	 *
	 * @param game An instance of Game already containing a solution
	 * @param index The name of the calculated index
	 * @return A map object with one entry: {<index>, <solution vector>}
	 */
	static std::vector<std::string> outputToR(Game& game, const std::string& index);

}; /* class DataInput */

} /* namespace epic::io */

#endif /* EPIC_IO_DATAOUTPUT_H_ */
