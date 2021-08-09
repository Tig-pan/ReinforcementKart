#include "kartSensors.h"

KartSensors::KartSensors()
{
	previousForwardProgress = 0.0f;
}

void KartSensors::tick(float newOriginX, float newOriginY, bool newIsDrifting, float newXVelocity, float newYVelocity, float newOriginAngle, float currentForwardProgress, Line* newNextCheckpointLine, Wall** newVisionGroup, int newVisionGroupSize)
{
	originX = newOriginX;
	originY = newOriginY;

	isDrifting = newIsDrifting;

	xVelocity = newXVelocity;
	yVelocity = newYVelocity;

	originAngle = newOriginAngle;

	progressMade = currentForwardProgress - previousForwardProgress;
	previousForwardProgress = currentForwardProgress;

	nextCheckpointLine = newNextCheckpointLine;

	visionGroup = newVisionGroup;
	visionGroupSize = newVisionGroupSize;
}

void KartSensors::raycastRelativeAngle(float leftToRight, float frontToBack, float relativeAngle, float length)
{
	float cosAngle = cosf(originAngle * DEG_TO_RAD);
	float sinAngle = sinf(originAngle * DEG_TO_RAD);

	float startX = originX + sinAngle * (0.5f - leftToRight) * KART_WIDTH + cosAngle * (0.5f - frontToBack) * KART_LENGTH;
	float startY = originY - cosAngle * (0.5f - leftToRight) * KART_WIDTH + sinAngle * (0.5f - frontToBack) * KART_LENGTH;

	//std::cout << startX << " " << startY << " " << startX + cosf((originAngle + relativeAngle) * DEG_TO_RAD) * length << " " << startY + sinf((originAngle + relativeAngle) * DEG_TO_RAD) * length << "\n";

	raycast(Line(startX, startY, startX + cosf((originAngle + relativeAngle) * DEG_TO_RAD) * length, startY + sinf((originAngle + relativeAngle) * DEG_TO_RAD) * length));
}

float KartSensors::getAngleToNextCheckpoint()
{
	const float angle = Line::angleToLine(originX, originY, cosf(originAngle * DEG_TO_RAD), sinf(originAngle * DEG_TO_RAD), nextCheckpointLine);
	// TODO: this is an unoptimal way of solving this
	if (Line::distanceToLine(originX + cosf(originAngle * DEG_TO_RAD + angle), originY + sinf(originAngle * DEG_TO_RAD + angle), nextCheckpointLine) <
		Line::distanceToLine(originX + cosf(originAngle * DEG_TO_RAD - angle), originY + sinf(originAngle * DEG_TO_RAD - angle), nextCheckpointLine))
	{
		return angle;
	}
	else
	{
		return -angle;
	}
}

float KartSensors::getForwardVelocity()
{
	const float dot = xVelocity * cosf(originAngle * DEG_TO_RAD) + yVelocity * sinf(originAngle * DEG_TO_RAD);

	// we know the magnitude squared of the forwad projection is 1, so we can just return the dot
	return dot;
}

float KartSensors::getRightwardVelocity()
{
	const float dot = xVelocity * cosf((originAngle + 90.0f) * DEG_TO_RAD) + yVelocity * sinf((originAngle + 90.0f) * DEG_TO_RAD);

	// we know the magnitude squared of the forwad projection is 1, so we can just return the dot
	return dot;
}

void KartSensors::raycast(Line kartLine)
{
	outputTime = 1.0f;
	for (int i = 0; i < visionGroupSize; i++)
	{
		Line::getLineSegmentIntersection(&kartLine, visionGroup[i]->getLine(), &outputX, &outputY, &outputNormalX, &outputNormalY, &outputTime);
	}
}
