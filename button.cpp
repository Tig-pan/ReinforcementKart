#include "gui.h"

Button::Button(void (*clicked)(), Transform transform, sf::RenderWindow& window)
	: GUI(transform)
	, clicked(clicked)

	, unselected(RECT_UNSELECTED)
	, highlighted(RECT_HIGHLIGHTED)
	, selected(RECT_SELECTED)

	, rect()

	, held(false)
	, over(false)
{
	rect.setOutlineThickness(RECT_OUTLINE_THICKNESS);
	rect.setOutlineColor(RECT_OUTLINE);
	rect.setFillColor(RECT_UNSELECTED);

	resize(window.getSize().x, window.getSize().y);
}

Button::Button(void (*clicked)(), Transform transform, sf::Color unselected, sf::Color highlighted, sf::Color selected, sf::Color outline, sf::RenderWindow& window)
	: GUI(transform)
	, clicked(clicked)

	, unselected(unselected)
	, highlighted(highlighted)
	, selected(selected)

	, rect()

	, held(false)
	, over(false)
{
	rect.setOutlineThickness(RECT_OUTLINE_THICKNESS);
	rect.setOutlineColor(outline);
	rect.setFillColor(unselected);

	resize(window.getSize().x, window.getSize().y);
}

void Button::resize(float screenWidth, float screenHeight)
{
	rect.setSize(sf::Vector2f(
		(transform.xMaxAnchor - transform.xMinAnchor) * screenWidth - transform.left + transform.right,
		(transform.yMaxAnchor - transform.yMinAnchor) * screenHeight - transform.top + transform.bottom));
	rect.setOrigin(sf::Vector2f(rect.getSize().x * 0.5f, rect.getSize().y * 0.5f));
	rect.setPosition(
		(transform.xMaxAnchor + transform.xMinAnchor) * 0.5f * screenWidth + (transform.left + transform.right) * 0.5f,
		(transform.yMaxAnchor + transform.yMinAnchor) * 0.5f * screenHeight + (transform.top + transform.bottom) * 0.5f);
}

void Button::handleEvent(sf::RenderWindow& window, sf::Event& e)
{
	if (e.type == sf::Event::MouseMoved)
	{
		if (isMouseOver(window))
		{
			over = true;
			rect.setFillColor(held ? selected : highlighted);
		}
		else
		{
			over = false;
			held = false;
			rect.setFillColor(unselected);
		}
	}

	if (e.type == sf::Event::MouseButtonPressed && over)
	{
		held = true;
		rect.setFillColor(selected);
	}

	if (e.type == sf::Event::MouseButtonReleased && held)
	{
		held = false;

		clicked();
		rect.setFillColor(highlighted);
	}
}

void Button::render(sf::RenderWindow& window)
{
	window.draw(rect);
}
