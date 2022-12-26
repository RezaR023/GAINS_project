#pragma once
#include "G4RunManager.hh"

class EfficiencyMessenger;

class RunManager : public G4RunManager {
	EfficiencyMessenger* m_messenger;

public:
	RunManager();
	~RunManager();
};