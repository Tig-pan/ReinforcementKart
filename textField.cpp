#include "gui.h"

TextField::TextField(TextFieldType type, std::function<void(TextFieldResult)> done, std::string labelText, std::string placeholderText, int fontSize, sf::Font& font, Transform transform, sf::RenderWindow& window)
	: GUI(transform)

	, type(type)
	, done(done)

	, unselected(RECT_UNSELECTED)
	, highlighted(RECT_HIGHLIGHTED)
	, selected(RECT_SELECTED)
	, outline(RECT_OUTLINE)

	, placeholderText(placeholderText)
	, fontSize(fontSize)
	, textString("")

	, rect()
	, label(labelText, font, fontSize)
	, text(placeholderText, font, fontSize)

	, held(false)
	, over(false)
	, active(false)
	, cursorTimeFlash(0)
{
	text.setFillColor(sf::Color(170, 170, 170, 255)); // placeholder color

	rect.setOutlineThickness(RECT_OUTLINE_THICKNESS);
	rect.setOutlineColor(RECT_OUTLINE);
	rect.setFillColor(RECT_UNSELECTED);

	cursor[0].color = sf::Color::White;
	cursor[1].color = sf::Color::White;

	resize(window.getSize().x, window.getSize().y);
}

TextField::TextField(TextFieldType type, std::function<void(TextFieldResult)> done, std::string labelText, std::string placeholderText, int fontSize, sf::Font& font, Transform transform, sf::Color unselected, sf::Color highlighted, sf::Color selected, sf::Color outline, sf::RenderWindow& window)
	: GUI(transform)

	, type(type)
	, done(done)

	, unselected(unselected)
	, highlighted(highlighted)
	, selected(selected)
	, outline(outline)

	, placeholderText(placeholderText)
	, fontSize(fontSize)
	, textString("")

	, rect()
	, label(labelText, font, fontSize)
	, text(placeholderText, font, fontSize)

	, held(false)
	, over(false)
	, active(false)
	, cursorTimeFlash(0)
{
	text.setFillColor(sf::Color(170, 170, 170, 255)); // placeholder color

	rect.setOutlineThickness(RECT_OUTLINE_THICKNESS);
	rect.setOutlineColor(outline);
	rect.setFillColor(unselected);

	cursor[0].color = sf::Color::White;
	cursor[1].color = sf::Color::White;

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

	label.setPosition(
		roundf(transform.xMinAnchor * screenWidth + transform.left - label.getLocalBounds().left),
		roundf(transform.yMinAnchor * screenHeight + transform.top - fontSize - RECT_OUTLINE_THICKNESS - TEXT_PADDING));

	text.setPosition(
		roundf(transform.xMinAnchor * screenWidth + transform.left - label.getLocalBounds().left + TEXT_PADDING),
		roundf((transform.yMaxAnchor + transform.yMinAnchor) * 0.5f * screenHeight + (transform.top + transform.bottom) * 0.5f - text.getLocalBounds().top - text.getLocalBounds().height * 0.5f));

	cursor[0].position.y = transform.yMinAnchor * screenHeight + transform.top + TEXT_PADDING / 2.0f;
	cursor[1].position.y = transform.yMaxAnchor * screenHeight + transform.bottom - TEXT_PADDING / 2.0f;
}

void TextField::handleEvent(sf::RenderWindow& window, sf::Event& e)
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

	if (e.type == sf::Event::MouseButtonReleased)
	{
		if (active && !held) // was active, and is now not going to be
		{
			finishText();
		}

		active = held;

		rect.setOutlineColor(active ? sf::Color::Yellow : outline);
		text.setFillColor(sf::Color::White);
		text.setString(textString);

		if (active)
		{
			// find closest cursor position to mouse
			float closest = 9999.0f;
			for (int i = 0; i < textString.size(); i++)
			{
				float distance = sf::Mouse::getPosition(window).x - text.findCharacterPos(i).x;
				if (abs(distance) < abs(closest))
				{
					cursorPosition = i;
					closest = distance;
				}
			}
			if (cursorPosition == textString.size() - 1 && closest > text.getCharacterSize() / 2.0f) // too far off to the right
			{
				cursorPosition = textString.size();
			}
		}
		updateCursorPosition();

		held = false;
		rect.setFillColor(unselected);
	}

	if (e.type == e.KeyPressed && active)
	{
		if (e.key.code == sf::Keyboard::Left)
		{
			if (cursorPosition > 0)
			{
				cursorPosition--;
				updateCursorPosition();
			}
		}
		else if (e.key.code == sf::Keyboard::Right)
		{
			if (cursorPosition < textString.size())
			{
				cursorPosition++;
				updateCursorPosition();
			}
		}
		else if (e.key.code == sf::Keyboard::Delete) // there is no escape sequence for delete
		{
			if (cursorPosition < textString.size())
			{
				textString.erase(cursorPosition, 1);
				text.setString(textString);
				updateCursorPosition();
			}
		}
	}
	if (e.type == e.TextEntered && active)
	{
		unsigned char keyCode = e.text.unicode;

		if (keyCode == '\r')
		{
			active = false;

			rect.setOutlineColor(outline);
			rect.setFillColor(over ? highlighted : unselected);

			finishText();
		}
		else if (keyCode == '\b')
		{
			if (cursorPosition > 0)
			{
				textString.erase(cursorPosition - 1, 1);
				cursorPosition--;
				text.setString(textString);
				updateCursorPosition();
			}
		}
		else if (acceptableCharacter(keyCode))
		{
			textString.insert(cursorPosition, 1, keyCode); // the 1 is for 1 byte
			cursorPosition++;
			text.setString(textString);
			updateCursorPosition();
		}
	}
}

void TextField::render(sf::RenderWindow& window)
{
	window.draw(rect);
	window.draw(label);
	window.draw(text);
	cursorTimeFlash = (cursorTimeFlash + 1) % (TEXT_CURSOR_FLASH_TIME * 2);
	if (cursorTimeFlash <= TEXT_CURSOR_FLASH_TIME)
	{
		window.draw(cursor, 2, sf::PrimitiveType::Lines);
	}
}

bool TextField::acceptableCharacter(unsigned char keyCode)
{
	switch (type)
	{
	case TextFieldType::Text:
		return true;
	case TextFieldType::AlphaNumeric:
		return (keyCode >= '0' && keyCode <= '9') || (keyCode >= 'A' && keyCode <= 'Z') || (keyCode >= 'a' && keyCode <= 'z');
	case TextFieldType::IntegerNumeric:
		return (cursorPosition == 0 && keyCode == '-' && (textString.size() == 0 || textString[0] != '-')) || (keyCode >= '0' && keyCode <= '9');
	case TextFieldType::FloatingNumeric:
		return (keyCode == '.' && !stringContains('.')) || (cursorPosition == 0 && keyCode == '-' && (textString.size() == 0 || textString[0] != '-')) || (keyCode >= '0' && keyCode <= '9');
	}
}

bool TextField::stringContains(unsigned char keyCode)
{
	for (int i = 0; i < textString.size(); i++)
	{
		if (textString[i] == keyCode)
		{
			return true;
		}
	}
	return false;
}

void TextField::updateCursorPosition()
{
	cursor[0].position.x = text.findCharacterPos(cursorPosition).x;
	cursor[1].position.x = cursor[0].position.x;
}

void TextField::finishText()
{
	TextFieldResult param;
	switch (type)
	{
	case TextFieldType::Text:
	case TextFieldType::AlphaNumeric:
		param.textResult = new char[textString.size() + 1];
		for (int i = 0; i < textString.size(); i++)
		{
			param.textResult[i] = textString[i];
		}
		param.textResult[textString.size()] = '\0'; // null terminated string
		break;
	case TextFieldType::IntegerNumeric:
		if (textString.size() == 0)
		{
			textString = "0";
			text.setString(textString);
		}
		param.floatResult = std::stoi(textString);
		break;
	case TextFieldType::FloatingNumeric:
		if (textString.size() == 0)
		{
			textString = "0";
			text.setString(textString);
		}
		param.floatResult = std::stof(textString);
		break;
	}
	done(param);
}
