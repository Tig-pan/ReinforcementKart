#include "kart.h"

Kart::Kart(std::string racerName, Input* input, sf::Color kartColor, float startX, float startY, float startAngle)
	: body(sf::Vector2f(KART_LENGTH, KART_WIDTH)) // the x component is the length, and the y is width. this is because the angle 0 is straight to the right
	, wheel(sf::Vector2f(KART_WHEEL_LENGTH, KART_WHEEL_WIDTH))

	, input(input)

	, xPosition(startX)
	, yPosition(startY)
	, angle(startAngle)

	, wheelAngle(0.0f)
	
	, xVelocity(0.0f)
	, yVelocity(0.0f)
	, angularVelocity(0.0f)

	, isDrifting(false)
{
	body.setFillColor(kartColor);
	body.setOrigin(KART_LENGTH / 2.0f, KART_WIDTH / 2.0f); // position is now measured relative to the center of the back wheels
	wheel.setFillColor(WHEEL_COLOR);
	wheel.setOrigin(KART_WHEEL_LENGTH / 2.0f, KART_WHEEL_WIDTH / 2.0f);
}

void Kart::update()
{
	/* This physics is incredibly preliminary, this will be hugely updated in the future, it is only currently included 
	as a base line to use for experimenting with other features (i.e. map collision) */

	input->update();

	const float currentVelocity = sqrtf(xVelocity * xVelocity + yVelocity * yVelocity);
	std::cout << currentVelocity << "\n";
	// approach the desired input
	const float desiredWheelAngle = input->getSteering() * KART_WHEEL_ANGLE_LIMIT;
	if (abs(wheelAngle - desiredWheelAngle) < 2.5f)
	{
		wheelAngle = desiredWheelAngle;
	}
	else if (wheelAngle > desiredWheelAngle)
	{
		wheelAngle -= 2.5f;
	}
	else
	{
		wheelAngle += 2.5f;
	}

	const float turningMulti = sqrtf(currentVelocity) * (isMovingForward() ? 1.0f : -1.0f) * (isDrifting ? KART_DRIFT_TURN_MULTI : 1.0f) * KART_TURN_SPEED;
	angle += wheelAngle * turningMulti;

	const float movementMulti = input->getMovement() * (input->getMovement() < 0 ? 0.5f : 1.0f) * (currentVelocity > KART_SOFT_SPEED_CAP ? KART_SOFT_SPEED_REDUCTION : 1.0f) * KART_ACCLERATION;
	xVelocity += cosf(angle * DEG_TO_RAD) * movementMulti;
	yVelocity += sinf(angle * DEG_TO_RAD) * movementMulti;

	// apply velocitys
	xPosition += xVelocity;
	yPosition += yVelocity;

	// drag and friction
	xVelocity *= KART_DRAG;
	yVelocity *= KART_DRAG;

	doFriction();
}

void Kart::render(sf::RenderWindow& window)
{
	/* Render the front wheels first, then the back wheels, then the body */

	// a very priliminary way of visually showing when you are drifting, ideally it would be shown with skidmarks or something, but for now changing the wheel color is ok
	if (isDrifting)
	{
		wheel.setFillColor(sf::Color::White);
	}
	else
	{
		wheel.setFillColor(WHEEL_COLOR);
	}

	wheel.setRotation(angle + wheelAngle);

	wheel.setPosition(sf::Vector2f(
		xPosition + (sinf(angle * DEG_TO_RAD) * KART_WIDTH / 2.0f) + (cosf(angle * DEG_TO_RAD) * KART_LENGTH / 4.0f),
		yPosition - (cosf(angle * DEG_TO_RAD) * KART_WIDTH / 2.0f) + (sinf(angle * DEG_TO_RAD) * KART_LENGTH / 4.0f))); // front left wheel
	window.draw(wheel);
	wheel.setPosition(sf::Vector2f(
		xPosition - (sinf(angle * DEG_TO_RAD) * KART_WIDTH / 2.0f) + (cosf(angle * DEG_TO_RAD) * KART_LENGTH / 4.0f),
		yPosition + (cosf(angle * DEG_TO_RAD) * KART_WIDTH / 2.0f) + (sinf(angle * DEG_TO_RAD) * KART_LENGTH / 4.0f))); // front right wheel
	window.draw(wheel);

	wheel.setRotation(angle);

	wheel.setPosition(sf::Vector2f(
		xPosition + (sinf(angle * DEG_TO_RAD) * KART_WIDTH / 2.0f) - (cosf(angle * DEG_TO_RAD) * KART_LENGTH / 4.0f),
		yPosition - (cosf(angle * DEG_TO_RAD) * KART_WIDTH / 2.0f) - (sinf(angle * DEG_TO_RAD) * KART_LENGTH / 4.0f))); // back left wheel
	window.draw(wheel);
	wheel.setPosition(sf::Vector2f(
		xPosition - (sinf(angle * DEG_TO_RAD) * KART_WIDTH / 2.0f) - (cosf(angle * DEG_TO_RAD) * KART_LENGTH / 4.0f),
		yPosition + (cosf(angle * DEG_TO_RAD) * KART_WIDTH / 2.0f) - (sinf(angle * DEG_TO_RAD) * KART_LENGTH / 4.0f))); // back right wheel
	window.draw(wheel);
	
	// now render the body
	body.setRotation(angle);
	body.setPosition(sf::Vector2f(xPosition, yPosition));

	window.draw(body);
}

float Kart::getMovementAngle(float usedAngle)
{
	const float velocityMagnitude = sqrtf(xVelocity * xVelocity + yVelocity * yVelocity);
	if (velocityMagnitude < 0.01f) // essentially zero
	{
		return 0.0f;
	}

	const float dot = xVelocity * cosf(usedAngle * DEG_TO_RAD) + yVelocity * sinf(usedAngle * DEG_TO_RAD);
	if (dot == velocityMagnitude) // this is mainly because acosf kinda freaks out when given exactly 1
	{
		return 0.0f;
	}
	return acosf(dot / velocityMagnitude);
}

bool Kart::isMovingForward()
{
	// calculates if its moving backward by finding the angle between its velocity and its forward trajectory
	return getMovementAngle(angle - wheelAngle) < (90.0f * DEG_TO_RAD);
}

void Kart::doFriction()
{
	// change the velocity towards the velocitys projection to the forward direction of the vehicle
	const float dot = xVelocity * cosf(angle * DEG_TO_RAD) + yVelocity * sinf(angle * DEG_TO_RAD);

	float xChange = xVelocity - cosf(angle * DEG_TO_RAD) * dot;
	float yChange = yVelocity - sinf(angle * DEG_TO_RAD) * dot;

	// drifting is decided based the euclidian changes
	if (isDrifting && sqrt(xChange * xChange + yChange * yChange) < (input->getDrift() ? KART_DRIFT_STOP_DRIFT : KART_BASE_STOP_DRIFT))
	{
		isDrifting = false;
	}
	else if (!isDrifting && abs(xChange * xChange + yChange * yChange) > (input->getDrift() ? KART_DRIFT_START_DRIFT : KART_BASE_START_DRIFT))
	{
		isDrifting = true;
	}

	// cap the change based on if the kart is drifting or not
	const float maxChange = isDrifting ? KART_DRIFT_MAX_CHANGE : KART_BASE_MAX_CHANGE;

	if (xChange > maxChange)
	{
		xChange = maxChange;
	}
	else if (xChange < -maxChange)
	{
		xChange = -maxChange;
	}

	if (yChange > maxChange)
	{
		yChange = maxChange;
	}
	else if (yChange < -maxChange)
	{
		yChange = -maxChange;
	}

	const float starterMagnitude = sqrtf(xVelocity * xVelocity + yVelocity * yVelocity);

	// update the velocities
	xVelocity -= xChange;
	yVelocity -= yChange;

	const float afterMagnitude = sqrtf(xVelocity * xVelocity + yVelocity * yVelocity);

	if (afterMagnitude > starterMagnitude)
	{
		xVelocity *= (starterMagnitude / afterMagnitude);
		yVelocity *= (starterMagnitude / afterMagnitude);
	}
}