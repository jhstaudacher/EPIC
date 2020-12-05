#ifndef EPIC_SYSTEMCONTROLUNIT_H_
#define EPIC_SYSTEMCONTROLUNIT_H_

#include "Game.h"
#include "index/ItfPowerIndex.h"
#include "io/DataOutput.h"
#include "io/UserInputHandler.h"

#include <iostream>

namespace epic {

/**
   * A class that takes care of the correct allocation of tasks
   *
   *
   */
class SystemControlUnit {
public:
	/**
       * Constructor A: For input from console
       *
       * @param numberOfInputArguments the number of input arguments
       * @param vectorOfInputArguments a vector that includes each command of input form user
       *
       */
	SystemControlUnit(int numberOfInputArguments, char* vectorOfInputArguments[]);

	/**
       * Constructor B: For input from R
       *
       * @param a_userInputHandler object of class UserInputHandler to handle input from R
       *
       */
	explicit SystemControlUnit(io::UserInputHandler* a_userInputHandler);

	~SystemControlUnit();

	/**
       * A function that determines the selected index depending on the user input and starts the calculation of this index. After this set the solution of the computation.
       *
       */
	void calculateIndex();

	/**
       * A function to create a mGame from console or R input and trying to minimise the weights with a simple gcd-algorithm.
       *
       */
	void createGamefromInputAndMinimiseWeights();

	/**
       * A function that handle the entered commands for prepare the input. Use an instance of UserInputHandler.
       *
       * @param numberOfInputArguments the number of input arguments
       * @param vectorOfInputArguments a vector that includes each command of input form user
       *
       */
	void handleInput(int numberOfInputArguments, char* vectorOfInputArguments[]);

	/**
       * A function that handles the entered commands to prepare output. Passes the result of the index calculation to an instance of DataOutput.
       *
       */
	void handleOutput();

	/**
       * A function that handle the output in R. Passes the result of the index calculation to an instance of DataOutput.
       *
       */
	std::map<std::string, std::vector<std::string>> handleRReturnValue();

	/**
       * Checks if the current computer hardware fits the requirements to compute the given index
       *
       * This method only produced user output (e.g. warnings). Even if the hardware does not meet the requirements this method will not throw an exception. Doing so enables the user to try the calculation. If the memory requirement is larger than the available one, the user gets asked whether to continue or not.
       *
       * @param index_ptr PowerIndex defining the requirements
       * @return If the calculation should be done or not (user decision)
       */
	bool checkHardware(index::ItfPowerIndex* index_ptr);

	/**
       * A function that will estimate the time needed for the computation and outputting it to the screen.
       *
       * @return Number of seconds the calculation will take
       */

	void estimateTime();

private:
	/**
       * An instance of class mGame
       */
	Game* mGame;

	/**
       * An instance of class UserInputHandler
       */
	io::UserInputHandler* mUserInputHandler;
	/**
	    * A method to find the gcd of an integer vector
	    */
	longUInt findGCD(std::vector<longUInt>& vector);
	/**
		* Base method to find the gcd of integer numbers
		*/
	longUInt gcd(longUInt a, longUInt b);

}; /* class SystemControlUnit */

} /* namespace epic */

#endif /* EPIC_SYSTEMCONTROLUNIT_H_ */
