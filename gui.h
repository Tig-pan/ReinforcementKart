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
	/*A constuctor for the Transform, it is used to define a GUI elements position and scale.
	
	left: a float, representing how many units left the bounding box should stretch left
	right: a float, representing how many units left the bounding box should stretch right
	top: a float, representing how many units left the bounding box should stretch up
	bottom: a float, representing how many units left the bounding box should stretch down
	xMinAnchor: what percentage along the screen the left side of the bounding box should start
	xMaxAnchor: what percentage along the screen the right side of the bounding box should start
	yMinAnchor: what percentage along the screen the top side of the bounding box should start
	yMaxAnchor: what percentage along the screen the bottom side of the bounding box should start*/
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
	/*The constructor for the GUI class that takes numerous values all defining the positionand scale of the GUI element
	 
	transform: the Transform of the GUI element, used to define position and scale*/
	GUI(Transform transform)
		: transform(transform)
	{}

	// Resizes and moves the GUI element based on the new screen dimensions
	virtual void resize(float screenWidth, float screenHeight) = 0;

	// Updates the GUI element based on this frames events, returns true if the mouse is clicked (initial LMB press) on the GUI element
	virtual bool handleEvent(sf::RenderWindow& window, sf::Event& e) = 0;

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
	/* A Label that extends GUI, takes a stringand values relating to the textand transform
	
	alignment: a LabelAlign enum, defines where the text should start in relation to the Transform
	labelText: a string, the text to be displayed
	fontSize: an integer, the size of the font to be displayed
	font: a reference to a sf::Font, the font used
	transform: the Transform of the GUI element
	window: a reference to a sf::RenderWindow, used to calculate the size/position of the object*/
	Label(LabelAlign alignment, std::string labelText, int fontSize, sf::Font& font, Transform transform, sf::RenderWindow& window);

	// Changes the labelText text to be the given string
	void setText(std::string labelText, sf::RenderWindow& window);

	void resize(float screenWidth, float screenHeight) override;

	bool handleEvent(sf::RenderWindow& window, sf::Event& e) override { return false; } // no events to be handled with a label

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
	/* A constructor for Button that extends GUI, taking all the GUI values and a function pointer to be called when the button is clicked

	clicked: a void function, this is called when the button is clicked
	labelText: a string, the text to be displayed on the button
	fontSize: an integer, the size of the font to be displayed
	font: a reference to a sf::Font, the font used
	transform: the Transform of the GUI element
	window: a reference to a sf::RenderWindow, used to calculate the size/position of the object*/
	Button(std::function<void()> clicked, std::string labelText, int fontSize, sf::Font& font, Transform transform, sf::RenderWindow& window);
	Button(std::function<void()> clicked, std::string labelText, int fontSize, sf::Font& font, Transform transform, sf::Color unselected, sf::Color highlighted, sf::Color selected, sf::Color outline, sf::RenderWindow& window);

	void resize(float screenWidth, float screenHeight) override;

	bool handleEvent(sf::RenderWindow& window, sf::Event& e) override;

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
	/* A constructor for ToggleGroup that controls multiple toggles.Must be created with a capacity, then have the toggles assigned after instantiation.
	
	capacity: the integer amount of Toggles in the toggle group*/
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
	/* A constructor for Toggle that extends GUI, functions kind of like a button, but can be toggled between 2 states

	toggled: a void function with a bool parameter, this is called when the toggle is clicked with the current state of the toggle
	group: a ToggleGroup pointer, the toggle group to which this toggle belongs (set to `nullptr` if no ToggleGroup)
	startState: a boolean defining wether to start active or inactive
	alignment: a LabelAlign enum, defines where the label for the Toggle should start in relation to the Transform
	labelText: a string, the text to be displayed next to the Toggle
	fontSize: an integer, the size of the font to be displayed
	font: a reference to a sf::Font, the font used
	transform: the Transform of the GUI element
	window: a reference to a sf::RenderWindow, used to calculate the size/position of the object*/
	Toggle(std::function<void(bool)> toggled, ToggleGroup* group, bool startState, LabelAlign alignment, std::string labelText, int fontSize, sf::Font& font, Transform transform, sf::RenderWindow& window);
	Toggle(std::function<void(bool)> toggled, ToggleGroup* group, bool startState, LabelAlign alignment, std::string labelText, int fontSize, sf::Font& font, Transform transform, sf::Color unselected, sf::Color highlighted, sf::Color selected, sf::Color outline, sf::RenderWindow& window);

	// A function typically called by the toggle group to deactivate this toggle remotely and without calling the toggled function
	void deactivate();

	void resize(float screenWidth, float screenHeight) override;

	bool handleEvent(sf::RenderWindow& window, sf::Event& e) override;

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
	UnsignedByte, // 0-255 integer
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
	/* A constructor for TextField that extends GUI, takes keyboard input upon selection and allows for text or numbers to be entered

	type: a TextFieldType enum, defines what kind of input to accept into the text field, and what kind of output to produce
	done: a void function with a TextFieldResult parameter, this is called when the TextField is finished with the result of the text converted into the TextFieldResult field
	labelText: a string, the text to be displayed next to the TextField
	placeholderText: a string, the default text to be presented on the TextField before any input has begun
	fontSize: an integer, the size of the font to be displayed
	font: a reference to a sf::Font, the font used
	transform: the Transform of the GUI element
	window: a reference to a sf::RenderWindow, used to calculate the size/position of the object*/
	TextField(TextFieldType type, std::function<void(TextFieldResult)> done, std::string labelText, std::string placeholderText, int fontSize, sf::Font& font, Transform transform, sf::RenderWindow& window);
	TextField(TextFieldType type, std::function<void(TextFieldResult)> done, std::string labelText, std::string placeholderText, int fontSize, sf::Font& font, Transform transform, sf::Color unselected, sf::Color highlighted, sf::Color selected, sf::Color outline, sf::RenderWindow& window);

	// Replaces the current text with a new placeholder text, also resets the current textString
	void setNewPlaceholder(std::string newPlaceholder);

	void resize(float screenWidth, float screenHeight) override;

	bool handleEvent(sf::RenderWindow& window, sf::Event& e) override;

	void render(sf::RenderWindow& window) override;
protected:
	const sf::Color PLACEHOLDER_COLOR = sf::Color(170, 170, 170, 255);

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