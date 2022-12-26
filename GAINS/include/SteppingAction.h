#pragma once

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class EventAction;

class SteppingAction : public G4UserSteppingAction {
public:
	SteppingAction();

	virtual void UserSteppingAction(const G4Step* step);
};

