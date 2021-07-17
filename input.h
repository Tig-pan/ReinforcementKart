#pragma once

#include <SFML/Window.hpp>

/*
An abstract class used to represent a method of giving input, this may be human input from a keyboard, or from an AI
*/
class Input abstract
{
public:
	// Called every frame, allows the Input to update its values however nessesary (ex. from a keyboard or from an AI)
	virtual void update() = 0;


	// Ranges from -1 to 1, -1 being full steering left, 1 being full steering right
	virtual float getSteering() = 0;

	// Ranges from -1 to 1, -1 being full reverse, 1 being full accelerate
	virtual float getMovement() = 0;

	// true if the key to start drifting is pressed, false otherwise
	virtual bool getDrift() = 0;

	// true if the key to use an item is pressed, false otherwise
	virtual bool getAction() = 0;
};

/*
An Input for KeyboardInput, this type of input is executed by a human using a keyboard. The default controls are:
	Left/a			-> steer left
	Right/d			-> steer right
	Down/s			-> brake
	Up/w			-> accelerate
	LShift/RShift	-> drift
	Space Bar		-> item action
*/
class KeyboardInput : public Input
{
public:
	// A constructor for KeyboardInput, takes no arguments
	KeyboardInput();

	void update() override;

	float getSteering() override;
	float getMovement() override;
	bool getDrift() override;
	bool getAction() override;
private:
	float currentSteer;
	float currentMovement;
	bool currentDrift;
	bool currentAction;
	bool isActionHeld;
};