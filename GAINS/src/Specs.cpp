#pragma once
#include "Specs.h"

G4double Specs::fullRadius() const
{
	return .5 * crystalDiameter + distanceToCap + capThickness;
}

G4double Specs::fullHalfLength() const
{
	return .5 * (crystalLength + distanceToCap + windowThickness);
}
