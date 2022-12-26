#pragma once
#include "G4UserEventAction.hh"
#include "globals.hh"
#include "RunAction.h"
#include <fstream>

class EventAction : public G4UserEventAction 
{
public:
	EventAction();
	virtual void BeginOfEventAction(const G4Event* event);
	virtual void EndOfEventAction(const G4Event* event);
};

