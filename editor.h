#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <iostream>
#include <list>

#include "gui.h"
#include "constants.h"
#include "wall.h"
#include "kart.h"

#define CAMERA_SPEED 10.0f

/*
All the possible states the editor can be in, ex. MainMenu, Editting, etc.
*/
enum class EditorState
{
	MainMenu,	// no map currently selected, therefore the menu should be (new/load/quit)
	Editting
};

/*
The current selection of objects to edit.
*/
enum class EditSelection
{
	Wall,
	Checkpoint
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
	// Handles events related to selections
	void handleSelectionEvent(sf::Event& e);
	// Renders everything to do with selections
	void renderSelection();

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

	// When the editWallToggle is clicked: change into using editting walls mode
	void toggleEditWallToggle(bool active);

	// When the editCheckpointToggle is clicked: change into using editting checkpoints mode
	void toggleEditCheckpointToggle(bool active);

	// When the saveButton is clicked: change the state to saving the map
	void clickSaveButton();

	// When the quitButton is clicked: confirm the quit, then go change the state to the main menu
	void clickQuitButton();

	// Returns the x position of the mouse relative to the current camera
	int getRelativeMouseX();
	// Returns the y position of the mouse relative to the current camera
	int getRelativeMouseY();

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
	Label* kartPositionLabel;
	Toggle* useKartToggle;
	Label* mousePositionLabel;

	ToggleGroup* currentEditSelection;
	Toggle* editWallToggle;
	Toggle* editCheckpointToggle;

	Button* saveButton;
	Button* quitButton;

	bool currentlyUsingKart;
	float xCamera;
	float yCamera;

	EditSelection selectionType;
	sf::Vector2f selectionStart;
	bool selectionHeld;

	// Map Data
	float startAngle;
	std::list<Wall> mapWalls;
};