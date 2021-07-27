#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>

#include "constants.h"

struct Transform
{
	Transform(float left, float right, float top, float bottom, float xMinAnchor, float xMaxAnchor, float yMinAnchor, float yMaxAnchor)
		: left(left), right(right), top(top), bottom(bottom)
		, xMinAnchor(xMinAnchor), xMaxAnchor(xMaxAnchor), yMinAnchor(yMinAnchor), yMaxAnchor(yMaxAnchor)
	{}

	float left; // the left side of the bounding box, relative to the anchors
	float right; // the right side of the bounding box, relative to the anchors
	float top; // the top side of the bounding box, relative to the anchors
	float bottom; // the bottom side of the bounding box, relative to the anchors

	float xMinAnchor; // ranges from 0-1, 0 is left side of the screen, 1 is right
	float xMaxAnchor; // ranges from 0-1, 0 is left side of the screen, 1 is right
	float yMinAnchor; // ranges from 0-1, 0 is top of the screen, 1 is bottom
	float yMaxAnchor; // ranges from 0-1, 0 is top of the screen, 1 is bottom
};

/*
An abstract class used by all GUI elements. Contains values for positioning, scaling, and transforming the GUI element.
*/
class GUI abstract
{
public:
	// The constructor for the GUI class that takes numerous values all defining the position and scale of the GUI element
	GUI(Transform transform)
		: transform(transform)
	{}

	// Resizes and moves the GUI element based on the new screen dimensions
	virtual void resize(float screenWidth, float screenHeight) = 0;

	// Updates the GUI element based on this frames events
	virtual void handleEvent(sf::RenderWindow& window, sf::Event& e) = 0;

	// Renders the GUI element onto the render window
	virtual void render(sf::RenderWindow& window) = 0;

protected:
	// Returns true if the mouse is over the transform bounding box of the GUI element
	bool isMouseOver(sf::RenderWindow& window);

	Transform transform;
};

class Button : public virtual GUI
{
public:
	// A Button that extends GUI, taking all the GUI values and a function pointer to be called when the button is clicked
	Button(void (*clicked)(), Transform transform, sf::RenderWindow& window);
	Button(void (*clicked)(), Transform transform, sf::Color unselected, sf::Color highlighted, sf::Color selected, sf::Color outline, sf::RenderWindow& window);

	void resize(float screenWidth, float screenHeight) override;

	void handleEvent(sf::RenderWindow& window, sf::Event& e) override;

	void render(sf::RenderWindow& window) override;

protected:
	void (*clicked)(); // a void returning function with no parameters to be called when the button is clicked

	sf::RectangleShape rect;

	sf::Color unselected;
	sf::Color highlighted;
	sf::Color selected;

	bool held;
	bool over;
};

enum class TextFieldType
{
	Text,
	AlphaNumeric, // no decimals are allowed, this basically just means no special characters
	IntegerNumeric,
	FloatingNumeric
};

union TextFieldResult
{
	float floatResult;
	int intResult;
	char* textResult;
};

class TextField : public virtual GUI
{
public:
	// A Text Field that extends GUI, takes keyboard input upon selection and allows for text or numbers to be entered
	TextField(TextFieldType type, void (*done)(TextFieldResult), std::string labelText, std::string placeholderText, int fontSize, sf::Font& font, Transform transform, sf::RenderWindow& window);
	TextField(TextFieldType type, void (*done)(TextFieldResult), std::string labelText, std::string placeholderText, int fontSize, sf::Font& font, Transform transform, sf::Color unselected, sf::Color highlighted, sf::Color selected, sf::Color outline, sf::RenderWindow& window);

	void resize(float screenWidth, float screenHeight) override;

	void handleEvent(sf::RenderWindow& window, sf::Event& e) override;

	void render(sf::RenderWindow& window) override;
protected:
	// Returns true if the given character is an acceptable character at the current position in the text, false otherwise
	bool acceptableCharacter(unsigned char keyCode);

	// Returns true if the current textString contains the given character, false otherwise
	bool stringContains(unsigned char keyCode);

	// Updates the current visual cursor position
	void updateCursorPosition();

	// This is called when the text field is done being iteracted with, and it now calls the done function with the appropriate parameter
	void finishText();

	TextFieldType type;
	void (*done)(TextFieldResult);

	sf::RectangleShape rect;
	sf::Text label;
	sf::Text text;

	std::string placeholderText;
	int fontSize;

	std::string textString;

	sf::Color unselected;
	sf::Color highlighted;
	sf::Color selected;
	sf::Color outline;

	sf::Vertex cursor[2];

	bool held;
	bool over;
	bool active;

	int cursorPosition;
	int cursorTimeFlash;
};