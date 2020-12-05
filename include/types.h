#ifndef EPIC_TYPES_H_
#define EPIC_TYPES_H_

#include <cinttypes>
#include <gmpxx.h>

namespace epic {

typedef mpz_class bigInt;
typedef mpf_class bigFloat;

#if defined(__linux__)
typedef std::uint64_t longUInt;
typedef std::int64_t longInt;
#else
typedef unsigned int longUInt;
typedef int longInt;
#endif

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
