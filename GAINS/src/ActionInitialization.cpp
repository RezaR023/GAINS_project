#include "ActionInitialization.h"
#include "PrimaryGeneratorAction.h"
#include "RunAction.h"
#include "EventAction.h"
#include "SteppingAction.h"
#include "TrackingAction.h"

ActionInitialization::ActionInitialization()
	: G4VUserActionInitialization() 
{
}

void ActionInitialization::Build() const 
{
	SetUserAction(new PrimaryGeneratorAction);

	RunAction* runAction = new RunAction;
	SetUserAction(runAction);

	EventAction* eventAction = new EventAction;
	SetUserAction(eventAction);

	SteppingAction* steppingAction = new SteppingAction;
	SetUserAction(steppingAction);

	TrackingAction* trackingAction = new TrackingAction;
	SetUserAction(trackingAction);
}
