#include "input.h"

KeyboardInput::KeyboardInput()
	: currentSteer(0.0f)
	, currentMovement(0.0f)
	, currentDrift(false)
	, currentAction(false)
	, isActionHeld(false)
{

}

void KeyboardInput::update()
{
	/* The car will have a method to slowly turn the wheels to the desired location, so we can set
	the desired steering to extreme values without consequence */

	const bool leftSteerPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A);
	const bool rightSteerPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D);

	if (leftSteerPressed == rightSteerPressed) // if they are either both pressed or none of them pressed, then set the steer to 0
	{
		currentSteer = 0.0f;
	}
	else if (rightSteerPressed)
	{
		currentSteer = 1.0f;
	}
	else
	{
		currentSteer = -1.0f;
	}

	/* Movement works similarly to steering, it can be set to extreme values because it is meerly an acceleration */

	const bool brakePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S);
	const bool accleratePressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W);

	if (brakePressed == accleratePressed) // if they are either both pressed or none of them pressed, then set the movement to 0
	{
		currentMovement = 0.0f;
	}
	else if (accleratePressed)
	{
		currentMovement = 1.0f;
	}
	else
	{
		currentMovement = -1.0f;
	}

	/* Drifting is just checking if its specific keys are currently pressed, as it can be held for different amounts for different effects */

	currentDrift = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift);

	/* Action is slightly different, only the first frame when it is pressed should the action be acknowleged. */

	bool action = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space);

	currentAction = !isActionHeld && action;
	isActionHeld = action;
}

float KeyboardInput::getSteering()
{
	return currentSteer;
}

float KeyboardInput::getMovement()
{
	return currentMovement;
}

bool KeyboardInput::getDrift()
{
	return currentDrift;
}

bool KeyboardInput::getAction()
{
	return currentAction;
}
