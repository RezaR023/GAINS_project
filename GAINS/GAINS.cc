#include <iostream>
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"

#include "RunManager.h"
#include "PhysicsList.h"
#include "DetectorConstruction.h"
#include "ActionInitialization.h"
#include "PrimaryGeneratorAction.h"
#include "G4AtomicTransitionManager.hh"

#include "Randomize.hh"

int main(int argc, char** argv)
{
	//detect interactive mode (if no arguments) and define UI session
	G4UIExecutive* ui = nullptr;
	if (argc == 1) ui = new G4UIExecutive(argc, argv);

	RunManager* runManager = new RunManager;
	runManager->SetUserInitialization(new DetectorConstruction);
	runManager->SetUserInitialization(new PhysicsList(2));
	runManager->SetUserInitialization(new ActionInitialization);
	runManager->Initialize();

	//initialize visualization
	G4VisManager* visManager = nullptr;

	//get the pointer to the User Interface manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	if (ui) {
		//interactive mode
		visManager = new G4VisExecutive;
		visManager->Initialize();
		UImanager->ApplyCommand("/control/execute deexcite.mac");
		ui->SessionStart();
		delete ui;
	}
	else {
		//batch mode
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command + fileName);
	}

	//job termination
	delete visManager;
	delete runManager;
}
