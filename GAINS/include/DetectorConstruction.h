#ifndef CYLINDRICAL_DETECTOR_CONSTRUCTION
#define CYLINDRICAL_DETECTOR_CONSTRUCTION

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "Specs.h"

class EventAction;

class DetectorConstruction : public G4VUserDetectorConstruction {
	void defineMaterials();
	G4LogicalVolume* buildDetector(const Specs& det);

public:
	DetectorConstruction();

	virtual G4VPhysicalVolume* Construct() override;
	virtual void ConstructSDandField() override;
	
};

#endif