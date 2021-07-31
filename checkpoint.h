#pragma once

/*
Used as checkpoints to define progress throughout the race. You must pass all the checkpoints in order to finish the race.
You can navigate backwards through checkpoints, but you still need to hit them all in order. The backwards navigation is only for
optimizing the Walls.
*/
class Checkpoint
{
public:
	Checkpoint(float x1, float y1, float x2, float y2, WallGroup* wallGrouping)
		: wallGrouping(wallGrouping), x1(x1), y1(y1), x2(x2), y2(y2) {}

	WallGroup* getWallGroup() { return wallGrouping; }

	void setX1(TextFieldResult result) { x1 = result.floatResult; }
	void setY1(TextFieldResult result) { y1 = result.floatResult; }
	void setX2(TextFieldResult result) { x2 = result.floatResult; }
	void setY2(TextFieldResult result) { y2 = result.floatResult; }

	float getX1() { return x1; }
	float getY1() { return y1; }
	float getX2() { return x2; }
	float getY2() { return y2; }
private:
	WallGroup* wallGrouping;

	float x1;
	float y1;
	float x2;
	float y2;
};