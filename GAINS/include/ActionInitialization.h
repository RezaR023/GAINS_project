#pragma once
#include "G4VUserActionInitialization.hh"

class ActionInitialization : public G4VUserActionInitialization {
public:
	ActionInitialization();

	virtual void Build() const;
};

