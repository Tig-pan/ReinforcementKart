#include "gui.h"

ToggleGroup::ToggleGroup(int capacity)
	: capacity(capacity)
{
	toggleArray = new Toggle*[capacity];
}

ToggleGroup::~ToggleGroup()
{
	delete[] toggleArray;
}

void ToggleGroup::setToggle(int index, Toggle* toggle)
{
	toggleArray[index] = toggle;
}

void ToggleGroup::deactivateOtherToggles(Toggle* enabledToggle)
{
	for (int i = 0; i < capacity; i++)
	{
		if (toggleArray[i] != enabledToggle)
		{
			toggleArray[i]->deactivate();
		}
	}
}

Toggle::Toggle(std::function<void(bool)> toggled, ToggleGroup* group, bool startState, LabelAlign alignment, std::string labelText, int fontSize, sf::Font& font, Transform transform, sf::RenderWindow& window)
	: GUI(transform)
	, toggled(toggled)
	, group(group)

	, alignment(alignment)

	, unselected(RECT_UNSELECTED)
	, highlighted(RECT_HIGHLIGHTED)
	, selected(RECT_UNSELECTED)

	, rect(sf::Vector2f(0, 0))
	, label(labelText, font, fontSize)

	, held(false)
	, over(false)
	, active(startState)
{
	rect.setOutlineThickness(RECT_OUTLINE_THICKNESS);
	rect.setOutlineColor(RECT_OUTLINE);
	rect.setFillColor(RECT_UNSELECTED);

	resize(window.getSize().x, window.getSize().y);
}

Toggle::Toggle(std::function<void(bool)> toggled, ToggleGroup* group, bool startState, LabelAlign alignment, std::string labelText, int fontSize, sf::Font& font, Transform transform, sf::Color unselected, sf::Color highlighted, sf::Color selected, sf::Color outline, sf::RenderWindow& window)
	: GUI(transform)
	, toggled(toggled)
	, group(group)

	, alignment(alignment)

	, unselected(unselected)
	, highlighted(highlighted)
	, selected(selected)

	, rect(sf::Vector2f(0, 0))
	, label(labelText, font, fontSize)

	, held(false)
	, over(false)
	, active(startState)
{
	rect.setOutlineThickness(RECT_OUTLINE_THICKNESS);
	rect.setOutlineColor(outline);
	rect.setFillColor(unselected);

	mark[0].color = sf::Color::White;
	mark[1].color = sf::Color::White;
	mark[2].color = sf::Color::White;
	mark[3].color = sf::Color::White;

	resize(window.getSize().x, window.getSize().y);
}

void Toggle::deactivate()
{
	active = false;
}

void Toggle::resize(float screenWidth, float screenHeight)
{
	rect.setSize(sf::Vector2f(
		(transform.xMaxAnchor - transform.xMinAnchor) * screenWidth - transform.left + transform.right,
		(transform.yMaxAnchor - transform.yMinAnchor) * screenHeight - transform.top + transform.bottom));
	rect.setOrigin(sf::Vector2f(rect.getSize().x * 0.5f, rect.getSize().y * 0.5f));
	rect.setPosition(
		(transform.xMaxAnchor + transform.xMinAnchor) * 0.5f * screenWidth + (transform.left + transform.right) * 0.5f,
		(transform.yMaxAnchor + transform.yMinAnchor) * 0.5f * screenHeight + (transform.top + transform.bottom) * 0.5f);

	sf::Vector2f pos;
	switch (alignment) // x position
	{
	case LabelAlign::LeftBottom:
	case LabelAlign::LeftMiddle:
	case LabelAlign::LeftTop:
		pos.x = roundf(transform.xMinAnchor * screenWidth + transform.left - label.getLocalBounds().left - label.getLocalBounds().width - TEXT_PADDING);
		break;
	case LabelAlign::CenterBottom:
	case LabelAlign::CenterMiddle:
	case LabelAlign::CenterTop:
		pos.x = roundf((transform.xMaxAnchor + transform.xMinAnchor) * 0.5f * screenWidth + (transform.left + transform.right) * 0.5f - label.getLocalBounds().left - label.getLocalBounds().width * 0.5f);
		break;
	case LabelAlign::RightBottom:
	case LabelAlign::RightMiddle:
	case LabelAlign::RightTop:
		pos.x = roundf(transform.xMaxAnchor * screenWidth + transform.right - label.getLocalBounds().left + TEXT_PADDING);
		break;
	}

	switch (alignment) // y position
	{
	case LabelAlign::LeftTop:
	case LabelAlign::CenterTop:
	case LabelAlign::RightTop:
		pos.y = roundf(transform.yMinAnchor * screenHeight + transform.top - label.getLocalBounds().top - label.getLocalBounds().height - TEXT_PADDING);
		break;
	case LabelAlign::LeftMiddle:
	case LabelAlign::CenterMiddle:
	case LabelAlign::RightMiddle:
		pos.y = roundf((transform.yMaxAnchor + transform.yMinAnchor) * 0.5f * screenHeight + (transform.top + transform.bottom) * 0.5f - label.getLocalBounds().top - label.getLocalBounds().height * 0.5f);
		break;
	case LabelAlign::LeftBottom:
	case LabelAlign::CenterBottom:
	case LabelAlign::RightBottom:
		pos.y = roundf(transform.yMaxAnchor * screenHeight + transform.bottom - label.getLocalBounds().top + TEXT_PADDING);
		break;
	}

	label.setPosition(pos);

	mark[0].position = sf::Vector2f(
		transform.xMinAnchor * screenWidth + transform.left,
		transform.yMinAnchor * screenHeight + transform.top);

	mark[1].position = sf::Vector2f(
		transform.xMaxAnchor * screenWidth + transform.right,
		transform.yMaxAnchor * screenHeight + transform.bottom);

	mark[2].position = sf::Vector2f(
		transform.xMaxAnchor * screenWidth + transform.right,
		transform.yMinAnchor * screenHeight + transform.top);

	mark[3].position = sf::Vector2f(
		transform.xMinAnchor * screenWidth + transform.left,
		transform.yMaxAnchor * screenHeight + transform.bottom);
}

bool Toggle::handleEvent(sf::RenderWindow& window, sf::Event& e)
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
		return true;
	}

	if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left && held)
	{
		held = false;

		rect.setFillColor(highlighted);
		if (group == nullptr)
		{
			active = !active;
			toggled(active);
		}
		else // has a ToggleGroup
		{
			if (!active)
			{
				active = true;
				group->deactivateOtherToggles(this);
				toggled(active);
			}
		}
	}
	return false;
}

void Toggle::render(sf::RenderWindow& window)
{
	window.draw(rect);
	window.draw(label);
	if (active)
	{
		window.draw(mark, 4, sf::PrimitiveType::Lines);
	}
}
