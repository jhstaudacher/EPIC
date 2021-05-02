#ifndef EPIC_INDEX_ITFPOWERINDEX_H_
#define EPIC_INDEX_ITFPOWERINDEX_H_

#include "Game.h"
#include "lint/ItfLargeNumberCalculator.h"

namespace epic::index {

class ItfPowerIndex {
public:
	explicit ItfPowerIndex(Game& g)
		: mGame(g) {
		mCalculator = nullptr;
		mNonZeroPlayerCount = mGame.getNumberOfPlayers() - mGame.getNumberOfPlayersWithWeight0();
	};
	virtual ~ItfPowerIndex() = default;

	/**
	 * Calculating the power index for each player.
	 */
	virtual std::vector<bigFloat> calculate() = 0;

	/**
	 * A function to get the full name of the index.
	 *
	 */
	virtual std::string getFullName() = 0;

	/**
	 * Getting an approximated size of memory (RAM) needed for computation
	 *
	 * @return The approximated RAM size in <cMemUnit_name>. If 0 is returned the approximated memory is larger than 2^64 Bytes (does not fit in to a longUInt variable).
	 */
	virtual longUInt getMemoryRequirement() = 0;

protected:
	Game& mGame;

	/**
	 * A specialized Calculator object for operating with LargeNumber objects
	 */
	lint::ItfLargeNumberCalculator* mCalculator;

	/**
	 * The number of players in the mGame with weight > zero
	 */
	longUInt mNonZeroPlayerCount;

}; /* ItfPowerIndex */

/**
 * # How to implement a ItfPowerIndex interface
 * @example new_index_implementation
 *
 * To add a new index calculation the following two steps must be done:
 * 	1. implement a new power index class (implementing the ItfPowerIndex interface)
 * 	2. integrate the new class
 *
 * ## Implement new index class
 * After thought of a name for the new index implementation (we use "NewIndex" for this tutorial) the new files include/index/NewIndex.h and src/index/NewIndex.cpp must be created.
 * For the implementation of the new class there are two options. The first option "independent implementation" is to create all functionality needed (independent from other power index implementations) from the scratch up while the second option "derived implementation" is to use already implemented functionality from the existing indices.
 *
 * ### independent implementation
 * For creating a completely new power index implementation the `include/index/ItfPowerIndex.h` interface must get implemented (i.e. inherit from ItfPowerIndex and implement the pure virtual methods). For a description what the pure virtual methods are supposed to do, see the comments in the `include/index/ItfPowerIndex.h` file.
 *
 * What must be done at least:
 *	- **Constructor**: initialize `mCalculator` (if large integer calculations are needed)
 *	- **Destructor**: delete `mCalculator` (if initialized in the constructor)
 *	- **calculate()**: do your calculations (you may need `mGame` and `mCalculator`) and return the index values for each player as a vector
 *	- **getFullName()**: return the full/complete name of your index
 *	- **getMemoryRequirement()**: return an approximation (upper bound) for the amount of memory needed during the `calculation()` method
 *
 * An already existing example for a independent implementation is provided by the RawPublicGood implementation (`include/index/RawPublicGood.h`, src/index/RawPublicGood.cpp`).
 *
 * The following files `NewIndex.h` and `NewIndex.cpp` showing an example as an empty class frame. For a working example you can checkout the git patch file `examples/new_index/independent/newindex_independent.patch`.
 *
 * **examples/new_index/independent/NewIndex.h**:
 * @include{lineno} new_index/independent/NewIndex.h
 *
 * **examples/new_index/independent/NewIndex.cpp**:
 * @include{lineno} new_index/independent/NewIndex.cpp
 *
 * ### derived implementation
 * By deriving from an already existing power index implementation the `include/index/ItfPowerIndex.h` interface gets indirectly implemented. In this case one must not care about the `mCalculator` object. For the other methods (especially `calculate()`) the functionality provided by the base class can be used.
 *
 * **NOTE**: Since the `mCalculator` object gets initialized by the base class, the calculations must be in the limits of this object (i.e. calculation maximum and allowed operations).
 *
 * **NOTE**: There are classes deriving from the `include/index/ItfPowerIndex.h` interface but not initializing the mCalculator object. Those classes (e.g. `PowerIndexWithWinningCoalitions`) providing extra methods but do not implement the pure virtual methods. Using a class like this can be done by a "independent implementaiton" deriving from such a class instead from the `ItfPowerIndex` interface directly.
 *
 * An already existing example for a derived implementation is provided by the PublicGood implementation (`include/index/PublicGood.h`, `src/index/PublicGood.cpp`).
 *
 * The following files `NewIndex.h` and `NewIndex.cpp` showing an example as an empty class frame. For a working example you can checkout the git patch file `examples/new_index/derived/newindex_derived.patch`.
 *
 * **examples/new_index/derived/NewIndex.h**:
 * @include{lineno} new_index/derived/NewIndex.h
 *
 * **examples/new_index/derived/NewIndex.cpp**:
 * @include{lineno} new_index/derived/NewIndex.cpp
 *
 *
 * ## Integrate new index
 *
 * To be able to access to new power index implementation through the CLI it must be integrated by following a few simple steps:
 * - `include/index/IndexFactory.h`: Add an entry for the new index into the `enum IndexType {...}`
 * @code
 * 	enum IndexType {
 * 		INVALID_INDEX,
 * 		NI, // <- enter a line like this (with an abbreviation of your index name)
 * 		ABZ,
 * 		// ... there are already a few entries
 * 	}
 * @endcode
 *
 * - `src/index/IndexFactory.cpp`: Include the header of your new class (e.g. `#include "index/NewIndex.h"`)
 * - `src/index/IndexFactory.cpp`: Add your index to the map of index names `cIndexNames`
 * @code
 * 	const std::map<epic::index::IndexFactory::IndexType, std::pair<std::string, std::string>> epic::index::IndexFactory::cIndexNames = {
 * 		{INVALID_INDEX, {"INVALID", "invalid index"}},
 * 		{NI, {"NI", "new index"}}, // <-- enter a line like this (make sure that the first `NI` and the second entry `"NI"` match)
 * 		{ABZ, {"ABZ", "absolute Banzhaf"}},
 * 		// ... there are already a few entries
 * 	}
 * @endcode
 *
 * - `src/index/IndexFactory.cpp`: Add your index to the `new_powerIndex()` method
 * @code
 * 	switch (getIndexType(short_name)) {
 * 		case IndexType::INVALID_INDEX: index = nullptr, break;
 * 		case IndexType::NI: index = NewIndex(g, approx, int_representation); break; // <-- enter a line like this
 * 		case IndexType::ABZ: index = AbsoluteBanzhaf(g, approx, int_representation); break;
 * 		// ... there are already a few cases
 * 	}
 * @endcode
 *
 * - `readme.md`: Add your new index to the readme for a consistent documentation
 * @code
 * 	| shortcut | name |
 * 	| -------- | ---- |
 * 	| `NI` | New Index |
 * 	| `ABZ` | Absolute Banzhaf |
 * 	...
 * @endcode
 *
 *
 * ## complete examples
 * If you are familiar with git patches, you can use one of the following files providing empty (but working) class implementations to get started (`git apply [-3] <patch-file>`):
 *	- independent implementation: `examples/new_index/independent/newindex_independent.patch`
 *	- derived implementation: `examples/new_index/derived/newindex_derived.patch`
 */

} /* namespace epic::index */

#endif /* EPIC_INDEX_ITFPOWERINDEX_H_ */
