#include "Logging.h"
#include "SystemControlUnit.h"

int main(int argc, char* argv[]) {
	try {
		epic::SystemControlUnit control(argc, argv);
	} catch (std::invalid_argument& e) {
		epic::log::out << epic::log::error << e.what() << epic::log::endl;
		return -1;
	}

	return 0;
}
