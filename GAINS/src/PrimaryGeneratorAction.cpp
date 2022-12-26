#include "PrimaryGeneratorAction.h"
#include "G4ParticleTable.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "G4AnalysisManager.hh"
#include <cmath>
#include "myglobals.h"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include <G4UImanager.hh>
#include "G4Ions.hh"
#include <sstream>
#include "G4INCLRandom.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
	: G4VUserPrimaryGeneratorAction()
{
	m_particleSource = new G4GeneralParticleSource;
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
	delete m_particleSource;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
	m_particleSource->GeneratePrimaryVertex(anEvent);
}
