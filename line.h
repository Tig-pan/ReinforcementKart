#pragma once

#include <math.h>

/*
A struct to represent a line segment in the world. Used by Walls, Checkpoints, etc.
*/
struct Line
{
public:
	Line(float x1, float y1, float x2, float y2)
		: x1(x1), y1(y1), x2(x2), y2(y2)
	{}

	// Returns the distance from the start of the line to the point
	static float distanceToStart(float px, float py, Line line);

	// Returns the distance from the end of the line to the point
	static float distanceToEnd(float px, float py, Line line);

	// Returns the distance from the closest place on the line to the point
	static float distanceToLine(float px, float py, Line line);

	// Returns true if the lines intersect, also updates the output x and output y variables with the intersection point
	static bool getLineSegmentIntersection(Line kLine, Line wLine, float* ox, float* oy, float* onx, float* ony, float* otime);

	float x1;
	float y1;
	float x2;
	float y2;
};