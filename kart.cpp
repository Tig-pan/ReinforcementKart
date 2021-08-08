#include "kart.h"

Kart::Kart(std::string racerName, Input* input, KartSensors* sensors, sf::Color kartColor, float startX, float startY, float startAngle, Checkpoint** race, int checkpointCount)
	: body(sf::Vector2f(KART_LENGTH, KART_WIDTH)) // the x component is the length, and the y is width. this is because the angle 0 is straight to the right
	, wheel(sf::Vector2f(KART_WHEEL_LENGTH, KART_WHEEL_WIDTH))

	, input(input)
	, sensors(sensors)
	, race(race)

	, checkpointCount(checkpointCount)
	, currentCheckpoint(0)
	, furthestCheckpoint(0)
	, lapNumber(0)

	, checkpointProgress(0.0f)

	, xPosition(startX)
	, yPosition(startY)
	, angle(startAngle)

	, wheelAngle(0.0f)
	
	, xVelocity(0.0f)
	, yVelocity(0.0f)
	, angularVelocity(0.0f)
	, kartSpin(0.0f)

	, isDrifting(false)
{
	body.setFillColor(kartColor);
	body.setOrigin(KART_LENGTH / 2.0f, KART_WIDTH / 2.0f); // position is now measured relative to the middle of the kart
	wheel.setFillColor(WHEEL_COLOR);
	wheel.setOrigin(KART_WHEEL_LENGTH / 2.0f, KART_WHEEL_WIDTH / 2.0f);
}

void Kart::tick()
{
	/* This physics is incredibly preliminary, this will be hugely updated in the future, it is only currently included
	as a base line to use for experimenting with other features (i.e. map collision) */
	
	if (sensors != nullptr)
	{
		sensors->tick(xPosition, yPosition, angle, race[(currentCheckpoint + 1) % checkpointCount]->getLine(), race[currentCheckpoint]->getVisionWallGroup(), race[currentCheckpoint]->getWallsInVisionGroup());
	}

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
	angularVelocity = wheelAngle * turningMulti; // TODO: this causes problems, because turning can cause the karts corners to pass through walls

	const float movementMulti = input->getMovement() * (input->getMovement() < 0 ? 0.5f : 1.0f) *
		(currentVelocity > KART_SOFT_SPEED_CAP ? 1.0f - (currentVelocity - KART_SOFT_SPEED_CAP) * KART_SOFT_SPEED_REDUCTION : 1.0f) * KART_ACCLERATION;
	xVelocity += cosf(angle * DEG_TO_RAD) * movementMulti;
	yVelocity += sinf(angle * DEG_TO_RAD) * movementMulti;
}

void Kart::update()
{
	doCollisions();

	float xPositionBefore = xPosition;
	float yPositionBefore = yPosition;

	// apply velocitys
	angle += angularVelocity + kartSpin;
	xPosition += xVelocity;
	yPosition += yVelocity;

	doCheckpoints(xPositionBefore, yPositionBefore);
	// to calculate checkpoint progress, find the closest point on both checkpoint lines, then do a ratio

	// drag and friction
	xVelocity *= KART_DRAG;
	yVelocity *= KART_DRAG;

	kartSpin *= KART_SPIN_FALLOFF;

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

	const float cosAngle = cosf(angle * DEG_TO_RAD);
	const float cosAngleWidth = cosAngle * (KART_WIDTH / 2.0f);
	const float cosAngleLength = cosAngle * (KART_LENGTH / 4.0f);

	const float sinAngle = sinf(angle * DEG_TO_RAD);
	const float sinAngleWidth = sinAngle * (KART_WIDTH / 2.0f);
	const float sinAngleLength = sinAngle * (KART_LENGTH / 4.0f);

	wheel.setPosition(
		xPosition + sinAngleWidth + cosAngleLength,
		yPosition - cosAngleWidth + sinAngleLength); // front left wheel
	window.draw(wheel);
	wheel.setPosition(
		xPosition - sinAngleWidth + cosAngleLength,
		yPosition + cosAngleWidth + sinAngleLength); // front right wheel
	window.draw(wheel);

	wheel.setRotation(angle);

	wheel.setPosition(
		xPosition + sinAngleWidth - cosAngleLength,
		yPosition - cosAngleWidth - sinAngleLength); // back left wheel
	window.draw(wheel);
	wheel.setPosition(
		xPosition - sinAngleWidth - cosAngleLength,
		yPosition + cosAngleWidth - sinAngleLength); // back right wheel
	window.draw(wheel);
	
	// now render the body
	body.setRotation(angle);
	body.setPosition(sf::Vector2f(xPosition, yPosition));

	window.draw(body);
}

void Kart::resetPosition(float newXPosition, float newYPosition, float newAngle)
{
	xPosition = newXPosition;
	yPosition = newYPosition;
	angle = newAngle;
	wheelAngle = 0.0f;

	xVelocity = 0.0f;
	yVelocity = 0.0f;
	kartSpin = 0.0f;
}

float Kart::getLapProgress()
{
	return ((float)furthestCheckpoint + checkpointProgress) / (float)checkpointCount;
}

float Kart::getAngleBetween(float vectorX, float vectorY, float usedAngle)
{
	const float vectorMagnitude = sqrtf(vectorX * vectorX + vectorY * vectorY);
	if (vectorMagnitude < 0.001f) // essentially zero
	{
		return 0.0f;
	}

	const float dot = vectorX * cosf(usedAngle * DEG_TO_RAD) + vectorY * sinf(usedAngle * DEG_TO_RAD);
	if (dot > vectorMagnitude - 0.001f) // this is mainly because acosf kinda freaks out when given exactly 1
	{
		return 0.0f;
	}
	if (dot < -vectorMagnitude + 0.001f) // this is mainly because acosf kinda freaks out when given exactly 1
	{
		return 180.0f * DEG_TO_RAD;
	}
	return acosf(dot / vectorMagnitude);
}

bool Kart::isMovingForward()
{
	// calculates if its moving backward by finding the angle between its velocity and its forward trajectory
	return getAngleBetween(xVelocity, yVelocity, angle - wheelAngle) < (90.0f * DEG_TO_RAD);
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
	if (race == nullptr)
	{
		return;
	}

	bool collisionThisIteration = false;

	const int ITER_CAP = 3;
	for (int iter = 0; iter <= ITER_CAP && (iter == 0 || collisionThisIteration); iter++)
	{
		// calculate points for the corners of the kart, before and after the expected movement
		float cosAngle = cosf(angle * DEG_TO_RAD);
		float cosAngleWidth = cosAngle * (KART_WIDTH / 2.0f);
		float cosAngleLength = cosAngle * (KART_LENGTH / 2.0f);

		float sinAngle = sinf(angle * DEG_TO_RAD);
		float sinAngleWidth = sinAngle * (KART_WIDTH / 2.0f);
		float sinAngleLength = sinAngle * (KART_LENGTH / 2.0f);

		const float beforeFLX = xPosition + sinAngleWidth + cosAngleLength;
		const float beforeFLY = yPosition - cosAngleWidth + sinAngleLength;

		const float beforeFRX = xPosition - sinAngleWidth + cosAngleLength;
		const float beforeFRY = yPosition + cosAngleWidth + sinAngleLength;

		const float beforeBLX = xPosition + sinAngleWidth - cosAngleLength;
		const float beforeBLY = yPosition - cosAngleWidth - sinAngleLength;

		const float beforeBRX = xPosition - sinAngleWidth - cosAngleLength;
		const float beforeBRY = yPosition + cosAngleWidth - sinAngleLength;

		cosAngle = cosf((angle + angularVelocity + kartSpin) * DEG_TO_RAD);
		cosAngleWidth = cosAngle * (KART_WIDTH / 2.0f);
		cosAngleLength = cosAngle * (KART_LENGTH / 2.0f);

		sinAngle = sinf((angle + angularVelocity + kartSpin) * DEG_TO_RAD);
		sinAngleWidth = sinAngle * (KART_WIDTH / 2.0f);
		sinAngleLength = sinAngle * (KART_LENGTH / 2.0f);

		const float afterFLX = xPosition + sinAngleWidth + cosAngleLength + xVelocity;
		const float afterFLY = yPosition - cosAngleWidth + sinAngleLength + yVelocity;

		const float afterFRX = xPosition - sinAngleWidth + cosAngleLength + xVelocity;
		const float afterFRY = yPosition + cosAngleWidth + sinAngleLength + yVelocity;

		const float afterBLX = xPosition + sinAngleWidth - cosAngleLength + xVelocity;
		const float afterBLY = yPosition - cosAngleWidth - sinAngleLength + yVelocity;

		const float afterBRX = xPosition - sinAngleWidth - cosAngleLength + xVelocity;
		const float afterBRY = yPosition + cosAngleWidth - sinAngleLength + yVelocity;

		Line flLine(beforeFLX, beforeFLY, afterFLX, afterFLY);
		Line frLine(beforeFRX, beforeFRY, afterFRX, afterFRY);
		Line blLine(beforeBLX, beforeBLY, afterBLX, afterBLY);
		Line brLine(beforeBRX, beforeBRY, afterBRX, afterBRY);

		Corner corner;
		float xIntersect;
		float yIntersect;
		float xNormal;
		float yNormal;
		float time = 2.0f;

		for (int i = 0; i < race[currentCheckpoint]->getWallsInCollisionGroup(); i++)
		{
			Wall* wall = race[currentCheckpoint]->getCollisionWallGroup()[i];

			if (Line::getLineSegmentIntersection(&flLine,
				wall->getLine(), &xIntersect, &yIntersect, &xNormal, &yNormal, &time))
			{
				corner = Corner::FrontLeft;
			}
			if (Line::getLineSegmentIntersection(&frLine,
				wall->getLine(), &xIntersect, &yIntersect, &xNormal, &yNormal, &time))
			{
				corner = Corner::FrontRight;
			}
			if (Line::getLineSegmentIntersection(&blLine,
				wall->getLine(), &xIntersect, &yIntersect, &xNormal, &yNormal, &time))
			{
				corner = Corner::BackLeft;
			}
			if (Line::getLineSegmentIntersection(&brLine,
				wall->getLine(), &xIntersect, &yIntersect, &xNormal, &yNormal, &time))
			{
				corner = Corner::BackRight;
			}
		}

		if (time <= 1.0f)
		{
			collisionThisIteration = true;

			kartSpin = 0.0f;
			angularVelocity = 0.0f;

			if (iter != ITER_CAP) // don't bother with any bounce on the last iter
			{
				const float angleBetween = getAngleBetween(xNormal, yNormal, angle) * RAD_TO_DEG;

				// reflection angle of a vector is: r = v - 2*n*dot(v,n) (n must be normalized)
				const float dot = xVelocity * xNormal + yVelocity * yNormal;
				const float velocityMagnitude = sqrt(xVelocity * xVelocity + yVelocity * yVelocity);
				float reduction;
				if (velocityMagnitude == 0.0f)
				{
					reduction = KART_ELASTICITY;
				}
				else
				{
					reduction = 1.0f - KART_BASE_BOUNCE_REDUCE - abs(dot / velocityMagnitude) * (1.0f - KART_ELASTICITY - KART_BASE_BOUNCE_REDUCE);
				}
				xVelocity = (xVelocity - 2.0f * xNormal * dot) * reduction;
				yVelocity = (yVelocity - 2.0f * yNormal * dot) * reduction;

				if (iter < ITER_CAP - 1) // don't bother with spin on second last iter
				{
					// after reflection, calculate kart spin by using the angle of incidence
					if (angleBetween <= 90.0f) // going backwards into the wall
					{
						float spin = (3.0f * KART_ELASTICITY + velocityMagnitude) * sqrtf(angleBetween * (90.0f - angleBetween)) * KART_SPIN_MULTIPLIER * KART_ELASTICITY;

						if ((corner == Corner::BackLeft) == (angleBetween > 40.0f))  // greater than a 40 degree bounce, so spin outwards (should be 45, but giving some leeway)
						{
							kartSpin = -spin;
						}
						else // less than a 40 degree bounce, so spin inwards
						{
							kartSpin = spin;
						}
					}
					else // going forward into it
					{
						float spin = (3.0f * KART_ELASTICITY + velocityMagnitude) * sqrtf((angleBetween - 90.0f) * (180.0f - angleBetween)) * KART_SPIN_MULTIPLIER * KART_ELASTICITY;

						if ((corner == Corner::FrontLeft) == (angleBetween > 140.0f)) // less than a 40 degree bounce, so spin inwards (should be 45, but giving some leeway)
						{
							kartSpin = -spin;
						}
						else // less than a 40 degree bounce, so spin outwards
						{
							kartSpin = spin;
						}
					}
				}
			}
			else // if too many collisions on one frame, just give up and say nothing happens
			{
				xVelocity = 0.0f;
				yVelocity = 0.0f;
			}
		}
		else
		{
			collisionThisIteration = false;
		}
	}
}

void Kart::doCheckpoints(float xPositionBefore, float yPositionBefore)
{
	if (race == nullptr)
	{
		return;
	}

	float xIntersect; // none of these are nessesary, but the function needs them
	float yIntersect;
	float xNormal;
	float yNormal;
	float time;

	Line kartMovement = Line(xPositionBefore, yPositionBefore, xPosition, yPosition);

	// attempt the checkpoint behind
	int behindIndex = currentCheckpoint == 0 ? checkpointCount - 1 : currentCheckpoint - 1;
	if (Line::getLineSegmentIntersection(&kartMovement,
		race[behindIndex]->getLine(),
		&xIntersect, &yIntersect, &xNormal, &yNormal, &time))
	{
		currentCheckpoint--;
		if (currentCheckpoint == -1)
		{
			currentCheckpoint = checkpointCount - 1;
		}
	}

	// attempt the checkpoint ahead
	if (Line::getLineSegmentIntersection(&kartMovement,
		race[currentCheckpoint]->getLine(),
		&xIntersect, &yIntersect, &xNormal, &yNormal, &time))
	{
		currentCheckpoint++;
		if (currentCheckpoint == checkpointCount)
		{
			currentCheckpoint = 0;
		}

		// update the furthest checkpoint and lap
		if (currentCheckpoint == 0 && furthestCheckpoint == checkpointCount - 1)
		{
			furthestCheckpoint = 0;
			lapNumber++;
		}
		else if (furthestCheckpoint == currentCheckpoint - 1)
		{
			furthestCheckpoint++;
		}
	}

	// calculate checkpoint progress
	behindIndex = currentCheckpoint == 0 ? checkpointCount - 1 : currentCheckpoint - 1;

	float behindDistance = Line::distanceToLine(xPosition, yPosition, race[behindIndex]->getLine());
	float forwardDistance = Line::distanceToLine(xPosition, yPosition, race[currentCheckpoint]->getLine());
	checkpointProgress = behindDistance / (behindDistance + forwardDistance);
}