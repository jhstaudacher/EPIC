#include "index/IndexFactory.h"

#include "FastUpperBoundApproximation.h"
#include "index/AbsoluteBanzhaf.h"
#include "index/AbsolutePowerIndexG.h"
#include "index/AbsolutePublicGood.h"
#include "index/Banzhaf.h"
#include "index/BanzhafOwen.h"
#include "index/BanzhafBelow.h"
#include "index/ColemanCollective.h"
#include "index/ColemanInitiative.h"
#include "index/ColemanPreventive.h"
#include "index/DeeganPackel.h"
#include "index/FelsenthalIndex.h"
#include "index/HarsanyiNashIndex.h"
#include "index/Johnston.h"
#include "index/KoenigBraeuninger.h"
#include "index/KoenigBraeuningerPH.h"
#include "index/Nevison.h"
#include "index/NevisonPH.h"
#include "index/Owen.h"
// #include "index/OwenExtendedPGI.h"
#include "index/PowerIndexF.h"
#include "index/PowerIndexG.h"
#include "index/PowerIndexGPH.h"
#include "index/PublicGood.h"
#include "index/PublicHelpTheta.h"
#include "index/PublicHelpThetaPH.h"
#include "index/PublicHelpXi.h"
#include "index/Rae.h"
#include "index/RawBanzhaf.h"
#include "index/RawBanzhafBelow.h"
#include "index/RawDeeganPackel.h"
#include "index/RawFelsenthal.h"
#include "index/RawJohnston.h"
#include "index/RawPowerIndexF.h"
#include "index/RawPowerIndexG.h"
#include "index/RawPublicGood.h"
#include "index/RawPublicHelpTheta.h"
#include "index/RawPublicHelpThetaSD.h"
#include "index/RawShapleyShubik.h"
#include "index/RawShapleyShubikBelow.h"
#include "index/ShapleyShubik.h"
#include "index/ShapleyShubikBelow.h"
#include "index/SingleValueW.h"
#include "index/SingleValueWM.h"
#include "index/SingleValueWS.h"
#include "index/SolidarityPGI.h"
#include "index/SymmetricCoalitionalBanzhaf.h"
#include "index/Tijs.h"
// #include "index/ThreatPGI1.h"
// #include "index/ThreatPGI2.h"
// #include "index/ThreatPGI3.h"
#include "index/UnionPGI.h"

const std::map<epic::index::IndexFactory::IndexType, std::pair<std::string, std::string>> epic::index::IndexFactory::cIndexNames = {
	{INVALID_INDEX, {"INVALID", "invalid index"}},
	{ABZ, {"ABZ", "absolute Banzhaf"}},
	{APG, {"APG", "absolute Public Good"}},
	{APIG, {"APIG", "absolute null-player-free Power Index G (based on the Dubey-Shapley identity)"}},
	{BZ, {"BZ", "Banzhaf"}},
	{BO, {"BO", "Banzhaf-Owen"}},
	{BZB, {"BZB", "Banzhaf below"}},
	{CC, {"CC", "Coleman collective"}},
	{CI, {"CI", "Coleman initiative"}},
	{CP, {"CP", "Coleman preventive"}},
	{DP, {"DP", "Deegan Packel"}},
	{FT, {"FT", "Felsenthal"}},
	{HN, {"HN", "Harsanyi Nash"}},
	{J, {"J", "Johnston"}},
	{KB, {"KB", "Koenig Braeuninger (based on the Dubey-Shapley identity)"}},
	{KBPH, {"KBPH", "Koenig Braeuninger (based on raw Public Help theta)"}},
	{N, {"N", "Nevison (based on the Dubey-Shapley identity)"}},
	{NPH, {"NPH", "Nevsion (based on raw Public Help theta)"}},
	{O, {"O", "Owen"}},
	//{OPGI, {"OPGI", "Owen Extended Public Good Index"}},
	{PG, {"PG", "Public Good"}},
	{PHT, {"PHT", "Public Help theta (based on the Dubey-Shapley identity)"}},
	{PHTPH, {"PHTPH", "Public Help theta (based on the raw Public Help theta)"}},
	{PHX, {"PHX", "Public Help xi"}},
	{PIF, {"PIF", "null-player-free Power Index F"}},
	{PIG, {"PIG", "null-player-free Power Index G (based on the Dubey-Shapley identity)"}},
	{PIGPH, {"PIGPH", "null-player-free Power Index G (based on raw Public Help theta)"}},
	{RA, {"RA", "Rae"}},
	{RBZ, {"RBZ", "raw Banzhaf"}},
	{RBZB, {"RBZB", "raw Banzhaf from below"}},
	{RDP, {"RDP", "raw Deegan Packel"}},
	{RFT, {"RFT", "raw Felsenthal"}},
	{RJ, {"RJ", "raw Johnston"}},
	{RPG, {"RPG", "raw Public Good"}},
	{RPHT, {"RPHT", "raw Public Help theta"}},
	{RPHTSD, {"RPHTSD", "raw Public Help theta (based on the Dubey-Shapley identity)"}},
	{RPIG, {"RPIG", "raw null-player-free Power Index G (based on the Dubey-Shapley identity)"}},
	{RPIF, {"RPIF", "raw null-player-free Power Index F"}},
	{RSH, {"RSH", "raw Shapley Shubik"}},
	{SCB, {"SCB", "Symmetric Coalitional Banzhaf"}},
	{RSHB, {"RSHB", "raw Shapley Shubik below"}},
	{SH, {"SH", "Shapley Shubik"}},
	{SHB, {"SHB", "Shapley Shubik below"}},
	{SPGI, {"SPGI", "Solidarity Public Good Index"}},
	{UPGI, {"UPGI", "Union Public Good Index"}},
	{T, {"T", "Tijs"}},
	//{TPGI1, {"TPGI1", "Threat Public Good Index 1"}},
	//{TPGI2, {"TPGI2", "Threat Public Good Index 2"}},
	//{TPGI3, {"TPGI3", "Threat Public Good Index 3"}},
	{W, {"W", "Number of winning coalitions"}},
	{WM, {"WM", "Number of minimal winning coalitions"}},
	{WS, {"WS", "Number of minimal winning coalitions of smallest cardinality"}}};

bool epic::index::IndexFactory::validateIndex(const std::string& short_name) {
	return getIndexType(short_name) != INVALID_INDEX;
}

epic::index::ItfPowerIndex* epic::index::IndexFactory::new_powerIndex(const std::string& short_name, Game* g, IntRepresentation int_representation) {
	ItfPowerIndex* index = nullptr;

	switch (getIndexType(short_name)) {
		case IndexType::INVALID_INDEX: index = nullptr; break;
		case IndexType::ABZ: index = new AbsoluteBanzhaf(); break;
		case IndexType::BZ: index = new Banzhaf(); break;
		case IndexType::BO: index = new BanzhafOwen(); break;
		case IndexType::BZB: index = new BanzhafBelow(); break;
		case IndexType::APG: index = new AbsolutePublicGood(); break;
		case IndexType::APIG: index = new AbsolutePowerIndexG(g); break;
		case IndexType::CC: index = new ColemanCollective(); break;
		case IndexType::CI: index = new ColemanInitiative(); break;
		case IndexType::CP: index = new ColemanPreventive(); break;
		case IndexType::DP: index = new DeeganPackel(); break;
		case IndexType::FT: index = new FelsenthalIndex(g); break;
		case IndexType::HN: index = new HarsanyiNashIndex(); break;
		case IndexType::J: index = new Johnston(g); break;
		case IndexType::KB: index = new KoenigBraeuninger(g); break;
		case IndexType::KBPH: index = new KoenigBraeuningerPH(g); break;
		case IndexType::N: index = new Nevison(g); break;
		case IndexType::NPH: index = new NevisonPH(g); break;
		case IndexType::O: index = new Owen(); break;
		case IndexType::PG: index = new PublicGood(); break;
		case IndexType::PHT: index = new PublicHelpTheta(g); break;
		case IndexType::PHTPH: index = new PublicHelpThetaPH(g); break;
		case IndexType::PHX: index = new PublicHelpXi(g); break;
		case IndexType::PIF: index = new PowerIndexF(g); break;
		case IndexType::PIG: index = new PowerIndexG(g); break;
		case IndexType::PIGPH: index = new PowerIndexGPH(g); break;
		case IndexType::RA: index = new Rae(g); break;
		case IndexType::RBZ: index = new RawBanzhaf(); break;
		case IndexType::RBZB: index = new RawBanzhafBelow(); break;
		case IndexType::RDP: index = new RawDeeganPackel(); break;
		case IndexType::RFT: index = new RawFelsenthal(g); break;
		case IndexType::RJ: index = new RawJohnston(g); break;
		case IndexType::RPG: index = new RawPublicGood(); break;
		case IndexType::RPHT: index = new RawPublicHelpTheta(g); break;
		case IndexType::RPHTSD: index = new RawPublicHelpThetaSD(g); break;
		case IndexType::RPIG: index = new RawPowerIndexG(g); break;
		case IndexType::RPIF: index = new RawPowerIndexF(g); break;
		case IndexType::RSH: index = new RawShapleyShubik(); break;
		case IndexType::SCB: index = new SymmetricCoalitionalBanzhaf(); break;
		case IndexType::RSHB: index = new RawShapleyShubikBelow(); break;
		case IndexType::SH: index = new ShapleyShubik(); break;
		case IndexType::SHB: index = new ShapleyShubikBelow(); break;
		case IndexType::SPGI: index = new SolidarityPGI(); break;
		case IndexType::T: index = new Tijs(); break;
		case IndexType::UPGI: index = new UnionPGI(); break;
		case IndexType::W: index = new SingleValueW(); break;
		case IndexType::WM: index = new SingleValueWM(); break;
		case IndexType::WS: index = new SingleValueWS(g); break;
	}

	return index;
}

void epic::index::IndexFactory::delete_powerIndex(ItfPowerIndex* index) {
	delete index;
}

void epic::index::IndexFactory::printIndexList(std::ostream& os) {
	for (auto it = ++cIndexNames.begin(); it != cIndexNames.end(); ++it) { // first entry is INVALID_INDEX (do not print that one)
		os << it->second.first << ":\t" << it->second.second << std::endl;
	}
}

epic::index::IndexFactory::IndexType epic::index::IndexFactory::getIndexType(const std::string& short_name) {
	for (const auto& it : cIndexNames) {
		if (it.second.first == short_name) {
			return it.first;
		}
	}

	return INVALID_INDEX;
}
