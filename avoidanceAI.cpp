#include "input.h"

AvoidanceAI::AvoidanceAI(KartSensors* sensor)
	: sensor(sensor)
{
	currentDecision = 8;
}

void AvoidanceAI::update()
{
	// Raycasts for the front of the kart, used mainly to determine the speed of the kart
	sensor->raycastRelativeAngle(0.5f, 0.0f, 0, 700);
	float frontA = sensor->getOutputTime();
	sensor->raycastRelativeAngle(0.8f, 0.0f, 5, 500);
	float frontB = sensor->getOutputTime();
	sensor->raycastRelativeAngle(0.2f, 0.0f, -5, 500);
	float frontC = sensor->getOutputTime();

	// Raycasts for the immediate right and left of the kart, used primarily for obstacle avoidance
	sensor->raycastRelativeAngle(1.0f, 1.0f, 10, 250);
	float right = sensor->getOutputTime();

	sensor->raycastRelativeAngle(0.0f, 1.0f, -10, 250);
	float left = sensor->getOutputTime();

	// Raycasts for a cone around the front of the kart, used to determine the path and for obstacle avoidance
	const int VIEW_COUNT = 17;

	float views[VIEW_COUNT];

	sensor->raycastRelativeAngle(0.0f, 0.5f, -100, 700);
	views[0] = sensor->getOutputTime();
	sensor->raycastRelativeAngle(0.0f, 0.5f, -70, 700);
	views[1] = sensor->getOutputTime();
	sensor->raycastRelativeAngle(0.0f, 0.5f, -60, 700);
	views[2] = sensor->getOutputTime();
	sensor->raycastRelativeAngle(0.0f, 0.5f, -50, 700);
	views[3] = sensor->getOutputTime();
	sensor->raycastRelativeAngle(0.0f, 0.0f, -40, 700);
	views[4] = sensor->getOutputTime();
	sensor->raycastRelativeAngle(0.0f, 0.0f, -30, 700);
	views[5] = sensor->getOutputTime();
	sensor->raycastRelativeAngle(0.0f, 0.0f, -20, 700);
	views[6] = sensor->getOutputTime();
	sensor->raycastRelativeAngle(0.0f, 0.0f, -10, 700);
	views[7] = sensor->getOutputTime();

	sensor->raycastRelativeAngle(0.5f, 0.0f, 0, 700);
	views[8] = sensor->getOutputTime() + 0.1f;

	sensor->raycastRelativeAngle(1.0f, 0.0f, 10, 700);
	views[9] = sensor->getOutputTime();
	sensor->raycastRelativeAngle(1.0f, 0.0f, 20, 700);
	views[10] = sensor->getOutputTime();
	sensor->raycastRelativeAngle(1.0f, 0.0f, 30, 700);
	views[11] = sensor->getOutputTime();
	sensor->raycastRelativeAngle(1.0f, 0.0f, 40, 700);
	views[12] = sensor->getOutputTime();
	sensor->raycastRelativeAngle(1.0f, 0.5f, 50, 700);
	views[13] = sensor->getOutputTime();
	sensor->raycastRelativeAngle(1.0f, 0.5f, 60, 700);
	views[14] = sensor->getOutputTime();
	sensor->raycastRelativeAngle(1.0f, 0.5f, 70, 700);
	views[15] = sensor->getOutputTime();
	sensor->raycastRelativeAngle(1.0f, 0.5f, 100, 700);
	views[16] = sensor->getOutputTime();

	// determine the speed of the kart based on the front facing raycasts
	if (frontA < 0.025f || frontB < 0.025f || frontC < 0.025f)
	{
		currentMovement = -1.0f;
		isBackingUp = true;
	}
	else if ((frontA < 0.15f || frontB < 0.15f || frontC < 0.15f) && isBackingUp)
	{
		currentMovement = -1.0f;
	}
	else
	{
		isBackingUp = false;
		
		float lowestFront = (frontA < frontB && frontA < frontC) ? frontA : ((frontB < frontA && frontB < frontC) ? frontB : frontC);

		currentMovement = lowestFront * 0.7f + 0.3f;
	}

	float angle = sensor->getAngleToNextCheckpoint() * RAD_TO_DEG;

	// Fine the furtherst open raycast for the cones, then head in that direction
	float lowest = 1.0f;
	float highest = views[currentDecision] + 0.1f;

	for (int i = 0; i < VIEW_COUNT; i++)
	{
		// Give a bonus to directions that align with the checkpoint direction
		float diff = 15.0f - fabsf(((i - VIEW_COUNT / 2.0f + 0.5f) / (VIEW_COUNT / 2.0f - 0.5f)) * 80.0f - angle);
		if (diff > 0)
		{
			views[i] += diff * 0.02f;
		}

		if (views[i] > highest)
		{
			highest = views[i];
			currentDecision = i;
		}
		if (views[i] < lowest)
		{
			lowest = views[i];
		}
	}

	currentSteer = (currentDecision - VIEW_COUNT / 2.0f + 0.5f) / (VIEW_COUNT / 2.0f - 0.5f) * ((1.0f - lowest) * 8.0f + 1.0f);
	currentSteer += (right - left) * 6.0f;

	// Steer slightly towards the next checkpoint
	if (angle > 40.0f)
	{
		angle -= 40.0f;
	}
	else if (angle < -40.0f)
	{
		angle += 40.0f;
	}
	else
	{
		angle = 0.0f;
	}
	currentSteer += angle / 20.0f;

	// Drift if the steering is too great
	if (currentSteer > 8.0f)
	{
		currentDrift = true;
		currentSteer = 1.0f;
	}
	else if (currentSteer < -8.0f)
	{
		currentDrift = true;
		currentSteer = -1.0f;
	}
	else
	{
		currentDrift = false;
		if (currentSteer > 1.0f)
		{
			currentSteer = 1.0f;
		}
		else if (currentSteer < -1.0f)
		{
			currentSteer = -1.0f;
		}
	}

	if (isBackingUp)
	{
		currentSteer = -currentSteer;
	}
}

float AvoidanceAI::getSteering()
{
	return currentSteer;
}

float AvoidanceAI::getMovement()
{
	return currentMovement;
}

bool AvoidanceAI::getDrift()
{
	return currentDrift;
}

bool AvoidanceAI::getAction()
{
	return false;
}
