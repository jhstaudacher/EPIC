#ifndef EPIC_IO_USERINPUTHANDLER_H_
#define EPIC_IO_USERINPUTHANDLER_H_

#include "FloatToIntRepresentation.h"
#include "types.h"

#include <map>
#include <string>
#include <vector>

namespace epic::io {

typedef enum OutputType { screen, /**< prints output to the console */
						  csv,	  /**< writes output to a .csv file */
						  R		  /**< passes the output to R */
} OutputType;

static const std::string MSG_REQUIRED_OPTS = "--index | -i <index abbreviation> : Specifies the power index to compute.\n"
											 "--weights | -w <csv filename> : CSV-file containing the weights for the players.\n"
											 "--quota | -q <integer> : The quota of the mGame.\n";

static const std::string MSG_OPTIONAL_OPTS = "--filter-null | -f : Flag to filter out null players.\n"
											 "--verbose | -v : Flag to provide additional details of the computation.\n"
											 "--gmp : Flag to use the GMP library for large integer representation. Setting this flag will make the memory prediction unreliable due to dynamic memory allocations!\n"
											 "--primes : Flag to use the Chinese Remainder Theorem for large integer representation\n"
											 "--float : Flag to input float values from csv. Setting this flag will find an integer representation of a given floating-point problem, which is in no way minimal or optimal\n"
											 "--csv : Flag to output the solution in a csv file.\n";

/**
 * A class that handles the input from the user (via console) and from the R interface
 *
 */

class UserInputHandler {
private:
	/**
    * A vector of weights for the individual players
    */
	std::vector<longUInt> mWeights;

	/**
    * The Index to compute 
    */
	std::string mIndex;
	/**
    * The quota of the mGame
    */
	longUInt mQuota;
    /**
    * The quota of the mGame as float
    */
	float mFloatQuota;    
	/**
    * The specified output type/channel
    */
	OutputType mOutputType;

	/**
    * Internal flag used for tests
    */
	bool mTestFlag;

	/**
    * Internal flag used for the input of float weights
    */
	bool mInputFloatWeights;

	/**
    * Flag to filter out null players
    */
	bool mFilterNullPlayers;

	/**
    * Flag to provide additional details of the computation
    */
	bool mVerbose;

	/**
    * Setting which large integer representation should be used.
    *
    * @remark If the problem is small enough that a single uint64_t is enough, a single variable gets used (neither GMP nor PRIMES) no matter what option is set!
    */
	IntRepresentation mIntRepresentation;

	/**
    * Name of the file containing the weights
    */
	std::string mWeightsFile;

	/**
    * Method used to validate the entered output type/channel
    */
	bool handleOutputType(char* value);

	/**
    * Method used to validate the entered weight file and weights
    */
	bool handleWeightsAndQuota(const std::string& fileName);

	/**
    * Method used to validate the quota specified in the weights file
    * This Method also sets the mTestFlag to true if the previous validation succeeds
    */
	bool handleQuotaFromWeightfile(const std::string& fileName);

	/**
    * Method used to validate the entered quota
    */
	bool handleQuota(char* value);

	/**
    * Method used to validate the name of the entered power index
    */
	bool handleIndex(char* value);

public:
	/**
     * Constructor A: For the R interface where the input is given as arguments
     *
     * @param index the name of the power index
     * @param weights vector of weights
     * @param quota the quota of the mGame
     * @param outputType the output type as depicted in enum OutputType
     * @param filterNullPlayers flag to filter out null players
     * @param verbose flag to provide additional details of the computation
     */
	UserInputHandler(const std::string& index, const std::vector<longUInt>& weights, longUInt quota, OutputType outputType, bool filterNullPlayers, bool verbose);

	/**
     * Constructor B: For the input from console
     */
	UserInputHandler();

	/**
     * Method to parse the user input from the console
     *
     * @param numberOfArguments the number of input arguments
     * @param vectorOfArguments a vector that includes each command of input from the user
     */
	bool parseCommandLine(int numberOfArguments, char* vectorOfArguments[]);

	/**
     * Method to return the vector of weights
     *
     * --weights \<filename.csv\>
     */
	std::vector<longUInt>& getWeights();

	/**
     * Method to return the quota
     *
     * --quota \<longUInt\>
     */
	longUInt getQuota() const;

	/**
     * Method to return the abbreviation of the index
     *
     * --index \<index abbr.\>
     */
	std::string getIndexToCompute() const;

	/**
     * Method to return the output type/channel
     *
     * --output <screen|csv>
     */
	OutputType getOutputType() const;

	/**
     * Method to return the flag for filtering null players
     *
     * --filter-null
     */
	bool doFilterNullPlayers() const;

	/**
     * Method to return integer representation to use
     *
     * --gmp & --primes
     */
	IntRepresentation getIntRepresentation() const;

	/**
     * Method to return the flag for the verbosity
     *
     * --verbose
     */
	bool isVerbose() const;

	/**
     * Method to return the name of the weights file
     */
	std::string getWeightsFileName() const;

}; /* class UserInputHandler */

} /* namespace epic::io */

#endif /* EPIC_IO_USERINPUTHANDLER_H_ */
