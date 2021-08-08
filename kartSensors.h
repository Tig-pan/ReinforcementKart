#pragma once

#include <SFML/Graphics.hpp>

#include "line.h"
#include "wall.h"
#include "constants.h"

/* A class that allows accessing various data about the kart. Typically used by inputs/AIs.
*/
class KartSensors
{
public:
	// A constructor for the KartSensors
	KartSensors();

	// Updates the positions/rotations of the kart so that the raycasts operate relative to the kart
	void tick(float newOriginX, float newOriginY, float newOriginAngle, Line* newNextCheckpointLine, Wall** newVisionGroup, int newVisionGroupSize);

	/*Does a raycast where the origin is based on the leftToRight and frontToBack variables, and the angle is the kartAngle + relativeAngle.Then stores the outputs in the KartSensors' output variables

	leftToRight: a float from 0-1, where 0 represents the raycast starting on the left side of the kart, and 1 for the right
	frontToBack: a float from 0-1, where 0 represents the raycast starting on the front side of the kart, and 1 for the back
	relativeAngle: the relative angle to cast the ray from the current kart angle
	length: the length of the raycast*/
	void raycastRelativeAngle(float leftToRight, float frontToBack, float relativeAngle, float length);

	// returns the angle to the closest point on the checkpoint from the kart and its current angle
	float getAngleToNextCheckpoint();

	float getOutputX() { return outputX; }
	float getOutputY() { return outputY; }
	float getOutputNormalX() { return outputNormalX; }
	float getOutputNormalY() { return outputNormalY; }
	float getOutputTime() { return outputTime; }
private:
	// Executes a raycast over all the walls in the kart's current checkpoint's vision group
	void raycast(Line kartLine);

	float originX;
	float originY;
	float originAngle;
	Line* nextCheckpointLine;
	Wall** visionGroup;
	int visionGroupSize;

	float outputX;
	float outputY;
	float outputNormalX;
	float outputNormalY;
	float outputTime;
};