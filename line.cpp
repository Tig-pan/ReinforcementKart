#include "line.h"

float Line::distanceToStart(float px, float py, Line line)
{
	float xDist = px - line.x1;
	float yDist = py - line.y1;
	return sqrtf(xDist * xDist + yDist * yDist);
}

float Line::distanceToEnd(float px, float py, Line line)
{
	float xDist = px - line.x2;
	float yDist = py - line.y2;
	return sqrtf(xDist * xDist + yDist * yDist);
}

float Line::distanceToLine(float px, float py, Line line)
{
	float lineWX = line.x1 - line.x2;
	float lineWY = line.y1 - line.y2;

	float distance = lineWX * lineWX + lineWY * lineWY;
	float dot = (px - line.x2) * lineWX + (py - line.y2) * lineWY;
	float ratio = dot / distance;
	if (ratio < 0.0f)
	{
		ratio = 0.0f;
	}
	else if (ratio > 1.0f)
	{
		ratio = 1.0f;
	}

	float closestX = line.x2 + lineWX * ratio;
	float closestY = line.y2 + lineWY * ratio;

	return sqrtf((px - closestX) * (px - closestX) + (py - closestY) * (py - closestY));
}

bool Line::getLineSegmentIntersection(Line kLine, Line wLine, float* ox, float* oy, float* onx, float* ony, float* otime)
{
	// the k's refer to the kart, the w's refer to the wall or whatever else is being checked against, the o's are the out's and receive values in the event of a collision
	// to use this in a raycast scenario, have k1 be the starting position, and k2 be the next position
	float lineKX = kLine.x2 - kLine.x1;
	float lineKY = kLine.y2 - kLine.y1;
	float lineWX = wLine.x2 - wLine.x1;
	float lineWY = wLine.y2 - wLine.y1;

	float denominator = lineKX * lineWY - lineKY * lineWX;
	if (denominator == 0.0f) // lines are parallel, and therefore don't intersect
	{
		return false;
	}
	float kFraction = (lineKX * (kLine.y1 - wLine.y1) - lineKY * (kLine.x1 - wLine.x1)) / denominator; // aka time
	float wFraction = (lineWX * (kLine.y1 - wLine.y1) - lineWY * (kLine.x1 - wLine.x1)) / denominator;

	if (kFraction >= 0.0f && kFraction <= 1.0f && wFraction >= 0.0f && wFraction <= 1.0f && kFraction < *otime)
	{
		// only overwrite the time if this one is shorter
		*otime = kFraction;
		*ox = kLine.x1 + kFraction * lineKX;
		*oy = kLine.y1 + kFraction * lineKY;
		float normalMagnitude = sqrtf(lineWX * lineWX + lineWY * lineWY);
		if (((lineWX == 0.0f && kLine.x1 > wLine.x1) || (kLine.y1 < (lineWY / lineWX) * (kLine.x1 - wLine.x1) + wLine.y1)))
		{
			if (lineWX >= 0.0f)
			{
				*onx = lineWY / normalMagnitude; // x normal
				*ony = -lineWX / normalMagnitude; // y normal
			}
			else
			{
				*onx = -lineWY / normalMagnitude; // x normal
				*ony = lineWX / normalMagnitude; // y normal
			}
		}
		else
		{
			if (lineWX >= 0.0f)
			{
				*onx = -lineWY / normalMagnitude; // x normal
				*ony = lineWX / normalMagnitude; // y normal
			}
			else
			{
				*onx = lineWY / normalMagnitude; // x normal
				*ony = -lineWX / normalMagnitude; // y normal
			}
		}

		return true; // only return true if this is the first collision (kFraction < *otime)
	}

	return false;
}
