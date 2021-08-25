#include "input.h"

DeepQLearningAI::DeepQLearningAI(KartSensors* sensor)
	: sensor(sensor)
	, generator()
	, distribution(0.0f, 1.0f)
{
	const int layerCount = 3;

	Layer** layers = new Layer*[layerCount];
	layers[0] = new DenseLayer(INPUT_SIZE, 60, new Sigmoid());
	layers[1] = new DenseLayer(60, 40, new Sigmoid());
	layers[2] = new DenseLayer(40, ACTION_COUNT, new Linear());

	qNetwork = new Network(Loss::MeanSquaredError,
		0.001f, // learning rate
		0.00001f, // weight decay
		layers,
		layerCount);

	softmax = new Softmax();

	actionExpectedQs = new float[ACTION_COUNT];
	stateArrA = new float[INPUT_SIZE];
	stateArrB = new float[INPUT_SIZE];
	previousIsA = false;

	memoryBeforeStates = new float[MEMORY_SIZE * INPUT_SIZE];
	memoryAfterStates = new float[MEMORY_SIZE * INPUT_SIZE];
	memoryActions = new int[MEMORY_SIZE];
	memoryRewards = new float[MEMORY_SIZE];
	currentMemoryIndex = -1;
	isMemoryFull = false;

	previousActionIndex = -1; // -1 means no action was taken

	discountFactor = 0.9f;

	justFailed = false;
}

void DeepQLearningAI::reset()
{
	justFailed = true;
}

void DeepQLearningAI::update()
{
	// Get the current reward for this tick based on the previous action
	float reward;
	if (justFailed)
	{
		reward = -150.0f;
		justFailed = false;
	}
	else
	{
		reward = sensor->getProgressMade() * 1000.0f;
	}

	// Makes variables for quick access to the current state and the previous state
	float* currentState = previousIsA ? stateArrB : stateArrA;
	float* previousState = previousIsA ? stateArrA : stateArrB;

	// Gets the current state from sensor data
	fillStateArr(currentState);

	if (isMemoryFull)
	{
		for (int i = 0; i < MEMORY_TRAINS; i++)
		{
			int index = generator() % MEMORY_SIZE;
			while (index != currentMemoryIndex && index != nextMemoryIndex)
			{
				index = generator() % MEMORY_SIZE;
			}
			train(&memoryBeforeStates[index * INPUT_SIZE], &memoryAfterStates[index * INPUT_SIZE], memoryActions[index], memoryRewards[index]);
		}
	}
	train(previousState, currentState, previousActionIndex, reward);

	qNetwork->apply();

	// Set the relevant memory
	if (currentMemoryIndex != -1)
	{
		if (isMemoryFull)
		{
			nextMemoryIndex = generator() % MEMORY_SIZE;
		}
		else
		{
			if (currentMemoryIndex == MEMORY_SIZE - 1)
			{
				isMemoryFull = true;
				nextMemoryIndex = generator() % MEMORY_SIZE;
			}
			else
			{
				nextMemoryIndex = currentMemoryIndex + 1;
			}
		}

		int currentMemoryOffset = currentMemoryIndex * INPUT_SIZE;
		int nextMemoryOffset = nextMemoryIndex * INPUT_SIZE;

		memoryRewards[currentMemoryIndex] = reward;
		memoryActions[nextMemoryIndex] = currentActionIndex;
		for (int i = 0; i < INPUT_SIZE; i++)
		{
			memoryAfterStates[currentMemoryOffset + i] = currentState[i];
			memoryBeforeStates[nextMemoryOffset + i] = currentState[i];
		}

		currentMemoryIndex = nextMemoryIndex;
	}
	else
	{
		memoryActions[0] = currentActionIndex;
		for (int i = 0; i < INPUT_SIZE; i++)
		{
			memoryBeforeStates[i] = currentState[i];
		}
		currentMemoryIndex = 0;
	}

	// Executes the action and sets previous variables
	executeAction(currentActionIndex);
	previousActionIndex = currentActionIndex;
	previousIsA = !previousIsA;
}

float DeepQLearningAI::getSteering()
{
	return currentSteer;
}

float DeepQLearningAI::getMovement()
{
	return currentMovement;
}

bool DeepQLearningAI::getDrift()
{
	return currentDrift;
}

bool DeepQLearningAI::getAction()
{
	return false;
}

void DeepQLearningAI::fillStateArr(float* arr)
{
	const int DISTANCE = 500;

	arr[0] = (sensor->getAngleToNextCheckpoint() * RAD_TO_DEG) / 90.0f; // Range: (-1, 1)

	sensor->raycastRelativeAngle(0.5f, 0.0f, 0, DISTANCE); // forward
	arr[1] = 1.0f - sensor->getOutputTime();
	sensor->raycastRelativeAngle(0.5f, 1.0f, 180, DISTANCE); // backward
	arr[2] = 1.0f - sensor->getOutputTime();

	sensor->raycastRelativeAngle(0.0f, 0.0f, -2, DISTANCE); // slight left
	arr[3] = 1.0f - sensor->getOutputTime();
	sensor->raycastRelativeAngle(1.0f, 0.0f, 2, DISTANCE); // slight right
	arr[4] = 1.0f - sensor->getOutputTime();

	sensor->raycastRelativeAngle(0.5f, 0.0f, 0, DISTANCE / 3.0f); // close forward
	arr[5] = 1.0f - sensor->getOutputTime();
	sensor->raycastRelativeAngle(0.0f, 0.0f, -2, DISTANCE / 3.0f); // close slight left
	arr[6] = 1.0f - sensor->getOutputTime();
	sensor->raycastRelativeAngle(1.0f, 0.0f, 2, DISTANCE / 3.0f); // close slight right
	arr[7] = 1.0f - sensor->getOutputTime();

	sensor->raycastRelativeAngle(0.0f, 0.0f, -7.5f, DISTANCE); // tiny left
	arr[8] = 1.0f - sensor->getOutputTime();
	sensor->raycastRelativeAngle(0.0f, 0.0f, -15, DISTANCE); // left
	arr[9] = 1.0f - sensor->getOutputTime();
	sensor->raycastRelativeAngle(0.0f, 0.0f, -30, DISTANCE); // left x2
	arr[10] = 1.0f - sensor->getOutputTime();
	sensor->raycastRelativeAngle(0.0f, 0.0f, -50, DISTANCE); // left x3
	arr[11] = 1.0f - sensor->getOutputTime();
	sensor->raycastRelativeAngle(0.0f, 0.5f, -70, DISTANCE); // far left
	arr[12] = 1.0f - sensor->getOutputTime();
	sensor->raycastRelativeAngle(0.0f, 0.5f, -90, DISTANCE); // far left x3
	arr[13] = 1.0f - sensor->getOutputTime();
	sensor->raycastRelativeAngle(0.0f, 0.5f, -120, DISTANCE); // far left x5
	arr[14] = 1.0f - sensor->getOutputTime();
	sensor->raycastRelativeAngle(0.0f, 1.0f, -155, DISTANCE); // back left x2
	arr[15] = 1.0f - sensor->getOutputTime();

	sensor->raycastRelativeAngle(1.0f, 0.0f, 7.5f, DISTANCE); // right
	arr[16] = 1.0f - sensor->getOutputTime();
	sensor->raycastRelativeAngle(1.0f, 0.0f, 15, DISTANCE); // right
	arr[17] = 1.0f - sensor->getOutputTime();
	sensor->raycastRelativeAngle(1.0f, 0.0f, 30, DISTANCE); // right x2
	arr[18] = 1.0f - sensor->getOutputTime();
	sensor->raycastRelativeAngle(1.0f, 0.0f, 50, DISTANCE); // right x3
	arr[19] = 1.0f - sensor->getOutputTime();
	sensor->raycastRelativeAngle(1.0f, 0.5f, 70, DISTANCE); // far right
	arr[20] = 1.0f - sensor->getOutputTime();
	sensor->raycastRelativeAngle(1.0f, 0.5f, 90, DISTANCE); // far right x3
	arr[21] = 1.0f - sensor->getOutputTime();
	sensor->raycastRelativeAngle(1.0f, 0.5f, 120, DISTANCE); // far right x5
	arr[22] = 1.0f - sensor->getOutputTime();
	sensor->raycastRelativeAngle(1.0f, 1.0f, 155, DISTANCE); // back right
	arr[23] = 1.0f - sensor->getOutputTime();

	arr[24] = sensor->getForwardVelocity() / 4.0f; // forward component of velocity
	arr[25] = sensor->getRightwardVelocity() / 4.0f; // rightward component of velocity
	arr[26] = sensor->getIsDrifting() ? 1.0f : 0.0f; // is drifting
}

void DeepQLearningAI::executeAction(int action)
{
	switch (action)
	{
	case 0: // idle
		currentMovement = 0.0f;
		currentSteer = 0.0f;
		currentDrift = false;
		break;
	case 1: // forward
		currentMovement = 1.0f;
		currentSteer = 0.0f;
		currentDrift = false;
		break;
	case 2: // backward
		currentMovement = -1.0f;
		currentSteer = 0.0f;
		currentDrift = false;
		break;
	case 3: // right
		currentMovement = 0.0f;
		currentSteer = 1.0f;
		currentDrift = false;
		break;
	case 4: // left
		currentMovement = 0.0f;
		currentSteer = -1.0f;
		currentDrift = false;
		break;
	case 5: // forward right
		currentMovement = 1.0f;
		currentSteer = 1.0f;
		currentDrift = false;
		break;
	case 6: // forward left
		currentMovement = 1.0f;
		currentSteer = -1.0f;
		currentDrift = false;
		break;
	case 7: // backward right
		currentMovement = -1.0f;
		currentSteer = 1.0f;
		currentDrift = false;
		break;
	case 8: // backward left
		currentMovement = -1.0f;
		currentSteer = -1.0f;
		currentDrift = false;
		break;
	/*
	case 9: // right drift
		currentMovement = 0.0f;
		currentSteer = 1.0f;
		currentDrift = true;
		break;
	case 10: // left drift
		currentMovement = 0.0f;
		currentSteer = -1.0f;
		currentDrift = true;
		break;
	case 11: // forward right drift
		currentMovement = 1.0f;
		currentSteer = 1.0f;
		currentDrift = true;
		break;
	case 12: // forward left drift
		currentMovement = 1.0f;
		currentSteer = -1.0f;
		currentDrift = true;
		break;*/
	}
}

void DeepQLearningAI::train(float* previousState, float* currentState, int actionIndex, float reward)
{
	// Get the Q values for all possible actions in the current state
	float* actionQValues = qNetwork->predict(currentState);

	float maxPossibleQ = actionQValues[0];
	for (int i = 1; i < ACTION_COUNT; i++)
	{
		if (actionQValues[i] > maxPossibleQ)
		{
			maxPossibleQ = actionQValues[i];
		}
	}

	/*std::cout << "a\n";
	if (currentState == stateArrA || currentState == stateArrB)
		std::cout << maxPossibleQ << "\n";*/

	// Pick a random action based on the softmax of the actionValues
	softmax->activate(actionQValues, ACTION_COUNT);
	float decision = distribution(generator);
	for (currentActionIndex = 0; decision > 0.0f; currentActionIndex++)
	{
		decision -= actionQValues[currentActionIndex];
	}
	currentActionIndex--;

	// Train based on maxPossibleQ and the current reward
	if (actionIndex != -1)
	{
		for (int i = 0; i < ACTION_COUNT; i++)
		{
			if (i == actionIndex) // only trains for the action selected
			{
				actionExpectedQs[i] = reward + discountFactor * maxPossibleQ;
			}
			else
			{
				actionExpectedQs[i] = NAN;
			}
		}
		qNetwork->train(previousState, actionExpectedQs);
	}
}
