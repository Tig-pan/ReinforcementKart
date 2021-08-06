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
	Checkpoint(Line line, Wall** wallGrouping, int wallsInGroup)
		: line(line), wallGrouping(wallGrouping), wallsInGroup(wallsInGroup) {}

	Wall** getWallGroup() { return wallGrouping; }
	int getWallsInGroup() { return wallsInGroup; }

	void setX1(TextFieldResult result) { line.x1 = result.floatResult; }
	void setY1(TextFieldResult result) { line.y1 = result.floatResult; }
	void setX2(TextFieldResult result) { line.x2 = result.floatResult; }
	void setY2(TextFieldResult result) { line.y2 = result.floatResult; }

	Line getLine() { return line; }
private:
	Wall** wallGrouping;
	int wallsInGroup;

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

	std::list<Wall*>& getWalls() { return walls; }

	void addWall(Wall* add)
	{
		walls.push_back(add);
	}

	// Returns true if the wall was removed from the list, false if it couldn't be found
	bool removeWall(Wall* remove)
	{
		for (auto it = walls.begin(); it != walls.end(); it++)
		{
			if (*it == remove)
			{
				walls.erase(it);
				return true;
			}
		}
		return false;
	}

	bool containsWall(Wall* contains)
	{
		for (auto it = walls.begin(); it != walls.end(); it++)
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
	std::list<Wall*> walls;

	Line line;
};
