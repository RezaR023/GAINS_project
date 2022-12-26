#include "SteppingAction.h"
#include "EventAction.h"
#include "G4Step.hh"

SteppingAction::SteppingAction()
	: G4UserSteppingAction() {
}

void SteppingAction::UserSteppingAction(const G4Step* step) {
}
