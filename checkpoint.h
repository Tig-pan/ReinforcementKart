#pragma once

#include <list>
#include "line.h"

/*
Used as checkpoints to define progress throughout the race. You must pass all the checkpoints in order to finish the race.
You can navigate backwards through checkpoints, but you still need to hit them all in order. The backwards navigation is only for
optimizing the Walls.
*/
class Checkpoint
{
public:
	Checkpoint(Line line, Wall** collisionWallGrouping, int wallsInCollisionGroup, Wall** visionWallGrouping, int wallsInVisionGroup)
		: line(line), collisionWallGrouping(collisionWallGrouping), wallsInCollisionGroup(wallsInCollisionGroup), visionWallGrouping(visionWallGrouping), wallsInVisionGroup(wallsInVisionGroup) {}

	Wall** getCollisionWallGroup() { return collisionWallGrouping; }
	int getWallsInCollisionGroup() { return wallsInCollisionGroup; }

	Wall** getVisionWallGroup() { return collisionWallGrouping; }
	int getWallsInVisionGroup() { return wallsInCollisionGroup; }

	void setX1(TextFieldResult result) { line.x1 = result.floatResult; }
	void setY1(TextFieldResult result) { line.y1 = result.floatResult; }
	void setX2(TextFieldResult result) { line.x2 = result.floatResult; }
	void setY2(TextFieldResult result) { line.y2 = result.floatResult; }

	Line getLine() { return line; }
private:
	Wall** collisionWallGrouping;
	int wallsInCollisionGroup;

	Wall** visionWallGrouping;
	int wallsInVisionGroup;

	Line line;
};

/*
A Checkpoint that supports dynamic adding and removing of walls to the group. Used by the editor
*/
class EditorCheckpoint
{
public:
	EditorCheckpoint(Line line)
		: line(line) {}

	std::list<Wall*>& getCollisionWalls() { return collisionWalls; }
	std::list<Wall*>& getVisionWalls() { return visionWalls; }

	void addCollisionWall(Wall* add)
	{
		collisionWalls.push_back(add);
	}

	// Returns true if the wall was removed from the list, false if it couldn't be found
	bool removeCollisionWall(Wall* remove)
	{
		for (auto it = collisionWalls.begin(); it != collisionWalls.end(); it++)
		{
			if (*it == remove)
			{
				collisionWalls.erase(it);
				return true;
			}
		}
		return false;
	}

	bool containsCollisionWall(Wall* contains)
	{
		for (auto it = collisionWalls.begin(); it != collisionWalls.end(); it++)
		{
			if (*it == contains)
			{
				return true;
			}
		}
		return false;
	}

	void addVisionWall(Wall* add)
	{
		visionWalls.push_back(add);
	}

	// Returns true if the wall was removed from the list, false if it couldn't be found
	bool removeVisionWall(Wall* remove)
	{
		for (auto it = visionWalls.begin(); it != visionWalls.end(); it++)
		{
			if (*it == remove)
			{
				visionWalls.erase(it);
				return true;
			}
		}
		return false;
	}

	bool containsVisionWall(Wall* contains)
	{
		for (auto it = visionWalls.begin(); it != visionWalls.end(); it++)
		{
			if (*it == contains)
			{
				return true;
			}
		}
		return false;
	}

	void setX1(TextFieldResult result) { line.x1 = result.floatResult; }
	void setY1(TextFieldResult result) { line.y1 = result.floatResult; }
	void setX2(TextFieldResult result) { line.x2 = result.floatResult; }
	void setY2(TextFieldResult result) { line.y2 = result.floatResult; }

	Line getLine() { return line; }
private:
	std::list<Wall*> collisionWalls;
	std::list<Wall*> visionWalls;

	Line line;
};
