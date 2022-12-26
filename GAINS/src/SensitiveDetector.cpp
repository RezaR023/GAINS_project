#include "SensitiveDetector.h"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "myglobals.h"

SensitiveDetector::SensitiveDetector(const G4String& name)
	: G4VSensitiveDetector(name) 
{
}

G4bool SensitiveDetector::ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist) 
{
	G4double Edep = aStep->GetTotalEnergyDeposit();

	if (Edep == 0.)
		return false;

	//Get the copynumber of the parent volume
	G4int detectorNumber = aStep->GetPreStepPoint()
		->GetTouchable()->GetCopyNumber(1);

	//Copy numbers start at 1
	MYGLOBALS::EdepsList[detectorNumber - 1] += Edep;
	return true;
}
