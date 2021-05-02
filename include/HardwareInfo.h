#ifndef EPIC_HARDWAREINFO_H_
#define EPIC_HARDWAREINFO_H_

#include "types.h"

namespace epic {

class HardwareInfo {
public:
	/**
	 * Creates a HardwareInfo instance. Inside the constructor the values get queried from the system and internally stored. Those values can get accessed using the getter-methods.
	 */
	HardwareInfo();

	/**
	 * @return The total size of RAM in <cMemUnit_name>. If the value is zero either the OS gets not supported or an error occurred at construction time while reading the value from the system.
	 */
	longUInt getTotalRamSize() const;

	/**
	 * @return The free size of RAM in <cMemUnit_name>. If the value is zero either the OS gets not supported or an error occurred at construction time while reading the value from the system.
	 */
	longUInt getFreeRamSize() const;

	/**
	 * @return The sum of all CPU-Caches (L1D, L2, L3, L4) in Bytes. If the value is zero either the OS gets not supported or an error occurred at construction time while reading the value from the system.
	 */
	longUInt getCacheSize() const;

	/**
	 * @return The maximum CPU frequency in kHz. If the value is zero either the OS gets not supported or an error occurred at construction time while reading the value from the system.
	 */
	longUInt getCPUFrequency() const;

private:
	longUInt mRamSizeTotal;
	longUInt mRamSizeFree;
	longUInt mCacheSize;
	longUInt mCpuFrequency;
};

} /* namespace epic */

#endif /* EPIC_HARDWAREINFO_H_ */
