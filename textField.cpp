#include "gui.h"

TextField::TextField(TextFieldType type, std::string labelText, std::string placeholderText, int fontSize, sf::Font& font, Transform transform, sf::RenderWindow& window)
	: GUI(transform)

	, unselected(RECT_UNSELECTED)
	, highlighted(RECT_HIGHLIGHTED)
	, selected(RECT_SELECTED)

	, placeholderText(placeholderText)
	, fontSize(fontSize)
	, textString("")

	, rect()
	, label(labelText, font, fontSize)
	, text(placeholderText, font, fontSize)
{
	rect.setOutlineThickness(RECT_OUTLINE_THICKNESS);
	rect.setOutlineColor(RECT_OUTLINE);
	rect.setFillColor(RECT_UNSELECTED);

	resize(window.getSize().x, window.getSize().y);
}

TextField::TextField(TextFieldType type, std::string labelText, std::string placeholderText, int fontSize, sf::Font& font, Transform transform, sf::Color unselected, sf::Color highlighted, sf::Color selected, sf::Color outline, sf::RenderWindow& window)
	: GUI(transform)

	, unselected(unselected)
	, highlighted(highlighted)
	, selected(selected)

	, placeholderText(placeholderText)
	, fontSize(fontSize)
	, textString("")

	, rect()
	, label(labelText, font, fontSize)
	, text(placeholderText, font, fontSize)
{
	rect.setOutlineThickness(RECT_OUTLINE_THICKNESS);
	rect.setOutlineColor(outline);
	rect.setFillColor(unselected);

	resize(window.getSize().x, window.getSize().y);
}

void TextField::resize(float screenWidth, float screenHeight)
{
	rect.setSize(sf::Vector2f(
		(transform.xMaxAnchor - transform.xMinAnchor) * screenWidth - transform.left + transform.right,
		(transform.yMaxAnchor - transform.yMinAnchor) * screenHeight - transform.top + transform.bottom));
	rect.setOrigin(sf::Vector2f(rect.getSize().x * 0.5f, rect.getSize().y * 0.5f));
	rect.setPosition(
		(transform.xMaxAnchor + transform.xMinAnchor) * 0.5f * screenWidth + (transform.left + transform.right) * 0.5f,
		(transform.yMaxAnchor + transform.yMinAnchor) * 0.5f * screenHeight + (transform.top + transform.bottom) * 0.5f);

	label.setPosition(transform.xMinAnchor * screenWidth + transform.left - label.getLocalBounds().left, 
		transform.yMinAnchor * screenHeight + transform.top - fontSize - RECT_OUTLINE_THICKNESS - TEXT_PADDING);

	text.setPosition(transform.xMinAnchor * screenWidth + transform.left - label.getLocalBounds().left + TEXT_PADDING,
		(transform.yMaxAnchor + transform.yMinAnchor) * 0.5f * screenHeight + (transform.top + transform.bottom) * 0.5f - text.getLocalBounds().top - text.getLocalBounds().height * 0.5f);
}

void TextField::handleEvent(sf::RenderWindow& window, sf::Event& e)
{
	if (e.type == e.TextEntered)
	{
		unsigned char keyCode = e.text.unicode;

		textString.push_back(keyCode);
		text.setString(textString);
	}
}

void TextField::render(sf::RenderWindow& window)
{
	window.draw(rect);
	window.draw(label);
	window.draw(text);
}
