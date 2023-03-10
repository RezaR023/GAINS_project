#pragma once
#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

class RunAction : public G4UserRunAction 
{
public:
	RunAction();
	virtual ~RunAction();

	virtual void BeginOfRunAction(const G4Run*);
	virtual void EndOfRunAction(const G4Run*);
};

