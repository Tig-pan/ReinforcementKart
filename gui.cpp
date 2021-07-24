#include "gui.h"

#include <iostream>

bool GUI::isMouseOver(sf::RenderWindow& window)
{
	float xMouse = sf::Mouse::getPosition(window).x;
	float yMouse = sf::Mouse::getPosition(window).y;

	return xMouse >= transform.xMinAnchor * window.getSize().x + transform.left &&
		xMouse <= transform.xMaxAnchor * window.getSize().x + transform.right &&
		yMouse >= transform.yMinAnchor * window.getSize().y + transform.top &&
		yMouse <= transform.yMaxAnchor * window.getSize().y + transform.bottom;
}