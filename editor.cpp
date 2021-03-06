#include "editor.h"

Editor::Editor(sf::RenderWindow& window, sf::Font& font)
	: state(EditorState::MainMenu)
    
    , window(window)
    , font(font)

    , guiView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y))
{
    openMainMenu();
}

void Editor::run()
{
    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (e.type == sf::Event::Resized)
            {
                guiView = sf::View(sf::FloatRect(0, 0, e.size.width, e.size.height));
                switch (state)
                {
                case EditorState::MainMenu:
                    newMapButton->resize(e.size.width, e.size.height);
                    loadMapButton->resize(e.size.width, e.size.height);

                    loadFileNameInput->resize(e.size.width, e.size.height);
                    cancelLoadButton->resize(e.size.width, e.size.height);
                    confirmLoadButton->resize(e.size.width, e.size.height);
                    break;
                case EditorState::Editting:
                    resetPositionButton->resize(e.size.width, e.size.height);
                    kartPositionLabel->resize(e.size.width, e.size.height);
                    useKartToggle->resize(e.size.width, e.size.height);
                    mousePositionLabel->resize(e.size.width, e.size.height);

                    editWallToggle->resize(e.size.width, e.size.height);
                    editCheckpointToggle->resize(e.size.width, e.size.height);

                    startAngleInput->resize(e.size.width, e.size.height);

                    saveButton->resize(e.size.width, e.size.height);
                    quitButton->resize(e.size.width, e.size.height);

                    startPositionLabel->resize(e.size.width, e.size.height);
                    startXInput->resize(e.size.width, e.size.height);
                    startYInput->resize(e.size.width, e.size.height);
                    endPositionLabel->resize(e.size.width, e.size.height);
                    endXInput->resize(e.size.width, e.size.height);
                    endYInput->resize(e.size.width, e.size.height);

                    deleteSelectionButton->resize(e.size.width, e.size.height);

                    startColorLabel->resize(e.size.width, e.size.height);
                    startColorR->resize(e.size.width, e.size.height);
                    startColorG->resize(e.size.width, e.size.height);
                    startColorB->resize(e.size.width, e.size.height);
                    endColorLabel->resize(e.size.width, e.size.height);
                    endColorR->resize(e.size.width, e.size.height);
                    endColorG->resize(e.size.width, e.size.height);
                    endColorB->resize(e.size.width, e.size.height);

                    checkpointIndexLabel->resize(e.size.width, e.size.height);

                    selectCheckpointToggle->resize(e.size.width, e.size.height);
                    addCollisionWallToCheckpointToggle->resize(e.size.width, e.size.height);
                    removeCollisionWallFromCheckpointToggle->resize(e.size.width, e.size.height);
                    addVisionWallToCheckpointToggle->resize(e.size.width, e.size.height);
                    removeVisionWallFromCheckpointToggle->resize(e.size.width, e.size.height);

                    saveFileNameInput->resize(e.size.width, e.size.height);
                    cancelSaveButton->resize(e.size.width, e.size.height);
                    confirmSaveButton->resize(e.size.width, e.size.height);
                    break;
                }
                window.setView(guiView);
            }

            switch (state)
            {
            case EditorState::MainMenu:
                if (isLoadMenuOpen)
                {
                    loadFileNameInput->handleEvent(window, e);
                    cancelLoadButton->handleEvent(window, e);
                    confirmLoadButton->handleEvent(window, e);
                }
                else
                {
                    newMapButton->handleEvent(window, e);
                    loadMapButton->handleEvent(window, e);
                }
                break;
            case EditorState::Editting:
                if (isSaveMenuOpen)
                {
                    saveFileNameInput->handleEvent(window, e);
                    cancelSaveButton->handleEvent(window, e);
                    confirmSaveButton->handleEvent(window, e);
                }
                else
                {
                    bool clickedOnGuiElement = false;
                    clickedOnGuiElement |= resetPositionButton->handleEvent(window, e);
                    clickedOnGuiElement |= useKartToggle->handleEvent(window, e);

                    clickedOnGuiElement |= editWallToggle->handleEvent(window, e);
                    clickedOnGuiElement |= editCheckpointToggle->handleEvent(window, e);

                    clickedOnGuiElement |= startAngleInput->handleEvent(window, e);

                    clickedOnGuiElement |= saveButton->handleEvent(window, e);
                    clickedOnGuiElement |= quitButton->handleEvent(window, e);

                    if (currentlySelectedWall != nullptr || currentlySelectedCheckpoint != nullptr)
                    {
                        clickedOnGuiElement |= startXInput->handleEvent(window, e);
                        clickedOnGuiElement |= startYInput->handleEvent(window, e);
                        clickedOnGuiElement |= endXInput->handleEvent(window, e);
                        clickedOnGuiElement |= endYInput->handleEvent(window, e);

                        clickedOnGuiElement |= deleteSelectionButton->handleEvent(window, e);
                    }

                    if (selectionType == EditSelection::Wall)
                    {
                        clickedOnGuiElement |= startColorR->handleEvent(window, e);
                        clickedOnGuiElement |= startColorG->handleEvent(window, e);
                        clickedOnGuiElement |= startColorB->handleEvent(window, e);
                        clickedOnGuiElement |= endColorR->handleEvent(window, e);
                        clickedOnGuiElement |= endColorG->handleEvent(window, e);
                        clickedOnGuiElement |= endColorB->handleEvent(window, e);
                    }
                    else
                    {
                        clickedOnGuiElement |= selectCheckpointToggle->handleEvent(window, e);
                        clickedOnGuiElement |= addCollisionWallToCheckpointToggle->handleEvent(window, e);
                        clickedOnGuiElement |= removeCollisionWallFromCheckpointToggle->handleEvent(window, e);
                        clickedOnGuiElement |= addVisionWallToCheckpointToggle->handleEvent(window, e);
                        clickedOnGuiElement |= removeVisionWallFromCheckpointToggle->handleEvent(window, e);
                    }

                    if (!clickedOnGuiElement)
                    {
                        // only do selection event if no gui element has been clicked on, avoids selecting things while just trying to click a input field
                        handleSelectionEvent(e);
                    }
                }
                break;
            }
        }

        switch (state)
        {
        case EditorState::Editting:
            if (!isSaveMenuOpen)
            {
                if (currentlyUsingKart)
                {
                    editorKart->tick();
                    editorKart->update();

                    xCamera = editorKart->getX();
                    yCamera = editorKart->getY();
                }
                else
                {
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
                    {
                        xCamera -= CAMERA_SPEED;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
                    {
                        xCamera += CAMERA_SPEED;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
                    {
                        yCamera += CAMERA_SPEED;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
                    {
                        yCamera -= CAMERA_SPEED;
                    }
                }
            }

            edittingView = sf::View(sf::FloatRect(
                xCamera - window.getSize().x * 0.5f,
                yCamera - window.getSize().y * 0.5f,
                window.getSize().x,
                window.getSize().y));

            kartPositionLabel->setText("Camera:  X: " + std::to_string(int(roundf(xCamera))) + "   Y: " + std::to_string(int(roundf(yCamera))), window);
            sf::Vector2i mouse = getRelativeMouse();
            mousePositionLabel->setText("Mouse:  X: " + std::to_string(mouse.x) + "   Y: " + std::to_string(mouse.y), window);
            break;
        }

        window.clear();

        switch (state)
        {
        case EditorState::MainMenu:
            newMapButton->render(window);
            loadMapButton->render(window);

            if (isLoadMenuOpen)
            {
                loadFileNameInput->render(window);
                cancelLoadButton->render(window);
                confirmLoadButton->render(window);
            }
            break;
        case EditorState::Editting:
            // Render the actual world stuff first
            window.setView(edittingView);

            renderSelection();

            window.draw(*timeTrialStart);
            if (currentlyUsingKart)
            {
                editorKart->render(window);
            }

            // Render the gui stuff on top
            window.setView(guiView);

            resetPositionButton->render(window);
            kartPositionLabel->render(window);
            useKartToggle->render(window);
            mousePositionLabel->render(window);

            editWallToggle->render(window);
            editCheckpointToggle->render(window);

            startAngleInput->render(window);

            saveButton->render(window);
            quitButton->render(window);

            if (currentlySelectedWall != nullptr || currentlySelectedCheckpoint != nullptr)
            {
                startPositionLabel->render(window);
                startXInput->render(window);
                startYInput->render(window);
                endPositionLabel->render(window);
                endXInput->render(window);
                endYInput->render(window);

                deleteSelectionButton->render(window);

                if (currentlySelectedCheckpoint != nullptr)
                {
                    checkpointIndexLabel->render(window);
                }
            }

            if (selectionType == EditSelection::Wall)
            {
                startColorLabel->render(window);
                startColorR->render(window);
                startColorG->render(window);
                startColorB->render(window);
                endColorLabel->render(window);
                endColorR->render(window);
                endColorG->render(window);
                endColorB->render(window);
            }
            else
            {
                selectCheckpointToggle->render(window);
                addCollisionWallToCheckpointToggle->render(window);
                removeCollisionWallFromCheckpointToggle->render(window);
                addVisionWallToCheckpointToggle->render(window);
                removeVisionWallFromCheckpointToggle->render(window);
            }

            if (isSaveMenuOpen)
            {
                saveFileNameInput->render(window);
                cancelSaveButton->render(window);
                confirmSaveButton->render(window);
            }
            break;
        }

        window.display();
    }
}

void Editor::handleSelectionEvent(sf::Event& e)
{
    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Right)
    {
        selectionHeld = true;
        sf::Vector2i mouse = getRelativeMouse();
        selectionStart = sf::Vector2f(mouse.x, mouse.y);
    }

    if (selectionType == EditSelection::Wall)
    {
        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
        {
            updateSelectedWall(getMouseWallSelection());
        }
        else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Right)
        {
            selectionHeld = false;
            sf::Vector2i mouse = getRelativeMouse();
            mapWalls.push_back(Wall(Line(selectionStart.x, selectionStart.y, mouse.x, mouse.y), DEFAULT_WALL_THICKNESS, startColor, endColor));
            updateSelectedWall(&*(--mapWalls.end()));
        }
        else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Key::Delete)
        {
            if (currentlySelectedWall != nullptr)
            {
                clickDeleteSelectionButton(); // simulate clicking the delete selection button (i.e. delete the current wall)
            }
            else
            {
                // otherwise delete the wall under the current mouse selection
                Wall* toDelete = getMouseWallSelection();

                // removes that wall from the groupings of the checkpoints
                for (auto it = mapCheckpoints.begin(); it != mapCheckpoints.end(); it++)
                {
                    (*it).removeCollisionWall(toDelete);
                    (*it).removeVisionWall(toDelete);
                }

                for (auto it = mapWalls.begin(); it != mapWalls.end(); it++)
                {
                    if (&*it == toDelete)
                    {
                        mapWalls.erase(it);
                        break;
                    }
                }
            }
        }
    }
    else
    {
        if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left && checkpointWallMode == CheckpointWallMode::SelectCheckpoint)
        {
            if (checkpointWallMode == CheckpointWallMode::SelectCheckpoint)
            {
                updateSelectedCheckpoint(getMouseCheckpointSelection());
            }
        }
        else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && currentlySelectedCheckpoint != nullptr)
        {
            // these perform the adding and removing from the checkpoints group
            Wall* w = getMouseWallSelection();
            if (checkpointWallMode == CheckpointWallMode::AddCollision)
            {
                if (w != nullptr && !currentlySelectedCheckpoint->containsCollisionWall(w))
                {
                    currentlySelectedCheckpoint->addCollisionWall(w);
                    w->setThickness(SELECTED_WALL_THICKNESS);
                }
            }
            else if (checkpointWallMode == CheckpointWallMode::RemoveCollision)
            {
                if (w != nullptr && !currentlySelectedCheckpoint->removeCollisionWall(w))
                {
                    w->setThickness(DEFAULT_WALL_THICKNESS);
                }
            }
            else if (checkpointWallMode == CheckpointWallMode::AddVision)
            {
                if (w != nullptr && !currentlySelectedCheckpoint->containsVisionWall(w))
                {
                    currentlySelectedCheckpoint->addVisionWall(w);
                    w->setThickness(SELECTED_WALL_THICKNESS);
                }
            }
            else if (checkpointWallMode == CheckpointWallMode::RemoveVision)
            {
                if (w != nullptr && !currentlySelectedCheckpoint->removeVisionWall(w))
                {
                    w->setThickness(DEFAULT_WALL_THICKNESS);
                }
            }
        }
        else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Right)
        {
            selectionHeld = false;
            sf::Vector2i mouse = getRelativeMouse();
            mapCheckpoints.push_back(EditorCheckpoint(Line(selectionStart.x, selectionStart.y, mouse.x, mouse.y)));
            updateSelectedCheckpoint(&*(--mapCheckpoints.end()));
        }
    }
}

void Editor::renderSelection()
{
    if (selectionHeld)
    {
        sf::Vertex selectionLines[2];

        if (selectionType == EditSelection::Wall)
        {
            selectionLines[0].color = startColor;
            selectionLines[1].color = endColor;
        }
        else
        {
            selectionLines[0].color = sf::Color::White;
            selectionLines[1].color = sf::Color::White;
        }
        
        selectionLines[0].position = selectionStart;
        sf::Vector2i mouse = getRelativeMouse();
        selectionLines[1].position = sf::Vector2f(mouse.x, mouse.y);

        window.draw(selectionLines, 2, sf::PrimitiveType::Lines);
    }

    for (auto it = mapWalls.begin(); it != mapWalls.end(); it++)
    {
        (*it).render(window);
    }

    if (selectionType == EditSelection::Checkpoint)
    {
        // Render checkpoints
        sf::Vertex checkpointLines[2];

        for (auto it = mapCheckpoints.begin(); it != mapCheckpoints.end(); it++)
        {
            if (&*it == currentlySelectedCheckpoint)
            {
                checkpointLines[0].color = sf::Color::White;
                checkpointLines[1].color = sf::Color::White;
            }
            else
            {
                checkpointLines[0].color = sf::Color::Green;
                checkpointLines[1].color = sf::Color::Green;
            }

            checkpointLines[0].position = sf::Vector2f((*it).getLine()->x1, (*it).getLine()->y1);
            checkpointLines[1].position = sf::Vector2f((*it).getLine()->x2, (*it).getLine()->y2);

            window.draw(checkpointLines, 2, sf::PrimitiveType::Lines);
        }
    }
}

Wall* Editor::getMouseWallSelection()
{
    // if the cursor is within MIN_SELECTION_DISTANCE to a wall, change the current selection to it
    Wall* closest = nullptr;
    float closestDistance = MIN_SELECTION_DISTANCE;

    for (auto it = mapWalls.begin(); it != mapWalls.end(); it++)
    {
        sf::Vector2i mouse = getRelativeMouse();
        float dist = Line::distanceToLine(mouse.x, mouse.y, (*it).getLine());
        if (dist < closestDistance)
        {
            closest = &*it;
            closestDistance = dist;
        }
    }

    return closest;
}

EditorCheckpoint* Editor::getMouseCheckpointSelection()
{
    // if the cursor is within MIN_SELECTION_DISTANCE to a wall, change the current selection to it
    EditorCheckpoint* closest = nullptr;
    float closestDistance = MIN_SELECTION_DISTANCE;

    for (auto it = mapCheckpoints.begin(); it != mapCheckpoints.end(); it++)
    {
        sf::Vector2i mouse = getRelativeMouse();
        float dist = Line::distanceToLine(mouse.x, mouse.y, (*it).getLine());
        if (dist < closestDistance)
        {
            closest = &*it;
            closestDistance = dist;
        }
    }

    return closest;
}

void Editor::updateSelectedWall(Wall* newSelection)
{
    if (currentlySelectedWall != nullptr)
    {
        currentlySelectedWall->setThickness(DEFAULT_WALL_THICKNESS);
    }

    currentlySelectedWall = newSelection;
    if (currentlySelectedWall != nullptr)
    {
        currentlySelectedWall->setThickness(SELECTED_WALL_THICKNESS);

        std::ostringstream ss; // have to use string stream because its the only way that formats properly
        ss << (currentlySelectedWall->getLine()->x1);
        startXInput->setNewString(ss.str());
        ss.str(std::string()); // clears the ss
        ss << (currentlySelectedWall->getLine()->y1);
        startYInput->setNewString(ss.str());

        ss.str(std::string());
        ss << (currentlySelectedWall->getLine()->x2);
        endXInput->setNewString(ss.str());
        ss.str(std::string());
        ss << (currentlySelectedWall->getLine()->y2);
        endYInput->setNewString(ss.str());

        startColorR->setNewString(std::to_string(currentlySelectedWall->getColor1().r));
        startColorG->setNewString(std::to_string(currentlySelectedWall->getColor1().g));
        startColorB->setNewString(std::to_string(currentlySelectedWall->getColor1().b));

        endColorR->setNewString(std::to_string(currentlySelectedWall->getColor2().r));
        endColorG->setNewString(std::to_string(currentlySelectedWall->getColor2().g));
        endColorB->setNewString(std::to_string(currentlySelectedWall->getColor2().b));
    }
}

void Editor::updateSelectedCheckpoint(EditorCheckpoint* newSelection)
{
    if (currentlySelectedCheckpoint != nullptr)
    {
        deselectAllCheckpointWalls();
    }
    currentlySelectedCheckpoint = newSelection;

    if (currentlySelectedCheckpoint != nullptr)
    {
        selectAllCheckpointWalls();

        int index = 0;
        for (auto it = mapCheckpoints.begin(); it != mapCheckpoints.end(); it++, index++)
        {
            if (&*it == currentlySelectedCheckpoint)
            {
                checkpointIndexLabel->setText("Checkpoint: " + std::to_string(index + 1) + " / " + std::to_string(mapCheckpoints.size()), window);
                break;
            }
        }

        std::ostringstream ss; // have to use string stream because its the only way that formats properly
        ss << (currentlySelectedCheckpoint->getLine()->x1);
        startXInput->setNewString(ss.str());
        ss.str(std::string()); // clears the ss
        ss << (currentlySelectedCheckpoint->getLine()->y1);
        startYInput->setNewString(ss.str());

        ss.str(std::string());
        ss << (currentlySelectedCheckpoint->getLine()->x2);
        endXInput->setNewString(ss.str());
        ss.str(std::string());
        ss << (currentlySelectedCheckpoint->getLine()->y2);
        endYInput->setNewString(ss.str());
    }
}

void Editor::deselectAllCheckpointWalls()
{
    if (checkpointWallMode == CheckpointWallMode::AddCollision || checkpointWallMode == CheckpointWallMode::RemoveCollision)
    {
        for (auto it = currentlySelectedCheckpoint->getCollisionWalls().begin(); it != currentlySelectedCheckpoint->getCollisionWalls().end(); it++)
        {
            (*it)->setThickness(DEFAULT_WALL_THICKNESS);
        }
    }
    else if (checkpointWallMode == CheckpointWallMode::AddVision || checkpointWallMode == CheckpointWallMode::RemoveVision)
    {
        for (auto it = currentlySelectedCheckpoint->getVisionWalls().begin(); it != currentlySelectedCheckpoint->getVisionWalls().end(); it++)
        {
            (*it)->setThickness(DEFAULT_WALL_THICKNESS);
        }
    }
}

void Editor::selectAllCheckpointWalls()
{
    if (checkpointWallMode == CheckpointWallMode::AddCollision || checkpointWallMode == CheckpointWallMode::RemoveCollision)
    {
        for (auto it = currentlySelectedCheckpoint->getCollisionWalls().begin(); it != currentlySelectedCheckpoint->getCollisionWalls().end(); it++)
        {
            (*it)->setThickness(SELECTED_WALL_THICKNESS);
        }
    }
    else if (checkpointWallMode == CheckpointWallMode::AddVision || checkpointWallMode == CheckpointWallMode::RemoveVision)
    {
        for (auto it = currentlySelectedCheckpoint->getVisionWalls().begin(); it != currentlySelectedCheckpoint->getVisionWalls().end(); it++)
        {
            (*it)->setThickness(SELECTED_WALL_THICKNESS);
        }
    }
}

void Editor::openMainMenu()
{
    isLoadMenuOpen = false;

	newMapButton = new Button([this]() { clickNewMapButton(); },
        "New Blank Map", 32, font, Transform(0, 0, 0, 0, 0.3f, 0.7f, 0.2f, 0.35f), window);
    loadMapButton = new Button([this]() { isLoadMenuOpen = true; },
        "Load Map", 32, font, Transform(0, 0, 0, 0, 0.3f, 0.7f, 0.65f, 0.8f), window);

    loadFileNameInput = new TextField(TextFieldType::Text,
        [this](TextFieldResult result) { loadFileName = result.textResult; },
        "Load File Name:", "load-file.map", 18, font,
        Transform(-110, 110, -15, 15, 0.5f, 0.5f, 0.5f, 0.5f), window);
    cancelLoadButton = new Button([this]() { isLoadMenuOpen = false; }, "Cancel", 18, font,
        Transform(-195, -120, -15, 15, 0.5f, 0.5f, 0.5f, 0.5f), window);
    confirmLoadButton = new Button([this]() { loadMap(); }, "Confirm", 18, font,
        Transform(120, 195, -15, 15, 0.5f, 0.5f, 0.5f, 0.5f), window);
}

void Editor::closeMainMenu()
{
    delete newMapButton;
}

void Editor::openEditting()
{
    mapWalls.clear();
    mapCheckpoints.clear();

    currentlySelectedWall = nullptr;
    currentlyUsingKart = true;
    selectionType = EditSelection::Wall;
    checkpointWallMode = CheckpointWallMode::SelectCheckpoint;
    selectionHeld = false;

    startColor = sf::Color(255, 170, 170, 255); // slightly red
    endColor = sf::Color(170, 170, 255, 255); // slightly blue

    isSaveMenuOpen = false;

    timeTrialStart = new sf::RectangleShape(sf::Vector2f(KART_LENGTH, KART_WIDTH));
    timeTrialStart->setFillColor(sf::Color::Transparent);
    timeTrialStart->setOutlineColor(sf::Color::White);
    timeTrialStart->setOutlineThickness(5);
    timeTrialStart->setOrigin(KART_LENGTH / 2.0f, KART_WIDTH / 2.0f);
    startAngle = 0.0f;
    timeTrialStart->setRotation(0.0f);


    editorKeyboardInput = new KeyboardInput();
    editorKart = new Kart("Edit Mode", editorKeyboardInput, nullptr, sf::Color::Yellow, 0, 0, startAngle, nullptr, 0);


    resetPositionButton = new Button([this]() { clickResetPositionButton(); }, "Reset Position", 16, font, 
        Transform(5, 125, 5, 30, 0.0f, 0.0f, 0.0f, 0.0f), window);
    kartPositionLabel = new Label(LabelAlign::LeftMiddle, "", 16, font, 
        Transform(135, 135, 5, 30, 0.0f, 0.0f, 0.0f, 0.0f), window);
    useKartToggle = new Toggle([this](bool active) { toggleUseKartToggle(active); }, nullptr, true, LabelAlign::LeftMiddle, "Kart Camera", 16, font, 
        Transform(100, 125, 40, 65, 0.0f, 0.0f, 0.0f, 0.0f), window);
    mousePositionLabel = new Label(LabelAlign::LeftMiddle, "", 16, font, 
        Transform(135, 135, 40, 65, 0.0f, 0.0f, 0.0f, 0.0f), window);


    currentEditSelection = new ToggleGroup(2);
    editWallToggle = new Toggle([this](bool active) { toggleEditWallToggle(active); }, currentEditSelection, true, LabelAlign::RightMiddle, "Walls", 16, font, 
        Transform(5, 30, 100, 125, 0.0f, 0.0f, 0.0f, 0.0f), window);
    editCheckpointToggle = new Toggle([this](bool active) { toggleEditCheckpointToggle(active); }, currentEditSelection, false, LabelAlign::RightMiddle, "Checkpoints", 16, font, 
        Transform(5, 30, 135, 160, 0.0f, 0.0f, 0.0f, 0.0f), window);
    currentEditSelection->setToggle(0, editWallToggle);
    currentEditSelection->setToggle(1, editCheckpointToggle);

    startAngleInput = new TextField(TextFieldType::FloatingNumeric,
        [this](TextFieldResult result) { setStartAngle(result); },
        "Start Angle:", "0", 16, font,
        Transform(5, 90, 215, 240, 0.0f, 0.0f, 0.0f, 0.0f), window);


    saveButton = new Button([this]() { isSaveMenuOpen = true; }, "Save", 16, font,
        Transform(-105, -60, 5, 30, 1.0f, 1.0f, 0.0f, 0.0f), window);
    quitButton = new Button([this]() { clickQuitButton(); }, "Quit", 16, font,
        Transform(-50, -5, 5, 30, 1.0f, 1.0f, 0.0f, 0.0f), window);


    startPositionLabel = new Label(LabelAlign::CenterBottom, "Start Position", 16, font,
        Transform(5, 165, -60, -60, 0.0f, 0.0f, 1.0f, 1.0f), window);

    startXInput = new TextField(TextFieldType::FloatingNumeric,
        [this](TextFieldResult result) { 
            if (currentlySelectedWall != nullptr) { currentlySelectedWall->setX1(result); } 
            if (currentlySelectedCheckpoint != nullptr) { currentlySelectedCheckpoint->setX1(result); } },
        "X", "0", 16, font,
        Transform(5, 80, -30, -5, 0.0f, 0.0f, 1.0f, 1.0f), window);
    startYInput = new TextField(TextFieldType::FloatingNumeric,
        [this](TextFieldResult result) { 
            if (currentlySelectedWall != nullptr) { currentlySelectedWall->setY1(result); }
            if (currentlySelectedCheckpoint != nullptr) { currentlySelectedCheckpoint->setY1(result); } },
        "Y", "0", 16, font,
        Transform(90, 165, -30, -5, 0.0f, 0.0f, 1.0f, 1.0f), window);


    endPositionLabel = new Label(LabelAlign::CenterBottom, "End Position", 16, font,
        Transform(215, 375, -60, -60, 0.0f, 0.0f, 1.0f, 1.0f), window);

    endXInput = new TextField(TextFieldType::FloatingNumeric,
        [this](TextFieldResult result) {
            if (currentlySelectedWall != nullptr) { currentlySelectedWall->setX2(result); }
            if (currentlySelectedCheckpoint != nullptr) { currentlySelectedCheckpoint->setX2(result); } },
        "X", "0", 16, font,
        Transform(215, 290, -30, -5, 0.0f, 0.0f, 1.0f, 1.0f), window);
    endYInput = new TextField(TextFieldType::FloatingNumeric,
        [this](TextFieldResult result) {
            if (currentlySelectedWall != nullptr) { currentlySelectedWall->setY2(result); }
            if (currentlySelectedCheckpoint != nullptr) { currentlySelectedCheckpoint->setY2(result); } },
        "Y", "0", 16, font,
        Transform(300, 375, -30, -5, 0.0f, 0.0f, 1.0f, 1.0f), window);


    deleteSelectionButton = new Button([this]() { clickDeleteSelectionButton(); }, "Delete", 16, font,
        Transform(425, 480, -30, -5, 0.0f, 0.0f, 1.0f, 1.0f), window);


    startColorLabel = new Label(LabelAlign::CenterBottom, "Start Color", 16, font,
        Transform(-355, -205, -60, -60, 1.0f, 1.0f, 1.0f, 1.0f), window);

    startColorR = new TextField(TextFieldType::UnsignedByte,
        [this](TextFieldResult result) { startColor.r = result.intResult; 
            if (currentlySelectedWall != nullptr) { currentlySelectedWall->setColor1(startColor); } },
        "Red", std::to_string(startColor.r), 16, font,
        Transform(-355, -310, -30, -5, 1.0f, 1.0f, 1.0f, 1.0f), window);
    startColorG = new TextField(TextFieldType::UnsignedByte,
        [this](TextFieldResult result) { startColor.g = result.intResult; 
            if (currentlySelectedWall != nullptr) { currentlySelectedWall->setColor1(startColor); } },
        "Green", std::to_string(startColor.g), 16, font,
        Transform(-300, -255, -30, -5, 1.0f, 1.0f, 1.0f, 1.0f), window);
    startColorB = new TextField(TextFieldType::UnsignedByte,
        [this](TextFieldResult result) { startColor.b = result.intResult; 
            if (currentlySelectedWall != nullptr) { currentlySelectedWall->setColor1(startColor); } },
        "Blue", std::to_string(startColor.b), 16, font,
        Transform(-245, -205, -30, -5, 1.0f, 1.0f, 1.0f, 1.0f), window);

    endColorLabel = new Label(LabelAlign::CenterBottom, "End Color", 16, font,
        Transform(-155, -5, -60, -60, 1.0f, 1.0f, 1.0f, 1.0f), window);

    endColorR = new TextField(TextFieldType::UnsignedByte,
        [this](TextFieldResult result) { endColor.r = result.intResult; 
            if (currentlySelectedWall != nullptr) { currentlySelectedWall->setColor2(endColor); } },
        "Red", std::to_string(endColor.r), 16, font,
        Transform(-155, -110, -30, -5, 1.0f, 1.0f, 1.0f, 1.0f), window);
    endColorG = new TextField(TextFieldType::UnsignedByte,
        [this](TextFieldResult result) { endColor.g = result.intResult; 
            if (currentlySelectedWall != nullptr) { currentlySelectedWall->setColor2(endColor); } },
        "Green", std::to_string(endColor.g), 16, font,
        Transform(-100, -55, -30, -5, 1.0f, 1.0f, 1.0f, 1.0f), window);
    endColorB = new TextField(TextFieldType::UnsignedByte,
        [this](TextFieldResult result) { endColor.b = result.intResult; 
            if (currentlySelectedWall != nullptr) { currentlySelectedWall->setColor2(endColor); } },
        "Blue", std::to_string(endColor.b), 16, font,
        Transform(-45, -5, -30, -5, 1.0f, 1.0f, 1.0f, 1.0f), window);

    checkpointIndexLabel = new Label(LabelAlign::LeftMiddle, "", 16, font,
        Transform(0, 0, -30, -5, 0.5f, 0.5f, 1.0f, 1.0f), window);

    addOrRemoveWall = new ToggleGroup(5);
    selectCheckpointToggle = new Toggle([this](bool result) { 
            if (currentlySelectedCheckpoint != nullptr) { deselectAllCheckpointWalls(); }
            checkpointWallMode = CheckpointWallMode::SelectCheckpoint; 
            if (currentlySelectedCheckpoint != nullptr) { selectAllCheckpointWalls(); }},
        addOrRemoveWall, true, LabelAlign::LeftMiddle, "Select Checkpoint", 16, font,
        Transform(-30, -5, -170, -145, 1.0f, 1.0f, 1.0f, 1.0f), window);
    addCollisionWallToCheckpointToggle = new Toggle([this](bool result) { 
            if (currentlySelectedCheckpoint != nullptr) { deselectAllCheckpointWalls(); }
            checkpointWallMode = CheckpointWallMode::AddCollision; 
            if (currentlySelectedCheckpoint != nullptr) { selectAllCheckpointWalls(); }},
        addOrRemoveWall, false, LabelAlign::LeftMiddle, "Add Collision Wall", 16, font,
        Transform(-30, -5, -135, -110, 1.0f, 1.0f, 1.0f, 1.0f), window);
    removeCollisionWallFromCheckpointToggle = new Toggle([this](bool result) { 
            if (currentlySelectedCheckpoint != nullptr) { deselectAllCheckpointWalls(); }
            checkpointWallMode = CheckpointWallMode::RemoveCollision; 
            if (currentlySelectedCheckpoint != nullptr) { selectAllCheckpointWalls(); }},
        addOrRemoveWall, false, LabelAlign::LeftMiddle, "Remove Collision Wall", 16, font,
        Transform(-30, -5, -100, -75, 1.0f, 1.0f, 1.0f, 1.0f), window);
    addVisionWallToCheckpointToggle = new Toggle([this](bool result) {
            if (currentlySelectedCheckpoint != nullptr) { deselectAllCheckpointWalls(); }
            checkpointWallMode = CheckpointWallMode::AddVision;
            if (currentlySelectedCheckpoint != nullptr) { selectAllCheckpointWalls(); }},
        addOrRemoveWall, false, LabelAlign::LeftMiddle, "Add Vision Wall", 16, font,
        Transform(-30, -5, -65, -40, 1.0f, 1.0f, 1.0f, 1.0f), window);
    removeVisionWallFromCheckpointToggle = new Toggle([this](bool result) {
            if (currentlySelectedCheckpoint != nullptr) { deselectAllCheckpointWalls(); }
            checkpointWallMode = CheckpointWallMode::RemoveVision;
            if (currentlySelectedCheckpoint != nullptr) { selectAllCheckpointWalls(); }},
        addOrRemoveWall, false, LabelAlign::LeftMiddle, "Remove Vision Wall", 16, font,
        Transform(-30, -5, -30, -5, 1.0f, 1.0f, 1.0f, 1.0f), window);
    addOrRemoveWall->setToggle(0, selectCheckpointToggle);
    addOrRemoveWall->setToggle(1, addCollisionWallToCheckpointToggle);
    addOrRemoveWall->setToggle(2, removeCollisionWallFromCheckpointToggle);
    addOrRemoveWall->setToggle(3, addVisionWallToCheckpointToggle);
    addOrRemoveWall->setToggle(4, removeVisionWallFromCheckpointToggle);

    saveFileNameInput = new TextField(TextFieldType::Text,
        [this](TextFieldResult result) { saveFileName = result.textResult; },
        "Save File Name:", "save-file.map", 18, font,
        Transform(-110, 110, -15, 15, 0.5f, 0.5f, 0.5f, 0.5f), window);
    cancelSaveButton = new Button([this]() { isSaveMenuOpen = false; }, "Cancel", 18, font,
        Transform(-195, -120, -15, 15, 0.5f, 0.5f, 0.5f, 0.5f), window);
    confirmSaveButton = new Button([this]() { saveMap(); }, "Confirm", 18, font,
        Transform(120, 195, -15, 15, 0.5f, 0.5f, 0.5f, 0.5f), window);
}

void Editor::setStartAngle(TextFieldResult result)
{
    startAngle = result.floatResult;
    timeTrialStart->setRotation(startAngle);
}

void Editor::clickNewMapButton()
{
    // When the newMapButton is clicked, close the mainMenu and open a blank map to start editting
    state = EditorState::Editting;
    closeMainMenu();
    openEditting();
}

void Editor::loadMap()
{
    clickNewMapButton(); // simulates opening up a blank map

    // Start Load file
    std::ifstream mapFile;
    mapFile.open(loadFileName, std::ios::in | std::ios::binary);

    mapFile.read((char*)&startAngle, sizeof(startAngle));
    timeTrialStart->setRotation(startAngle);

    editorKart->resetPosition(0.0f, 0.0f, startAngle);

    int wallCount;
    mapFile.read((char*)&wallCount, sizeof(wallCount));

    float x1;
    float y1;
    float x2;
    float y2;
    uint8_t r1;
    uint8_t g1;
    uint8_t b1;
    uint8_t r2;
    uint8_t g2;
    uint8_t b2;

    for (int i = 0; i < wallCount; i++)
    {
        mapFile.read((char*)&x1, sizeof(x1));
        mapFile.read((char*)&y1, sizeof(y1));
        mapFile.read((char*)&x2, sizeof(x2));
        mapFile.read((char*)&y2, sizeof(y2));

        mapFile.read((char*)&r1, sizeof(r1));
        mapFile.read((char*)&g1, sizeof(g1));
        mapFile.read((char*)&b1, sizeof(b1));

        mapFile.read((char*)&r2, sizeof(r2));
        mapFile.read((char*)&g2, sizeof(g2));
        mapFile.read((char*)&b2, sizeof(b2));

        mapWalls.push_back(Wall(Line(x1, y1, x2, y2), DEFAULT_WALL_THICKNESS, sf::Color(r1, g1, b1), sf::Color(r2, g2, b2)));
    }

    int checkpointCount;
    mapFile.read((char*)&checkpointCount, sizeof(checkpointCount));

    int wallsInGroup;
    EditorCheckpoint* last;

    for (int i = 0; i < checkpointCount; i++)
    {
        mapFile.read((char*)&x1, sizeof(x1));
        mapFile.read((char*)&y1, sizeof(y1));
        mapFile.read((char*)&x2, sizeof(x2));
        mapFile.read((char*)&y2, sizeof(y2));

        mapCheckpoints.push_back(EditorCheckpoint(Line(x1, y1, x2, y2)));
        last = &*(--mapCheckpoints.end());

        // Collision Group
        mapFile.read((char*)&wallsInGroup, sizeof(wallsInGroup));

        for (int j = 0; j < wallsInGroup; j++)
        {
            int thisIndex;
            mapFile.read((char*)&thisIndex, sizeof(thisIndex));

            int index = 0;
            for (auto it = mapWalls.begin(); it != mapWalls.end(); it++, index++)
            {
                if (thisIndex == index)
                {
                    last->addCollisionWall(&*it);
                    break;
                }
            }
        }

        // Vision Group
        mapFile.read((char*)&wallsInGroup, sizeof(wallsInGroup));

        for (int j = 0; j < wallsInGroup; j++)
        {
            int thisIndex;
            mapFile.read((char*)&thisIndex, sizeof(thisIndex));

            int index = 0;
            for (auto it = mapWalls.begin(); it != mapWalls.end(); it++, index++)
            {
                if (thisIndex == index)
                {
                    last->addVisionWall(&*it);
                    break;
                }
            }
        }
    }
    // Done loading map
}

void Editor::clickResetPositionButton()
{
    if (currentlyUsingKart)
    {
        editorKart->resetPosition(0.0f, 0.0f, startAngle);
    }
    else
    {
        xCamera = 0.0f;
        yCamera = 0.0f;
    }
}

void Editor::toggleUseKartToggle(bool active)
{
    currentlyUsingKart = active;

    if (currentlyUsingKart)
    {
        editorKart->resetPosition(xCamera, yCamera, startAngle);
    }
    else
    {
        xCamera = editorKart->getX();
        yCamera = editorKart->getY();
    }
}

void Editor::toggleEditWallToggle(bool active) // active will never be false because its part of a group
{
    updateSelectedCheckpoint(nullptr);
    selectionType = EditSelection::Wall;
}

void Editor::toggleEditCheckpointToggle(bool active) // active will never be false because its part of a group
{
    updateSelectedWall(nullptr);
    selectionType = EditSelection::Checkpoint;
}

void Editor::saveMap()
{
    /* file format:
    *
    * startAngle
    * number of walls
    * (x1, y1, x2, y2, startColorR, startColorG, startColorB, endColorR, endColorG, endColorB) * number of walls
    * number of checkpoints
    * (x1, y1, x2, y2, number of walls in checkpoint group, (wall index) * number of walls in collision checkpoint group,  (wall index) * number of walls in vision checkpoint group) * number of checkpoints
    */

    uint8_t colorTemp;
    float temp;

    std::ofstream output;
    output.open(saveFileName, std::ios::out | std::ios::trunc | std::ios::binary);

    output.write((char*)&startAngle, sizeof(startAngle));

    int number = mapWalls.size();
    output.write((char*)&number, sizeof(number));

    for (auto it = mapWalls.begin(); it != mapWalls.end(); it++)
    {
        temp = (*it).getLine()->x1;
        output.write((char*)&temp, sizeof(temp));
        temp = (*it).getLine()->y1;
        output.write((char*)&temp, sizeof(temp));
        temp = (*it).getLine()->x2;
        output.write((char*)&temp, sizeof(temp));
        temp = (*it).getLine()->y2;
        output.write((char*)&temp, sizeof(temp));

        colorTemp = (*it).getColor1().r;
        output.write((char*)&colorTemp, sizeof(colorTemp));
        colorTemp = (*it).getColor1().g;
        output.write((char*)&colorTemp, sizeof(colorTemp));
        colorTemp = (*it).getColor1().b;
        output.write((char*)&colorTemp, sizeof(colorTemp));

        colorTemp = (*it).getColor2().r;
        output.write((char*)&colorTemp, sizeof(colorTemp));
        colorTemp = (*it).getColor2().g;
        output.write((char*)&colorTemp, sizeof(colorTemp));
        colorTemp = (*it).getColor2().b;
        output.write((char*)&colorTemp, sizeof(colorTemp));
    }

    number = mapCheckpoints.size();
    output.write((char*)&number, sizeof(number));

    for (auto it = mapCheckpoints.begin(); it != mapCheckpoints.end(); it++)
    {
        temp = (*it).getLine()->x1;
        output.write((char*)&temp, sizeof(temp));
        temp = (*it).getLine()->y1;
        output.write((char*)&temp, sizeof(temp));
        temp = (*it).getLine()->x2;
        output.write((char*)&temp, sizeof(temp));
        temp = (*it).getLine()->y2;
        output.write((char*)&temp, sizeof(temp));

        number = (*it).getCollisionWalls().size();
        output.write((char*)&number, sizeof(number));

        for (auto jt = (*it).getCollisionWalls().begin(); jt != (*it).getCollisionWalls().end(); jt++)
        {
            int index = 0;
            for (auto kt = mapWalls.begin(); kt != mapWalls.end(); kt++, index++)
            {
                if (&*kt == *jt)
                {
                    output.write((char*)&index, sizeof(index));
                    break;
                }
            }
        }

        number = (*it).getVisionWalls().size();
        output.write((char*)&number, sizeof(number));

        for (auto jt = (*it).getVisionWalls().begin(); jt != (*it).getVisionWalls().end(); jt++)
        {
            int index = 0;
            for (auto kt = mapWalls.begin(); kt != mapWalls.end(); kt++, index++)
            {
                if (&*kt == *jt)
                {
                    output.write((char*)&index, sizeof(index));
                    break;
                }
            }
        }
    }

    output.close();

    isSaveMenuOpen = false;
}

void Editor::clickQuitButton()
{
    window.close();
}

void Editor::clickDeleteSelectionButton()
{
    if (currentlySelectedWall != nullptr)
    {
        Wall* toDelete = currentlySelectedWall;

        // removes that wall from the groupings of the checkpoints
        for (auto it = mapCheckpoints.begin(); it != mapCheckpoints.end(); it++)
        {
            (*it).removeCollisionWall(toDelete);
            (*it).removeVisionWall(toDelete);
        }

        updateSelectedWall(nullptr);

        for (auto it = mapWalls.begin(); it != mapWalls.end(); it++)
        {
            if (&*it == toDelete)
            {
                mapWalls.erase(it);
                break;
            }
        }
    }
    else if (currentlySelectedCheckpoint != nullptr)
    {
        EditorCheckpoint* toDelete = currentlySelectedCheckpoint;
        updateSelectedCheckpoint(nullptr);

        for (auto it = mapCheckpoints.begin(); it != mapCheckpoints.end(); it++)
        {
            if (&*it == toDelete)
            {
                mapCheckpoints.erase(it);
                break;
            }
        }
    }
}

sf::Vector2i Editor::getRelativeMouse()
{
    float x = xCamera + sf::Mouse::getPosition(window).x - float(window.getSize().x) / 2.0f;
    float y = yCamera + sf::Mouse::getPosition(window).y - float(window.getSize().y) / 2.0f;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl)) // Control snaps the wall to increments of 50 units
    {
        x = roundf(x / CONTROL_SNAP_INCRERMENT) * CONTROL_SNAP_INCRERMENT;
        y = roundf(y / CONTROL_SNAP_INCRERMENT) * CONTROL_SNAP_INCRERMENT;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)) // Shift snaps the wall to other walls within 25 units
    {
        float closestX = x;
        float closestY = y;
        float closestDistance = SHIFT_SNAP_DISTANCE;

        for (auto it = mapWalls.begin(); it != mapWalls.end(); it++)
        {
            // start
            float dist = Line::distanceToStart(x, y, (*it).getLine());
            if (dist < closestDistance)
            {
                closestX = (*it).getLine()->x1;
                closestY = (*it).getLine()->y1;
                closestDistance = dist;
            }

            // end
            dist = Line::distanceToEnd(x, y, (*it).getLine());
            if (dist < closestDistance)
            {
                closestX = (*it).getLine()->x2;
                closestY = (*it).getLine()->y2;
                closestDistance = dist;
            }
        }
        x = closestX;
        y = closestY;
    }
    return sf::Vector2i(roundf(x), roundf(y));
}