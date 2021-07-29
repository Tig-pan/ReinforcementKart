#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>
#include <functional>

#include "constants.h"

/*
A method of determining screen dimensions. Uses variables for anchors along positions of the screen, and for fixed distances from those anchors.
*/
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

/*
Different methods of aligning a Label within its Transform.
*/
enum class LabelAlign
{
	LeftTop,
	LeftMiddle,
	LeftBottom,
	CenterTop,
	CenterMiddle,
	CenterBottom,
	RightTop,
	RightMiddle,
	RightBottom
};

/*
A Label class that inherits from GUI, is merely displays a string onto the screen.
*/
class Label : GUI
{
public:
	// A Label that extends GUI, takes a string and values relating to the text and transform
	Label(LabelAlign alignment, std::string labelText, int fontSize, sf::Font& font, Transform transform, sf::RenderWindow& window);

	// Changes the labelText text to be the given string
	void setText(std::string labelText, sf::RenderWindow& window);

	void resize(float screenWidth, float screenHeight) override;

	void handleEvent(sf::RenderWindow& window, sf::Event& e) override {} // no events to be handled with a label

	void render(sf::RenderWindow& window) override;

protected:
	LabelAlign alignment;

	sf::Text label;
};

/*
A Button class that inherits from GUI, is represented by a rectangle that can be clicked. Calls a given function when clicked.
*/
class Button : GUI
{
public:
	// A Button that extends GUI, taking all the GUI values and a function pointer to be called when the button is clicked
	Button(std::function<void()> clicked, std::string labelText, int fontSize, sf::Font& font, Transform transform, sf::RenderWindow& window);
	Button(std::function<void()> clicked, std::string labelText, int fontSize, sf::Font& font, Transform transform, sf::Color unselected, sf::Color highlighted, sf::Color selected, sf::Color outline, sf::RenderWindow& window);

	void resize(float screenWidth, float screenHeight) override;

	void handleEvent(sf::RenderWindow& window, sf::Event& e) override;

	void render(sf::RenderWindow& window) override;

protected:
	std::function<void()> clicked; // a void returning function with no parameters to be called when the button is clicked

	sf::RectangleShape rect;
	sf::Text label;

	sf::Color unselected;
	sf::Color highlighted;
	sf::Color selected;

	bool held;
	bool over;
};

class Toggle; // forward declaration so ToggleGroup can access it

/*
A ToggleGroup is a class that manages multiple toggles. And allows then to pick between multiple options, instead of merely turning a single value off and on.
*/
class ToggleGroup
{
public:
	// A ToggleGroup that controls multiple toggles. Must be created with a capacity, then have the toggles assigned after instantiation.
	ToggleGroup(int capacity);
	~ToggleGroup();

	// Assigns a specific toggle to a place in the internal toggle array.
	void setToggle(int index, Toggle* toggle);

	// Deactivates all other Toggles in the ToggleGroup other than the given one.
	void deactivateOtherToggles(Toggle* enabledToggle);
private:
	Toggle** toggleArray;
	int capacity;
};


/*
A Toggle class that inherits from GUI, is represented by a rectangle that can be clicked, when clicked it is toggled between 2 states. It also calls a function
with the current state as a parameter. Can also be included in a ToggleGroup.
*/
class Toggle : GUI
{
public:
	// A Toggle that extends GUI, functions kind of like a button, but can be toggled between 2 states
	Toggle(std::function<void(bool)> toggled, ToggleGroup* group, bool startState, LabelAlign alignment, std::string labelText, int fontSize, sf::Font& font, Transform transform, sf::RenderWindow& window);
	Toggle(std::function<void(bool)> toggled, ToggleGroup* group, bool startState, LabelAlign alignment, std::string labelText, int fontSize, sf::Font& font, Transform transform, sf::Color unselected, sf::Color highlighted, sf::Color selected, sf::Color outline, sf::RenderWindow& window);

	// A function typically called by the toggle group to deactivate this toggle remotely and without calling the toggled function
	void deactivate();

	void resize(float screenWidth, float screenHeight) override;

	void handleEvent(sf::RenderWindow& window, sf::Event& e) override;

	void render(sf::RenderWindow& window) override;
private:
	std::function<void(bool)> toggled;
	ToggleGroup* group;

	sf::RectangleShape rect;
	sf::Text label;
	sf::Vertex mark[4];

	LabelAlign alignment;

	sf::Color unselected;
	sf::Color highlighted;
	sf::Color selected;

	bool held;
	bool over;
	bool active;
};

/*
Different types of a TextField.
*/
enum class TextFieldType
{
	Text,
	AlphaNumeric, // no decimals are allowed, this basically just means no special characters
	IntegerNumeric,
	FloatingNumeric
};

/*
A union representing the possible results of the TextField.
*/
union TextFieldResult
{
	float floatResult;
	int intResult;
	char* textResult;
};

/*
A TextField class that inherits from GUI, is represented by a rectangle that can be clicked and have text inputted into it from the keyboard. 
Calls a function with the TextFieldResult when the input is done.
*/
class TextField : GUI
{
public:
	// A Text Field that extends GUI, takes keyboard input upon selection and allows for text or numbers to be entered
	TextField(TextFieldType type, std::function<void(TextFieldResult)> done, std::string labelText, std::string placeholderText, int fontSize, sf::Font& font, Transform transform, sf::RenderWindow& window);
	TextField(TextFieldType type, std::function<void(TextFieldResult)> done, std::string labelText, std::string placeholderText, int fontSize, sf::Font& font, Transform transform, sf::Color unselected, sf::Color highlighted, sf::Color selected, sf::Color outline, sf::RenderWindow& window);

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
	std::function<void(TextFieldResult)> done;

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