#pragma once
#include <G4Types.hh>
#include <vector>
#include "Specs.h"

namespace MYGLOBALS {

	extern const int NUM_DETECTORS;

	//Contains the energy deposited in each detector
	extern std::vector<G4double> EdepsList;

	//Contains specifications of the detectors
	extern const std::vector<Specs> DET_SPECS;
}