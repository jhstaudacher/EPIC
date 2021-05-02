#include "HardwareInfo.h"

#if defined(__linux__)
/*
 * LINUX
 */

#include <fstream>		 // read system pseudo-files
#include <sys/sysinfo.h> // sysinfo
#include <unistd.h>		 // sysconf

epic::HardwareInfo::HardwareInfo() {
	{ // RAM size
		struct sysinfo info;
		if (sysinfo(&info) == 0) {
			// success
			mRamSizeTotal = info.totalram / cMemUnit_factor;
			mRamSizeFree = info.freeram / cMemUnit_factor;
		} else {
			// error
			mRamSizeTotal = 0;
			mRamSizeFree = 0;
		}
	}

	{ // CACHE size
		const int cache_level[] = {
			_SC_LEVEL1_DCACHE_SIZE,
			_SC_LEVEL2_CACHE_SIZE,
			_SC_LEVEL3_CACHE_SIZE,
			_SC_LEVEL4_CACHE_SIZE};

		mCacheSize = 0;
		for (int i : cache_level) {
			mCacheSize += sysconf(i);
		}
	}

	{ // CPU frequency
		const std::string file_name = "/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_max_freq";
		std::ifstream f;
		f.open(file_name);

		if (f.is_open()) {
			f >> mCpuFrequency;
			f.close();
		} else {
			mCpuFrequency = 0;
		}
	}
}

#elif defined(__WIN32__) || defined(__CYGWIN__)
/*
 * WINDOWS
 */

#include <iostream>
#include <windows.h>

epic::HardwareInfo::HardwareInfo() {
	{ // RAM size
		MEMORYSTATUSEX state = {sizeof(state)};

		if (GlobalMemoryStatusEx(&state)) {
			mRamSizeTotal = state.ullTotalPhys / cMemUnit_factor;
			mRamSizeFree = state.ullAvailPhys / cMemUnit_factor;
		} else {
			mRamSizeTotal = 0;
			mRamSizeFree = 0;
		}
	}

	{ // CACHE size
		DWORD bufferLength = 0;
		DWORD i = 0;
		SYSTEM_LOGICAL_PROCESSOR_INFORMATION* buffer = 0;

		GetLogicalProcessorInformation(0, &bufferLength);
		buffer = (SYSTEM_LOGICAL_PROCESSOR_INFORMATION*)malloc(bufferLength);

		if (GetLogicalProcessorInformation(&buffer[0], &bufferLength)) {
			for (i = 0; i != bufferLength / sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION); ++i) {
				if (buffer[i].Relationship == RelationCache) {
					mCacheSize += buffer[i].Cache.Size;
				}
			}
		} else {
			mCacheSize = 0;
		}

		free(buffer);
	}

	{ // CPU frequency
		DWORD bufferSize = MAX_PATH;
		DWORD mhz = MAX_PATH;
		HKEY hKey;

		long error = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
								   "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0",
								   0,
								   KEY_READ,
								   &hKey);

		if (error == ERROR_SUCCESS) {
			RegQueryValueEx(hKey, "~MHz", NULL, NULL, (LPBYTE)&mhz, &bufferSize);

			mCpuFrequency = mhz * 1000;
		} else {
			mCpuFrequency = 0;
		}
	}
}

#else
/*
 * UNSUPPORTED OS
 */

epic::HardwareInfo::HardwareInfo() {
	mRamSizeTotal = 0;
	mRamSizeFree = 0;
	mCacheSize = 0;
	mCpuFrequency = 0;
}

#endif

epic::longUInt epic::HardwareInfo::getTotalRamSize() const {
	return mRamSizeTotal;
}

epic::longUInt epic::HardwareInfo::getFreeRamSize() const {
	return mRamSizeFree;
}

epic::longUInt epic::HardwareInfo::getCacheSize() const {
	return mCacheSize;
}

epic::longUInt epic::HardwareInfo::getCPUFrequency() const {
	return mCpuFrequency;
}
