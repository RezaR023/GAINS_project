#pragma once
#include "G4UserTrackingAction.hh"

class TrackingAction : public G4UserTrackingAction {
public:
	TrackingAction();
	~TrackingAction();

	virtual void  PreUserTrackingAction(const G4Track* track);
};