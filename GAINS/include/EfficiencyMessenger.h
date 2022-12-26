#pragma once

#include "G4UImessenger.hh"
#include "globals.hh"

class RunManager;

class EfficiencyMessenger : public G4UImessenger {
public:
	EfficiencyMessenger(RunManager* runManager);
	~EfficiencyMessenger();

public:
	void SetNewValue(G4UIcommand* command, G4String newValues);
	G4String GetCurrentValue(G4UIcommand* command);

private:
	RunManager* m_runManager;
	G4UIcommand* m_runSinglePeaksCmd;
};