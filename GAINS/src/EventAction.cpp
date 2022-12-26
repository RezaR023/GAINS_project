#include "EventAction.h"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"
#include "myglobals.h"
#include "G4SystemOfUnits.hh"

EventAction::EventAction()
	: G4UserEventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event* event)
{
	//reset energy desposition list
	std::fill(MYGLOBALS::EdepsList.begin(), MYGLOBALS::EdepsList.end(), 0.);
}

void EventAction::EndOfEventAction(const G4Event* event)
{
	//Check if more than 1 keV has been deposited in any detector
	bool hasDepositedEnergy =
		std::any_of(MYGLOBALS::EdepsList.begin(), MYGLOBALS::EdepsList.end(), 
			[](G4double Edep) {
				return Edep > 1 * keV; 
			});

	//Write data to ntuples
	if (hasDepositedEnergy) {
		G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
		for (int i = 0; i < MYGLOBALS::NUM_DETECTORS; i++)
			analysisManager->FillNtupleDColumn(0, i, MYGLOBALS::EdepsList[i]);
		analysisManager->AddNtupleRow(0);
	}

	//G4cout << "--------------------------\n\n";
}
