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

	const float movementMulti = input->getMovement() * (input->getMovement() < 0 ? 0.5f : 1.0f) * 
		(currentVelocity > KART_SOFT_SPEED_CAP ? 1.0f - (currentVelocity - KART_SOFT_SPEED_CAP) * KART_SOFT_SPEED_REDUCTION : 1.0f) * KART_ACCLERATION;
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

	const float changeMagnitude = sqrt(xChange * xChange + yChange * yChange);

	// drifting is decided based the euclidian changes
	if (isDrifting && changeMagnitude < (input->getDrift() ? KART_DRIFT_STOP_DRIFT : KART_BASE_STOP_DRIFT))
	{
		isDrifting = false;
	}
	else if (!isDrifting && changeMagnitude > (input->getDrift() ? KART_DRIFT_START_DRIFT : KART_BASE_START_DRIFT))
	{
		isDrifting = true;
	}

	// cap the change based on if the kart is drifting or not
	const float maxChange = isDrifting ? KART_DRIFT_MAX_CHANGE : KART_BASE_MAX_CHANGE;

	if (changeMagnitude > maxChange)
	{
		xChange = (xChange / changeMagnitude) * maxChange;
		yChange = (yChange / changeMagnitude) * maxChange;
	}

	// update the velocities
	xVelocity -= xChange;
	yVelocity -= yChange;
}

bool Kart::intersectingWithWall(Wall& wall, sf::RenderWindow& debug)
{
	// calculate points for the corners of the kart
	float kartFLX = xPosition + (sinf(angle * DEG_TO_RAD) * KART_WIDTH / 2.0f) + (cosf(angle * DEG_TO_RAD) * KART_LENGTH / 2.0f);
	float kartFLY = yPosition - (cosf(angle * DEG_TO_RAD) * KART_WIDTH / 2.0f) + (sinf(angle * DEG_TO_RAD) * KART_LENGTH / 2.0f);

	float kartFRX = xPosition - (sinf(angle * DEG_TO_RAD) * KART_WIDTH / 2.0f) + (cosf(angle * DEG_TO_RAD) * KART_LENGTH / 2.0f);
	float kartFRY = yPosition + (cosf(angle * DEG_TO_RAD) * KART_WIDTH / 2.0f) + (sinf(angle * DEG_TO_RAD) * KART_LENGTH / 2.0f);

	float kartBLX = xPosition + (sinf(angle * DEG_TO_RAD) * KART_WIDTH / 2.0f) - (cosf(angle * DEG_TO_RAD) * KART_LENGTH / 2.0f);
	float kartBLY = yPosition - (cosf(angle * DEG_TO_RAD) * KART_WIDTH / 2.0f) - (sinf(angle * DEG_TO_RAD) * KART_LENGTH / 2.0f);

	float kartBRX = xPosition - (sinf(angle * DEG_TO_RAD) * KART_WIDTH / 2.0f) - (cosf(angle * DEG_TO_RAD) * KART_LENGTH / 2.0f);
	float kartBRY = yPosition + (cosf(angle * DEG_TO_RAD) * KART_WIDTH / 2.0f) - (sinf(angle * DEG_TO_RAD) * KART_LENGTH / 2.0f);

	float intersectX;
	float intersectY;

	// then test if any of those 4 lines made from connecting the corners of the kart collide with the wall
	if (getLineSegmentIntersection(kartFLX, kartFLY, kartFRX, kartFRY, wall.getX1(), wall.getY1(), wall.getX2(), wall.getY2(), &intersectX, &intersectY) ||
		getLineSegmentIntersection(kartFRX, kartFRY, kartBRX, kartBRY, wall.getX1(), wall.getY1(), wall.getX2(), wall.getY2(), &intersectX, &intersectY) ||
		getLineSegmentIntersection(kartBRX, kartBRY, kartBLX, kartBLY, wall.getX1(), wall.getY1(), wall.getX2(), wall.getY2(), &intersectX, &intersectY) ||
		getLineSegmentIntersection(kartBLX, kartBLY, kartFLX, kartFLY, wall.getX1(), wall.getY1(), wall.getX2(), wall.getY2(), &intersectX, &intersectY))
	{
		body.setFillColor(sf::Color::Red); // temp debug
		return true;
	}

	body.setFillColor(sf::Color::Yellow); // temp debug
	return false;
}

bool Kart::getLineSegmentIntersection(float s1x1, float s1y1, float s1x2, float s1y2, float s2x1, float s2y1, float s2x2, float s2y2, float* ox, float* oy)
{
	float lineAX = s1x2 - s1x1;
	float lineAY = s1y2 - s1y1;
	float lineBX = s2x2 - s2x1;
	float lineBY = s2y2 - s2y1;

	float denominator = lineAX * lineBY - lineBX * lineAY;
	if (denominator == 0.0f) // lines are parallel, and therefore don't intersect
	{
		return false;
	}
	float xFraction = (lineAX * (s1y1 - s2y1) - lineAY * (s1x1 - s2x1)) / denominator;
	float yFraction = (lineBX * (s1y1 - s2y1) - lineBY * (s1x1 - s2x1)) / denominator;

	if (xFraction >= 0 && xFraction <= 1 && yFraction >= 0 && yFraction <= 1)
	{
		*ox = s1x1 + xFraction * lineAX;
		*oy = s1y1 + yFraction * lineAY;

		return true;
	}

	return false;
}
