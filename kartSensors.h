#pragma once

#include "kart.h"

/* A class that allows accessing various data about the kart. Typically used by inputs/AIs.
*/
class KartSensors
{
public:
	// A constructor for the KartSensors, takes the Kart to make the sensors for as a parameter
	KartSensors(Kart& kart);

	/*Does a raycast where the origin is based on the leftToRight and frontToBack variables, and the angle is the kartAngle + relativeAngle.Then stores the outputs in the KartSensors' output variables

	leftToRight: a float from 0-1, where 0 represents the raycast starting on the left side of the kart, and 1 for the right
	frontToBack: a float from 0-1, where 0 represents the raycast starting on the front side of the kart, and 1 for the back
	relativeAngle: the relative angle to cast the ray from the current kart angle
	length: the length of the raycast*/
	void raycastRelativeAngle(float leftToRight, float frontToBack, float relativeAngle, float length);

	float getOutputX() { return outputX; }
	float getOutputY() { return outputY; }
	float getOutputNormalX() { return outputNormalX; }
	float getOutputNormalY() { return outputNormalY; }
	float getOutputTime() { return outputTime; }
private:
	// Executes a raycast over all the walls in the kart's current checkpoint's vision group
	void raycast(float startX, float startY, float angle);

	float outputX;
	float outputY;
	float outputNormalX;
	float outputNormalY;
	float outputTime;
};