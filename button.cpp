#include "gui.h"

Button::Button(std::function<void()> clicked, std::string labelText, int fontSize, sf::Font& font, Transform transform, sf::RenderWindow& window)
	: GUI(transform)
	, clicked(clicked)

	, unselected(RECT_UNSELECTED)
	, highlighted(RECT_HIGHLIGHTED)
	, selected(RECT_SELECTED)

	, rect()
	, label(labelText, font, fontSize)

	, held(false)
	, over(false)
{
	rect.setOutlineThickness(RECT_OUTLINE_THICKNESS);
	rect.setOutlineColor(RECT_OUTLINE);
	rect.setFillColor(RECT_UNSELECTED);

	resize(window.getSize().x, window.getSize().y);
}

Button::Button(std::function<void()> clicked, std::string labelText, int fontSize, sf::Font& font, Transform transform, sf::Color unselected, sf::Color highlighted, sf::Color selected, sf::Color outline, sf::RenderWindow& window)
	: GUI(transform)
	, clicked(clicked)

	, unselected(unselected)
	, highlighted(highlighted)
	, selected(selected)

	, rect(sf::Vector2f(0,0))
	, label(labelText, font, fontSize)

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

	label.setPosition(
		roundf((transform.xMaxAnchor + transform.xMinAnchor) * 0.5f * screenWidth + (transform.left + transform.right) * 0.5f - label.getLocalBounds().left - label.getLocalBounds().width * 0.5f),
		roundf((transform.yMaxAnchor + transform.yMinAnchor) * 0.5f * screenHeight + (transform.top + transform.bottom) * 0.5f - label.getLocalBounds().top - label.getLocalBounds().height * 0.5f));
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

	if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left && over)
	{
		held = true;

		rect.setFillColor(selected);
	}

	if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left && held)
	{
		held = false;

		rect.setFillColor(highlighted);
		clicked();
	}
}

void Button::render(sf::RenderWindow& window)
{
	window.draw(rect);
	window.draw(label);
}
