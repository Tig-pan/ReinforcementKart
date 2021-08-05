#pragma once

#include <SFML/Graphics.hpp>

#include <string>
#include <sstream>
#include <iostream>
#include <list>
#include <fstream>

#include "gui.h"
#include "constants.h"
#include "wall.h"
#include "kart.h"

#define CAMERA_SPEED 10.0f
#define DEFAULT_WALL_THICKNESS 2.0f
#define SELECTED_WALL_THICKNESS 6.0f

#define MIN_SELECTION_DISTANCE 10.0f
#define CONTROL_SNAP_INCRERMENT 50.0f
#define SHIFT_SNAP_DISTANCE 25.0f

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
An enum that dictates if you can currently add or remove walls from a checkpoint group.
*/
enum class CheckpointWallMode
{
	SelectCheckpoint,
	Add,
	Remove
};

/*
A class that emcompasses the entirety of the map editor. Allows you to save, load, and edit maps.
*/
class Editor
{
public:
	/* A constuctor for Editor, allows for the creationand editing of maps, takes no parameters other than a font, and the render window to use.
	 
	window: the sf::RenderWindow& window to render the editor onto
	font: the sf::Font to use for all GUI text*/
	Editor(sf::RenderWindow& window, sf::Font& font);

	// Runs indefinatly until the editor is closed, or the window is closed
	void run();

private:
	// Handles events related to selections
	void handleSelectionEvent(sf::Event& e);
	// Renders everything to do with selections
	void renderSelection();

	// Returns the wall that can currently be selected by the mouse
	Wall* getMouseWallSelection();

	// Returns the checkpoint that can currently be selected by the mouse
	EditorCheckpoint* getMouseCheckpointSelection();

	// Sets the appropriate values after selecting a new Wall
	void updateSelectedWall(Wall* newSelection);

	// Sets the appropriate values after selecting a new Checkpoint
	void updateSelectedCheckpoint(EditorCheckpoint* newSelection);

	// Sets all the currently held walls of the checkpoint back to the default thickness
	void deselectAllCheckpointWalls();

	// Sets all the currently held walls of the checkpoint to the selected thickness
	void selectAllCheckpointWalls();

	// Creates all the GUI elements for the MainMenu state
	void openMainMenu();
	// Removes all the GUI elements for the MainMenu state
	void closeMainMenu();

	// Creates all the GUI elements for the Editting state
	void openEditting();
	// Updates the startAngle and repositions the timeTrialStart RectangleShape based on the updated startAngle
	void setStartAngle(TextFieldResult result);

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

	// When the confirmSaveButton is clicked: save the map to a file
	void saveMap();

	// When the quitButton is clicked: confirm the quit, then go change the state to the main menu
	void clickQuitButton();

	// When the deleteSelectionButton is clicked: delete the currently selected wall/checkpoint
	void clickDeleteSelectionButton();

	// Returns the position of the mouse relative to the current camera
	sf::Vector2i getRelativeMouse();

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

	TextField* startAngleInput;

	Button* saveButton;
	Button* quitButton;

	Label* startPositionLabel;
	TextField* startXInput;
	TextField* startYInput;
	Label* endPositionLabel;
	TextField* endXInput;
	TextField* endYInput;

	Button* deleteSelectionButton;

	Label* startColorLabel;
	TextField* startColorR;
	TextField* startColorG;
	TextField* startColorB;
	Label* endColorLabel;
	TextField* endColorR;
	TextField* endColorG;
	TextField* endColorB;

	Label* checkpointIndexLabel;

	ToggleGroup* addOrRemoveWall;
	Toggle* selectCheckpointToggle;
	Toggle* addWallToCheckpointToggle;
	Toggle* removeWallFromCheckpointToggle;

	bool currentlyUsingKart;
	float xCamera;
	float yCamera;

	EditSelection selectionType;
	sf::Vector2f selectionStart;
	bool selectionHeld;

	Wall* currentlySelectedWall;
	sf::Color startColor;
	sf::Color endColor;

	EditorCheckpoint* currentlySelectedCheckpoint;
	CheckpointWallMode checkpointWallMode;

	bool isSaveMenuOpen;
	TextField* saveFileNameInput;
	Button* cancelSaveButton;
	Button* confirmSaveButton;
	char* saveFileName;

	// Map Data
	float startAngle;
	std::list<Wall> mapWalls;
	std::list<EditorCheckpoint> mapCheckpoints;
};