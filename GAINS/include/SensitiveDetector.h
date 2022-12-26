#pragma once

#include "G4VSensitiveDetector.hh"

class SensitiveDetector : public G4VSensitiveDetector 
{
public:
	SensitiveDetector(const G4String& name);

	virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
};

