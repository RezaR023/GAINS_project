#ifndef PRIMARY_GENERATOR_ACTION
#define PRIMARY_GENERATOR_ACTION

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ThreeVector.hh"

class G4GeneralParticleSource;
class Specs;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {

	G4GeneralParticleSource* m_particleSource;

public:
	PrimaryGeneratorAction();
	virtual ~PrimaryGeneratorAction();

	virtual void GeneratePrimaries(G4Event* anEvent);
};

#endif

