#include "kart.h"

Kart::Kart(std::string racerName, Input* input, sf::Color kartColor, float startX, float startY, float startAngle)
	: rect(sf::Vector2f(KART_LENGTH, KART_WIDTH)) // the x component is the length, and the y is width. this is because the angle 0 is straight to the right

	, input(input)

	, xPosition(startX)
	, yPosition(startY)
	, angle(startAngle)

	, wheelAngle(0.0f)
	
	, xVelocity(0.0f)
	, yVelocity(0.0f)
	, angularVelocity(0.0f)
{
	rect.setFillColor(kartColor);
	rect.setOrigin(KART_LENGTH / 2.0f, KART_WIDTH / 2.0f); // position is now measured relative to the senter of the kart
}

void Kart::update()
{
	/* This physics is incredibly preliminary, this will be hugely updated in the future, it is only currently included 
	as a base line to use for experimenting with other features (i.e. map collision) */

	input->update();

	const float turningMulti = sqrtf(xVelocity * xVelocity + yVelocity * yVelocity);
	angle += input->getSteering() * turningMulti;

	const float movementMulti = input->getMovement() * 0.1f * (input->getMovement() < 0 ? 0.5f : 1.0f);
	xVelocity += cosf(angle * DEG_TO_RAD) * movementMulti;
	yVelocity += sinf(angle * DEG_TO_RAD) * movementMulti;

	// apply velocitys
	xPosition += xVelocity;
	yPosition += yVelocity;

	// drag and friction
	xVelocity *= 0.97f;
	yVelocity *= 0.97f;
}

void Kart::render(sf::RenderWindow& window)
{
	rect.setPosition(sf::Vector2f(xPosition, yPosition));
	rect.setRotation(angle);

	window.draw(rect);
}
