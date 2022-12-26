#include "RunManager.h"
#include "PrimaryGeneratorAction.h"
#include "RunAction.h"
#include "EfficiencyMessenger.h"

RunManager::RunManager()
	: G4RunManager()
{
	m_messenger = new EfficiencyMessenger(this);
}

RunManager::~RunManager()
{
	delete m_messenger;
}
