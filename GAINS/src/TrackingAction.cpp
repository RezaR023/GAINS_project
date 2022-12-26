#include "TrackingAction.h"
#include "G4Track.hh"
#include "G4Gamma.hh"
#include "myglobals.h"
#include "G4VProcess.hh"

TrackingAction::TrackingAction()
	: G4UserTrackingAction() 
{
}

TrackingAction::~TrackingAction()
{
}

void TrackingAction::PreUserTrackingAction(const G4Track* track)
{
	//Print particle name and creation process
	/*
	G4cout << track->GetParticleDefinition()->GetParticleName();

	const G4VProcess* process = track->GetCreatorProcess();
	if (process) {
		G4String processName = process->GetProcessName();
		G4cout << " " << processName;
	}

	G4cout << G4endl;
	*/
}