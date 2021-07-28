#include "gui.h"

Label::Label(LabelAlign alignment, std::string labelText, int fontSize, sf::Font& font, Transform transform, sf::RenderWindow& window)
	: GUI(transform) 

	, alignment(alignment)

	, label(labelText, font, fontSize)
{
	resize(window.getSize().x, window.getSize().y);
}

void Label::setText(std::string labelText, sf::RenderWindow& window)
{
	label.setString(labelText);

	resize(window.getSize().x, window.getSize().y);
}

void Label::resize(float screenWidth, float screenHeight)
{
	sf::Vector2f pos;
	switch (alignment) // x position
	{
	case LabelAlign::LeftBottom:
	case LabelAlign::LeftMiddle:
	case LabelAlign::LeftTop:
		pos.x = roundf(transform.xMinAnchor * screenWidth + transform.left - label.getLocalBounds().left);
		break;
	case LabelAlign::CenterBottom:
	case LabelAlign::CenterMiddle:
	case LabelAlign::CenterTop:
		pos.x = roundf((transform.xMaxAnchor + transform.xMinAnchor) * 0.5f * screenWidth + (transform.left + transform.right) * 0.5f - label.getLocalBounds().left - label.getLocalBounds().width * 0.5f);
		break;
	case LabelAlign::RightBottom:
	case LabelAlign::RightMiddle:
	case LabelAlign::RightTop:
		pos.x = roundf(transform.xMaxAnchor * screenWidth + transform.right - label.getLocalBounds().left - label.getLocalBounds().width);
		break;
	}

	switch (alignment) // y position
	{
	case LabelAlign::LeftTop:
	case LabelAlign::CenterTop:
	case LabelAlign::RightTop:
		pos.y = roundf(transform.yMinAnchor * screenHeight + transform.top - label.getLocalBounds().top);
		break;
	case LabelAlign::LeftMiddle:
	case LabelAlign::CenterMiddle:
	case LabelAlign::RightMiddle:
		pos.y = roundf((transform.yMaxAnchor + transform.yMinAnchor) * 0.5f * screenHeight + (transform.top + transform.bottom) * 0.5f - label.getLocalBounds().top - label.getLocalBounds().height * 0.5f);
		break;
	case LabelAlign::LeftBottom:
	case LabelAlign::CenterBottom:
	case LabelAlign::RightBottom:
		pos.y = roundf(transform.yMaxAnchor * screenHeight + transform.bottom - label.getLocalBounds().top - label.getLocalBounds().height);
		break;
	}

	label.setPosition(pos);
}

void Label::render(sf::RenderWindow& window)
{
	window.draw(label);
}
