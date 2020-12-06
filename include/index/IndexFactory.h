#ifndef EPIC_INDEX_INDEXFACTORY_H_
#define EPIC_INDEX_INDEXFACTORY_H_

#include "ItfPowerIndex.h"

#include <iostream>
#include <map>
#include <string>

namespace epic::index {

class IndexFactory {
public:
	IndexFactory() = delete;

	/**
	 * Check if a certain power index abbreviation is valid (in the list of available indices)
	 *
	 * @param short_name power index abbreviation
	 * @return whether the given index abbreviation is valid
	 */
	static bool validateIndex(const std::string& short_name);

	/**
	 * A function that creates an instance of the index to be calculated.
	 *
	 * @param short_name the index name abbreviation defining the power index to create
	 * @param g an instance of mGame
	 * @param int_representation Defines the kind of integer representation to use for the calculation (gets passed to the constructor of the derived PowerIndex class).
	 *
	 * @return A powerindex instance.
	 */
	static ItfPowerIndex* new_powerIndex(const std::string& short_name, Game& g, IntRepresentation int_representation = DEFAULT);

	/**
	 * Delete a powerindex instance
	 *
	 * This method can be used to delete a powerindex instance previously created by the new_powerIndex() method. It only calls "delete index" which can also be done manually but for completeness this method exists.
	 *
	 * @param index The instance to delete. The pointer must point to an allocated object.
	 */
	static void delete_powerIndex(ItfPowerIndex* index);

	/**
	 * print a list of all available power indices including the abbreviation and the full name
	 *
	 * @param os the ostream to write the index list to
	 */
	static void printIndexList(std::ostream& os = std::cout);

private:
	/**
	 * A enum of abbreviations of each index
	 */
	enum IndexType {
		INVALID_INDEX, /**< Unknown/Invalid power index */
		ABZ,		   /**< absolute Banzhaf */
		BZ,			   /**< Banzhaf */
		CC,			   /**< Coleman collective */
		CI,			   /**< Coleman initiative */
		CP,			   /**< Coleman preventive */
		DP,			   /**< Deegan Packel */
		FT,			   /**< Felsenthal */
		HN,			   /**< Harsanyi Nash */
		J,			   /**< Johnston */
		KB,			   /**< Koenig Braeuninger (based on the Dubey-Shapley identity) */
		KBPH,		   /**< Koenig Braeuninger (based on raw Public Help theta) */
		N,			   /**< Nevison (based on the Dubey-Shapley identity)*/
		NPH,		   /**< Nevison (based on raw Public Help theta) */
		PG,			   /**< Public Good */
		PHT,		   /**< Public Help theta (based on the Dubey-Shapley identity)*/
		PHTPH,		   /**< Public Help theta (based on raw Public Help theta)) */
		PHX,		   /**< Public Help xi */
		PIF,		   /**< null-player-free Power Index F */
		PIG,		   /**< null-player-free Power Index G (based on the Dubey-Shapley identity)*/
		PIGPH,		   /**< null-player-free Power Index G (based on raw Public Help theta) */
		RA,			   /**< Rae */
		RBZ,		   /**< raw Banzhaf */
		RDP,		   /**< raw Deegan Packel */
		RFT,		   /**< raw Felsenthal */
		RJ,			   /**< raw Johnston */
		RPG,		   /**< raw Public Good */
		RPHT,		   /**< raw Public Help theta */
		RPHTSD,		   /**< raw Public Help theta (based on the Dubey-Shapley identity) */
		RPIG,		   /**< raw null-player-free Power Index G (based on the Dubey-Shapley identity) */
		RPIF,		   /**< raw null-player-free Power Index F */
		RSH,		   /**< raw Shapley Shubik */
		SH,			   /**< Shapley Shubik */
		T,			   /**< Tijs */
	};

	/**
	 * get the IndexType for a given power index abbreviation
	 *
	 * @param short_name the index abbreviation
	 * @return an IndexType object representing the given abbreviation
	 */
	static IndexType getIndexType(const std::string& short_name);

	/**
	 * Maps each IndexTypes to a pair of abbreviation and full name
	 */
	static const std::map<IndexType, std::pair<std::string, std::string>> cIndexNames;
};

} // namespace epic::index

#endif // EPIC_INDEX_INDEXFACTORY_H_
