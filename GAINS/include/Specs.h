#pragma once
#include <G4Types.hh>
#include "G4SystemOfUnits.hh"

class Specs {
public:
	G4int number;

	G4double theta;
	G4double phi;
	G4double distance;

	G4double holeDiameter;
	G4double crystalDiameter;
	G4double crystalLength;
	G4double holeDepth;

	G4double windowThickness;
	G4double distanceToCap = 5 * mm;
	G4double capThickness = 1.5 * mm;

	G4double fullRadius() const;
	G4double fullHalfLength() const;
};