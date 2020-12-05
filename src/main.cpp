#include "SystemControlUnit.h"

#include <iostream>

int main(int argc, char* argv[]) {
	try {
		epic::SystemControlUnit control(argc, argv);
	} catch (std::invalid_argument& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return -1;
	}

	return 0;
}
