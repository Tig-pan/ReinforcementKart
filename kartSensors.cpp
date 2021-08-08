#include "kartSensors.h"

KartSensors::KartSensors()
{

}

void KartSensors::tick(float newOriginX, float newOriginY, float newOriginAngle, Line* newNextCheckpointLine, Wall** newVisionGroup, int newVisionGroupSize)
{
	originX = newOriginX;
	originY = newOriginY;
	originAngle = newOriginAngle;

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
	float angle = Line::angleToLine(originX, originY, cosf(originAngle * DEG_TO_RAD), sinf(originAngle * DEG_TO_RAD), nextCheckpointLine);
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

void KartSensors::raycast(Line kartLine)
{
	outputTime = 1.0f;
	for (int i = 0; i < visionGroupSize; i++)
	{
		Line::getLineSegmentIntersection(&kartLine, visionGroup[i]->getLine(), &outputX, &outputY, &outputNormalX, &outputNormalY, &outputTime);
	}
}
