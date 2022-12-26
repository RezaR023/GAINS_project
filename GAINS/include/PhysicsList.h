#pragma once

#include "QBBC.hh"

class PhysicsList : public QBBC {
public:
	PhysicsList(G4int ver = 1);

protected:
	virtual void ConstructProcess() override;
};