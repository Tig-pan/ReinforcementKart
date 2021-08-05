#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <fstream>

#include "input.h"
#include "wall.h"
#include "checkpoint.h"
#include "kart.h"

/*
Runs the actual playable part of Reinforcement Kart. Loads a map and allows the Input to drive the kart around and complete laps of the given track.
*/
class Game
{
public:
	/* A constuctor for Game, executes playing a map, and allows the Input to control the kart driving around the track.

	mapFileName: the name of the file to read as the track
	input: a pointer to the input object that will control the kart
	window: the sf::RenderWindow& window to render the game onto
	font: the sf::Font to use for all GUI text*/
	Game(std::string mapFileName, Input* input, sf::RenderWindow& window, sf::Font& font);

	// Runs indefinatly until the game is closed, or the window is closed
	void run();

private:
	sf::RenderWindow& window;
	sf::Font& font;

	sf::View guiView;
	sf::View gameView;

	Kart* kart;
	Input* input;

	float startAngle;
	int wallCount;
	Wall** walls; // array of wall pointers
	int checkpointCount;
	Checkpoint** checkpoints; // array of checkpoint pointers
};