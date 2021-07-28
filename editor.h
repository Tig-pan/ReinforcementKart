#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>

#include "gui.h"
#include "constants.h"
#include "kart.h"

enum class EditorState
{
	MainMenu,	// no map currently selected, therefore the menu should be (new/load/quit)
	Editting
};

/*
A class that emcompasses the entirety of the map editor. Allows you to save, load, and edit maps.
*/
class Editor
{
public:
	Editor(sf::RenderWindow& window, sf::Font& font);

	// Runs indefinatly until the editor is closed, or the window is closed
	void run();

private:
	// Creates all the GUI elements for the MainMenu state
	void openMainMenu();
	// Removes all the GUI elements for the MainMenu state
	void closeMainMenu();

	// Creates all the GUI elements for the Editting state
	void openEditting();
	// Updates the startAngle and repositions the timeTrialStart RectangleShape based on the updated startAngle
	void setStartAngle(float newStartAngle);

	// When the newMapButton is clicked: changes the state to editting with a blank map
	void clickNewMapButton();

	// When the resetPositionButton is clicked: set the position of the editting view/editorKart to be at 0,0
	void clickResetPositionButton();

	// When the useKartToggle is clicked: toggle between using the kart to move the camera, and moving it naturally
	void toggleUseKartToggle(bool active);

	sf::RenderWindow& window;
	sf::Font& font;

	EditorState state;

	sf::View guiView;

	// MainMenu
	Button* newMapButton;

	// Editting
	sf::RectangleShape* timeTrialStart;
	KeyboardInput* editorKeyboardInput;
	sf::View edittingView;
	Kart* editorKart;

	Button* resetPositionButton;
	Label* positionLabel;
	Toggle* useKartToggle;

	Button* saveButton;
	Button* abandonButton;



	// Map Data
	float startAngle;
};