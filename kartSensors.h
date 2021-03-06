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
	// A constructor for the KartSensors, takes the amount of checkpoints in the current race as an input
	KartSensors();

	// Updates the positions/rotations/progress/vision of the kart so that the raycasts operate relative to the kart and that the sensors store updated data
	void tick(float newOriginX, float newOriginY, bool newIsDrifting, float newXVelocity, float newYVelocity, float newOriginAngle, float currentForwardProgress, Line* newNextCheckpointLine, Wall** newVisionGroup, int newVisionGroupSize);

	/*Does a raycast where the origin is based on the leftToRight and frontToBack variables, and the angle is the kartAngle + relativeAngle.Then stores the outputs in the KartSensors' output variables

	leftToRight: a float from 0-1, where 0 represents the raycast starting on the left side of the kart, and 1 for the right
	frontToBack: a float from 0-1, where 0 represents the raycast starting on the front side of the kart, and 1 for the back
	relativeAngle: the relative angle (degrees) to cast the ray from the current kart angle
	length: the length of the raycast*/
	void raycastRelativeAngle(float leftToRight, float frontToBack, float relativeAngle, float length);

	// Returns the angle (radians) to the closest point on the checkpoint from the kart and its current angle
	float getAngleToNextCheckpoint();

	// Returns the forward component of the current velocity (will be negative if moving backward)
	float getForwardVelocity();

	// Returns the rightward component of the current velocity (will be negative if moving leftward)
	float getRightwardVelocity();

	bool getIsDrifting() { return isDrifting;  }

	float getProgressMade() { return progressMade; }

	float getOutputX() { return outputX; }
	float getOutputY() { return outputY; }
	float getOutputNormalX() { return outputNormalX; }
	float getOutputNormalY() { return outputNormalY; }
	float getOutputTime() { return outputTime; }
private:
	// Executes a raycast over all the walls in the kart's current checkpoint's vision group
	void raycast(Line kartLine);

	int checkpointCount;

	float originX;
	float originY;

	float xVelocity;
	float yVelocity;

	bool isDrifting;

	float originAngle;

	Line* nextCheckpointLine;
	Wall** visionGroup;
	int visionGroupSize;

	float previousForwardProgress;
	float progressMade;

	float outputX;
	float outputY;
	float outputNormalX;
	float outputNormalY;
	float outputTime;
};