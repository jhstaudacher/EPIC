#ifndef EPIC_TYPES_H_
#define EPIC_TYPES_H_

#include <cinttypes>
#include <gmpxx.h>
#include <sstream>

namespace epic {

typedef mpz_class bigInt;
typedef mpf_class bigFloat;

// Larger integers are better. However the current GMP version can not handle long long integers.
typedef unsigned long longUInt;
typedef long longInt;


/**
 * The calculation factor between the base unit (Bytes) and cMemUnit_name!
 * x [Byte] / cMemUnit_factor = y [cMemUnit_name]
 */
static const longUInt cMemUnit_factor = 1024;

/**
 * The shortcut for the used memory unit.
 */
static const std::string cMemUnit_name = "KiB";


/**
 * Enum-Type defining the different options for large integer representations
 */
enum IntRepresentation {
	GMP,	/**< Using the GMP-library */
	PRIMES, /**< Using the Chinese Remainder Theorem */
	DEFAULT /**< for number of primes p <= 4 PRIMES, for p > 4 GMP */
};

static constexpr longUInt c_sizeof_longUInt = sizeof(longUInt);
static constexpr longUInt c_sizeof_double = sizeof(double);

/**
 * A helper class containing some functionality often needed for the GMP type.
 */
class GMPHelper {
public:
	GMPHelper() = delete;

	/**
	 * Calculating the size of a bigInt variable including all dynamic allocated memory
	 *
	 * @param value The bigInt object to calculate the size for.
	 * @return The calculated size in Bytes
	 */
	static longUInt size_of_int(const bigInt& value) {
		return (mpz_size(value.get_mpz_t()) * mLimbSize) + sizeof(bigInt);
	}

	/**
	 * Calculating the size of a bigFloat variable including all dynamic allocated memory
	 *
	 * @param value The bigFloat object to calculate the size for.
	 * @return The calculated size in Bytes
	 */
	static longUInt size_of_float(const bigFloat& value) {
		return (mpf_size(value.get_mpf_t()) * mLimbSize) + sizeof(bigFloat);
	}

	/**
	 * Converting a bigFloat variable to std::string
	 *
	 * @param value The bigFloat object to convert
	 * @return The std::string of the bigFloat object
	 */
	static std::string mpf_class_to_string(mpf_class& value) {
		double dValue = value.get_d();
		std::ostringstream stringstream;
		stringstream << dValue;

		std::string str = stringstream.str();

		return str;
	}

private:
	static constexpr longUInt mLimbSize = GMP_LIMB_BITS / 8;
};

/**
 * The index namespace contains all power indices.
 */
namespace index {}

/**
 * The io namespace contains everything need for user interaction.
 */
namespace io {}

/**
 * The lint namespace contains all classes needed for large integer representations.
 */
namespace lint {}

} /* namespace epic */

#endif /* EPIC_TYPES_H_ */
