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

void Kart::tick()
{
	/* This physics is incredibly preliminary, this will be hugely updated in the future, it is only currently included
	as a base line to use for experimenting with other features (i.e. map collision) */

	input->update();

	const float currentVelocity = sqrtf(xVelocity * xVelocity + yVelocity * yVelocity);

	// approach the desired input
	const float desiredWheelAngle = input->getSteering() * KART_WHEEL_ANGLE_LIMIT;
	if (abs(wheelAngle - desiredWheelAngle) < KART_HANDLING)
	{
		wheelAngle = desiredWheelAngle;
	}
	else if (wheelAngle > desiredWheelAngle)
	{
		wheelAngle -= KART_HANDLING;
	}
	else
	{
		wheelAngle += KART_HANDLING;
	}

	const float turningMulti = sqrtf(currentVelocity) * (isMovingForward() ? 1.0f : -1.0f) * (isDrifting ? KART_DRIFT_TURN_MULTI : 1.0f) * KART_TURN_SPEED;
	angle += wheelAngle * turningMulti; // TODO: this causes problems, because turning can cause the karts corners to pass through walls

	const float movementMulti = input->getMovement() * (input->getMovement() < 0 ? 0.5f : 1.0f) *
		(currentVelocity > KART_SOFT_SPEED_CAP ? 1.0f - (currentVelocity - KART_SOFT_SPEED_CAP) * KART_SOFT_SPEED_REDUCTION : 1.0f) * KART_ACCLERATION;
	xVelocity += cosf(angle * DEG_TO_RAD) * movementMulti;
	yVelocity += sinf(angle * DEG_TO_RAD) * movementMulti;
}

void Kart::update()
{
	doCollisions();

	// apply velocitys
	xPosition += xVelocity;
	yPosition += yVelocity;

	// drag and friction
	xVelocity *= KART_DRAG;
	yVelocity *= KART_DRAG;

	doFriction();
}

void Kart::updateWallGroups(WallGroup* newWallGroups, int newWallGroupCount)
{
	wallGroupCount = newWallGroupCount;
	wallGroups = newWallGroups;
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

void Kart::doCollisions()
{
	// calculate points for the corners of the kart
	const float cosAngle = cosf(angle * DEG_TO_RAD);
	const float cosAngleWidth = cosAngle * KART_WIDTH / 2.0f;
	const float cosAngleLength = cosAngle * KART_LENGTH / 2.0f;

	const float sinAngle = cosf(angle * DEG_TO_RAD);
	const float sinAngleWidth = sinAngle * KART_WIDTH / 2.0f;
	const float sinAngleLength = sinAngle * KART_LENGTH / 2.0f;

	const float kartFLX = xPosition + sinAngleWidth + cosAngleLength;
	const float kartFLY = yPosition - cosAngleWidth + sinAngleLength;

	const float kartFRX = xPosition - sinAngleWidth + cosAngleLength;
	const float kartFRY = yPosition + cosAngleWidth + sinAngleLength;

	const float kartBLX = xPosition + sinAngleWidth - cosAngleLength;
	const float kartBLY = yPosition - cosAngleWidth - sinAngleLength;

	const float kartBRX = xPosition - sinAngleWidth - cosAngleLength;
	const float kartBRY = yPosition + cosAngleWidth - sinAngleLength;

	Corner corner;
	float intersectX;
	float intersectY;
	float time = 2.0f;

	for (int group = 0; group < wallGroupCount; group++)
	{
		for (int i = 0; i < wallGroups[group].count; i++)
		{
			Wall* wall = wallGroups[group].walls[i];

			if (getLineSegmentIntersection(kartFLX, kartFLY, kartFLX + xVelocity, kartFLY + yVelocity,
				wall->getX1(), wall->getY1(), wall->getX2(), wall->getY2(), &intersectX, &intersectY, &time))
			{
				corner = Corner::FrontLeft;
			}
			if (getLineSegmentIntersection(kartFRX, kartFRY, kartFRX + xVelocity, kartFRY + yVelocity,
				wall->getX1(), wall->getY1(), wall->getX2(), wall->getY2(), &intersectX, &intersectY, &time))
			{
				corner = Corner::FrontRight;
			}
			if (getLineSegmentIntersection(kartBLX, kartBLY, kartBLX + xVelocity, kartBLY + yVelocity,
				wall->getX1(), wall->getY1(), wall->getX2(), wall->getY2(), &intersectX, &intersectY, &time))
			{
				corner = Corner::BackLeft;
			}
			if (getLineSegmentIntersection(kartBRX, kartBRY, kartBRX + xVelocity, kartBRY + yVelocity,
				wall->getX1(), wall->getY1(), wall->getX2(), wall->getY2(), &intersectX, &intersectY, &time))
			{
				corner = Corner::BackRight;
			}
		}
	}

	if (time <= 1.0f)
	{
		body.setFillColor(sf::Color::Red); // temp debug
		xVelocity *= -0.5f;
		yVelocity *= -0.5f;
	}
	else
	{
		body.setFillColor(sf::Color::Yellow); // temp debug
	}
}

bool Kart::getLineSegmentIntersection(float kx1, float ky1, float kx2, float ky2, float wx1, float wy1, float wx2, float wy2, float* ox, float* oy, float* otime)
{
	// the k's refer to the kart, the w's refer to the wall or whatever else is being checked against, the o's are the out's and receive values in the event of a collision
	// to use this in a raycast scenario, have k1 be the starting position, and k2 be the next position
	float lineKX = kx2 - kx1;
	float lineKY = ky2 - ky1;
	float lineWX = wx2 - wx1;
	float lineWY = wy2 - wy1;

	float denominator = lineKX * lineWY - lineKY * lineWX;
	if (denominator == 0.0f) // lines are parallel, and therefore don't intersect
	{
		return false;
	}
	float kFraction = (lineKX * (ky1 - wy1) - lineKY * (kx1 - wx1)) / denominator; // aka time
	float wFraction = (lineWX * (ky1 - wy1) - lineWY * (kx1 - wx1)) / denominator;

	if (kFraction >= 0 && kFraction <= 1 && wFraction >= 0 && wFraction <= 1 && kFraction < *otime)
	{
		// only overwrite the time if this one is shorter
		*otime = kFraction;
		*ox = kx1 + kFraction * lineKX;
		*oy = ky1 + kFraction * lineKY;

		return true; // only return true if this is the first collision (kFraction < *otime)
	}

	return false;
}
