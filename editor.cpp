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
                    break;
                case EditorState::Editting:
                    resetPositionButton->resize(e.size.width, e.size.height);
                    kartPositionLabel->resize(e.size.width, e.size.height);
                    useKartToggle->resize(e.size.width, e.size.height);
                    mousePositionLabel->resize(e.size.width, e.size.height);

                    editWallToggle->resize(e.size.width, e.size.height);
                    editCheckpointToggle->resize(e.size.width, e.size.height);

                    saveButton->resize(e.size.width, e.size.height);
                    quitButton->resize(e.size.width, e.size.height);

                    startColorLabel->resize(e.size.width, e.size.height);
                    startColorR->resize(e.size.width, e.size.height);
                    startColorG->resize(e.size.width, e.size.height);
                    startColorB->resize(e.size.width, e.size.height);
                    endColorLabel->resize(e.size.width, e.size.height);
                    endColorR->resize(e.size.width, e.size.height);
                    endColorG->resize(e.size.width, e.size.height);
                    endColorB->resize(e.size.width, e.size.height);
                    break;
                }
                window.setView(guiView);
            }

            switch (state)
            {
            case EditorState::MainMenu:
                newMapButton->handleEvent(window, e);
                break;
            case EditorState::Editting:
                bool clickedOnGuiElement = false;
                clickedOnGuiElement |= resetPositionButton->handleEvent(window, e);
                clickedOnGuiElement |= useKartToggle->handleEvent(window, e);

                clickedOnGuiElement |= editWallToggle->handleEvent(window, e);
                clickedOnGuiElement |= editCheckpointToggle->handleEvent(window, e);

                clickedOnGuiElement |= saveButton->handleEvent(window, e);
                clickedOnGuiElement |= quitButton->handleEvent(window, e);

                clickedOnGuiElement |= startColorR->handleEvent(window, e);
                clickedOnGuiElement |= startColorG->handleEvent(window, e);
                clickedOnGuiElement |= startColorB->handleEvent(window, e);
                clickedOnGuiElement |= endColorR->handleEvent(window, e);
                clickedOnGuiElement |= endColorG->handleEvent(window, e);
                clickedOnGuiElement |= endColorB->handleEvent(window, e);

                if (!clickedOnGuiElement)
                {
                    // only do selection event if no gui element has been clicked on, avoids selecting things while just trying to click a input field
                    handleSelectionEvent(e);
                }
                break;
            }
        }

        switch (state)
        {
        case EditorState::Editting:
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

            edittingView = sf::View(sf::FloatRect(
                xCamera - window.getSize().x * 0.5f,
                yCamera - window.getSize().y * 0.5f,
                window.getSize().x,
                window.getSize().y));

            kartPositionLabel->setText("Camera:  X: " + std::to_string(int(roundf(xCamera))) + "   Y: " + std::to_string(int(roundf(yCamera))), window);
            mousePositionLabel->setText("Mouse:  X: " + std::to_string(getRelativeMouseX()) + "   Y: " + std::to_string(getRelativeMouseY()), window);
            break;
        }

        window.clear();

        switch (state)
        {
        case EditorState::MainMenu:
            newMapButton->render(window);
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

            saveButton->render(window);
            quitButton->render(window);

            startColorLabel->render(window);
            startColorR->render(window);
            startColorG->render(window);
            startColorB->render(window);
            endColorLabel->render(window);
            endColorR->render(window);
            endColorG->render(window);
            endColorB->render(window);
            break;
        }

        window.display();
    }
}

void Editor::handleSelectionEvent(sf::Event& e)
{
    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left)
    {
        if (selectionType == EditSelection::Wall)
        {
            // if the cursor is within MIN_SELECTION_DISTANCE to a wall, change the current selection to it
            Wall* closest = nullptr;
            float closestDistance = MIN_SELECTION_DISTANCE;

            for (auto it = mapWalls.begin(); it != mapWalls.end(); it++)
            {
                float dist = Wall::distanceToLine(getRelativeMouseX(), getRelativeMouseY(), (*it).getX1(), (*it).getY1(), (*it).getX2(), (*it).getY2());
                if (dist < closestDistance)
                {
                    closest = &*it;
                    closestDistance = dist;
                }
            }

            updateSelectedWall(closest);
        }
    }
    else if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Right)
    {
        selectionHeld = true;
        selectionStart = sf::Vector2f(getRelativeMouseX(), getRelativeMouseY());
    }
    else if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Right)
    {
        selectionHeld = false;
        mapWalls.push_back(Wall(selectionStart.x, selectionStart.y, getRelativeMouseX(), getRelativeMouseY(), DEFAULT_WALL_THICKNESS, startColor, endColor));
        updateSelectedWall(&*(--mapWalls.end()));
    }
}

void Editor::renderSelection()
{
    if (selectionHeld)
    {
        sf::Vertex selectionLines[2];

        selectionLines[0].color = startColor;
        selectionLines[1].color = endColor;
        
        selectionLines[0].position = selectionStart;
        selectionLines[1].position = sf::Vector2f(getRelativeMouseX(), getRelativeMouseY());

        window.draw(selectionLines, 2, sf::PrimitiveType::Lines);
    }

    for (auto it = mapWalls.begin(); it != mapWalls.end(); it++)
    {
        (*it).render(window);
    }
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

        startColorR->setNewPlaceholder(std::to_string(currentlySelectedWall->getColor1().r));
        startColorG->setNewPlaceholder(std::to_string(currentlySelectedWall->getColor1().g));
        startColorB->setNewPlaceholder(std::to_string(currentlySelectedWall->getColor1().b));

        endColorR->setNewPlaceholder(std::to_string(currentlySelectedWall->getColor2().r));
        endColorG->setNewPlaceholder(std::to_string(currentlySelectedWall->getColor2().g));
        endColorB->setNewPlaceholder(std::to_string(currentlySelectedWall->getColor2().b));
    }
}

void Editor::openMainMenu()
{
	newMapButton = new Button([this]() { clickNewMapButton(); }, "New Blank Map", 32, font, Transform(0, 0, 0, 0, 0.3f, 0.7f, 0.2f, 0.35f), window);
}

void Editor::closeMainMenu()
{
    delete newMapButton;
}

void Editor::openEditting()
{
    currentlySelectedWall = nullptr;
    currentlyUsingKart = true;
    selectionType = EditSelection::Wall;
    selectionHeld = false;

    startColor = sf::Color(255, 170, 170, 255); // slightly red
    endColor = sf::Color(170, 170, 255, 255); // slightly blue

    timeTrialStart = new sf::RectangleShape(sf::Vector2f(KART_LENGTH, KART_WIDTH));
    timeTrialStart->setFillColor(sf::Color::Transparent);
    timeTrialStart->setOutlineColor(sf::Color::White);
    timeTrialStart->setOutlineThickness(5);
    timeTrialStart->setOrigin(KART_LENGTH / 2.0f, KART_WIDTH / 2.0f);
    setStartAngle(0.0f);

    editorKeyboardInput = new KeyboardInput();
    editorKart = new Kart("Edit Mode", editorKeyboardInput, sf::Color::Yellow, 0, 0, startAngle, nullptr, 0);

    resetPositionButton = new Button([this]() { clickResetPositionButton(); }, "Reset Position", 16, font, 
        Transform(5, 125, 5, 30, 0.0f, 0.0f, 0.0f, 0.0f), window);
    kartPositionLabel = new Label(LabelAlign::LeftMiddle, "", 16, font, 
        Transform(135, 135, 5, 30, 0.0f, 0.0f, 0.0f, 0.0f), window);
    useKartToggle = new Toggle([this](bool active) { toggleUseKartToggle(active); }, nullptr, true, LabelAlign::LeftMiddle, "Kart Camera", 16, font, 
        Transform(100, 125, 40, 65, 0.0f, 0.0f, 0.0f, 0.0f), window);
    mousePositionLabel = new Label(LabelAlign::LeftMiddle, "", 16, font, 
        Transform(135, 135, 40, 65, 0.0f, 0.0f, 0.0f, 0.0f), window);

    saveButton = new Button([this]() { clickSaveButton(); }, "Save", 16, font, 
        Transform(-105, -60, 5, 30, 1.0f, 1.0f, 0.0f, 0.0f), window);
    quitButton = new Button([this]() { clickQuitButton(); }, "Quit", 16, font, 
        Transform(-50, -5, 5, 30, 1.0f, 1.0f, 0.0f, 0.0f), window);

    currentEditSelection = new ToggleGroup(2);
    editWallToggle = new Toggle([this](bool active) { toggleEditWallToggle(active); }, currentEditSelection, true, LabelAlign::RightMiddle, "Walls", 16, font, 
        Transform(5, 30, 100, 125, 0.0f, 0.0f, 0.0f, 0.0f), window);
    editCheckpointToggle = new Toggle([this](bool active) { toggleEditCheckpointToggle(active); }, currentEditSelection, false, LabelAlign::RightMiddle, "Checkpoints", 16, font, 
        Transform(5, 30, 135, 160, 0.0f, 0.0f, 0.0f, 0.0f), window);
    currentEditSelection->setToggle(0, editWallToggle);
    currentEditSelection->setToggle(1, editCheckpointToggle);

    startColorLabel = new Label(LabelAlign::CenterBottom, "Start Color", 16, font,
        Transform(-355, -205, -60, -60, 1.0f, 1.0f, 1.0f, 1.0f), window);

    startColorR = new TextField(TextFieldType::UnsignedByte,
        [this](TextFieldResult result) { startColor.r = result.intResult; if (currentlySelectedWall != nullptr) { currentlySelectedWall->setColor1(startColor); } },
        "Red", std::to_string(startColor.r), 16, font,
        Transform(-355, -310, -30, -3, 1.0f, 1.0f, 1.0f, 1.0f), window);
    startColorG = new TextField(TextFieldType::UnsignedByte,
        [this](TextFieldResult result) { startColor.g = result.intResult; if (currentlySelectedWall != nullptr) { currentlySelectedWall->setColor1(startColor); } },
        "Green", std::to_string(startColor.g), 16, font,
        Transform(-300, -255, -30, -3, 1.0f, 1.0f, 1.0f, 1.0f), window);
    startColorB = new TextField(TextFieldType::UnsignedByte,
        [this](TextFieldResult result) { startColor.b = result.intResult; if (currentlySelectedWall != nullptr) { currentlySelectedWall->setColor1(startColor); } },
        "Blue", std::to_string(startColor.b), 16, font,
        Transform(-245, -205, -30, -3, 1.0f, 1.0f, 1.0f, 1.0f), window);

    endColorLabel = new Label(LabelAlign::CenterBottom, "End Color", 16, font,
        Transform(-155, -5, -60, -60, 1.0f, 1.0f, 1.0f, 1.0f), window);

    endColorR = new TextField(TextFieldType::UnsignedByte,
        [this](TextFieldResult result) { endColor.r = result.intResult; if (currentlySelectedWall != nullptr) { currentlySelectedWall->setColor2(endColor); } },
        "Red", std::to_string(endColor.r), 16, font,
        Transform(-155, -110, -30, -3, 1.0f, 1.0f, 1.0f, 1.0f), window);
    endColorG = new TextField(TextFieldType::UnsignedByte,
        [this](TextFieldResult result) { endColor.g = result.intResult; if (currentlySelectedWall != nullptr) { currentlySelectedWall->setColor2(endColor); } },
        "Green", std::to_string(endColor.g), 16, font,
        Transform(-100, -55, -30, -3, 1.0f, 1.0f, 1.0f, 1.0f), window);
    endColorB = new TextField(TextFieldType::UnsignedByte,
        [this](TextFieldResult result) { endColor.b = result.intResult; if (currentlySelectedWall != nullptr) { currentlySelectedWall->setColor2(endColor); } },
        "Blue", std::to_string(endColor.b), 16, font,
        Transform(-45, -5, -30, -3, 1.0f, 1.0f, 1.0f, 1.0f), window);
}

void Editor::setStartAngle(float newStartAngle)
{
    startAngle = newStartAngle;
    timeTrialStart->setRotation(startAngle);
}

void Editor::clickNewMapButton()
{
    // When the newMapButton is clicked, close the mainMenu and open a blank map to start editting
    state = EditorState::Editting;
    closeMainMenu();
    openEditting();
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
    selectionType = EditSelection::Wall;
}

void Editor::toggleEditCheckpointToggle(bool active) // active will never be false because its part of a group
{
    selectionType = EditSelection::Checkpoint;
}

void Editor::clickSaveButton()
{
    // TODO
}

void Editor::clickQuitButton()
{
    // TODO
}

int Editor::getRelativeMouseX()
{
    float returnable = xCamera + sf::Mouse::getPosition(window).x - float(window.getSize().x) / 2.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl))
    {
        returnable = roundf(returnable / 50.0f) * 50.0f;
    }
    return roundf(returnable);
}

int Editor::getRelativeMouseY()
{
    float returnable = yCamera + sf::Mouse::getPosition(window).y - float(window.getSize().y) / 2.0f;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl))
    {
        returnable = roundf(returnable / 50.0f) * 50.0f;
    }
    return roundf(returnable);
}

