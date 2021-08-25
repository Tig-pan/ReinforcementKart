#pragma once

#include <SFML/Window.hpp>

#include <random>

#include "kartSensors.h"
#include "network.h"

/*
An abstract class used to represent a method of giving input, this may be human input from a keyboard, or from an AI
*/
class Input abstract
{
public:
	// Called after the kart position is reset, typically used by reinforcement bots to not train for the previous action taken
	virtual void reset() = 0;

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

	void reset() { }
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

/*
An incredibly simple bot that meerly drives forward, and steers/accelerates to avoid crashing
*/
class AvoidanceAI : public Input
{
public:
	// A constructor for AvoidanceAI, takes a KartSensors class as an arguement
	AvoidanceAI(KartSensors* sensor);

	void reset() { }
	void update() override;

	float getSteering() override;
	float getMovement() override;
	bool getDrift() override;
	bool getAction() override;
private:
	KartSensors* sensor;

	float currentSteer;
	float currentMovement;

	int currentDecision;
	
	bool isBackingUp;
	bool currentDrift;
};

/*
An AI that trains using Deep Q reinforcement learning in order to learn how to control the kart and maximize the track speed.
*/
class DeepQLearningAI : public Input
{
public:
	// A constructor for DeepQLearningAI, takes a KartSensors class as an arguement
	DeepQLearningAI(KartSensors* sensor);

	void reset() override;
	void update() override;

	float getSteering() override;
	float getMovement() override;
	bool getDrift() override;
	bool getAction() override;
private:
	const static int MEMORY_TRAINS = 9;
	const static int MEMORY_SIZE = 2000;
	const static int INPUT_SIZE = 27;
	const static int ACTION_COUNT = 9; // 13 with drifts

	// Fills up the given state array with the appropriate measured values
	void fillStateArr(float* arr);

	// Executes the action according to the selection action
	void executeAction(int action);

	/* Trains the internal Q network based on the given statesand reward
	also updates the currentActionIndex variable */
	void train(float* previousState, float* currentState, int actionIndex, float reward);

	KartSensors* sensor;

	Network* qNetwork; // inputs = sensor data | outputs = Q value for each action

	Softmax* softmax; // used to softmax the action q values to pick a random one

	std::default_random_engine generator;
	std::uniform_real<float> distribution;

	float* actionExpectedQs;
	float* stateArrA;
	float* stateArrB;
	bool previousIsA;

	float* memoryBeforeStates;
	float* memoryAfterStates;
	int* memoryActions;
	float* memoryRewards;
	int currentMemoryIndex;
	int nextMemoryIndex;
	bool isMemoryFull;

	int currentActionIndex;
	int previousActionIndex;

	float discountFactor;

	float currentSteer;
	float currentMovement;
	bool currentDrift;

	bool justFailed;
};