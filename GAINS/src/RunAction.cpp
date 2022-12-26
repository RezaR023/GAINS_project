#include <iostream>
#include "RunAction.h"
#include "G4AnalysisManager.hh"
#include <string>
#include "myglobals.h"
#include <G4UImanager.hh>

RunAction::RunAction()
	: G4UserRunAction()
{
	auto analysisManager = G4AnalysisManager::Instance();
	analysisManager->SetVerboseLevel(1);

	//Create a tuple for each detector
	analysisManager->CreateNtuple("GAINS", "Energy deposition");
	for (int i = 1; i <= MYGLOBALS::NUM_DETECTORS; i++)
		analysisManager->CreateNtupleDColumn("Edep" + std::to_string(i));
	analysisManager->FinishNtuple();
}

RunAction::~RunAction()
{
	delete G4AnalysisManager::Instance();
}

void RunAction::BeginOfRunAction(const G4Run*)
{
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	if (analysisManager->GetFileName() != "")
		analysisManager->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run*)
{
	//save ntuples
	//      
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
	if (analysisManager->GetFileName() != "") {
		analysisManager->Write();
		analysisManager->CloseFile();
	}
}
