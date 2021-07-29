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
        window.setView(guiView);

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
                resetPositionButton->handleEvent(window, e);
                useKartToggle->handleEvent(window, e);

                editWallToggle->handleEvent(window, e);
                editCheckpointToggle->handleEvent(window, e);

                saveButton->handleEvent(window, e);
                quitButton->handleEvent(window, e);
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
                edittingView = sf::View(sf::FloatRect(
                    editorKart->getX() - window.getSize().x * 0.5f,
                    editorKart->getY() - window.getSize().y * 0.5f,
                    window.getSize().x,
                    window.getSize().y));

                int camX = int(roundf(editorKart->getX()));
                int camY = int(roundf(editorKart->getY()));
                kartPositionLabel->setText("Camera:  X: " + std::to_string(camX) + "   Y: " + std::to_string(camY), window);
                mousePositionLabel->setText("Mouse:  X: " + std::to_string(camX + sf::Mouse::getPosition(window).x - int(window.getSize().x) / 2) + "   Y: " + std::to_string(camY + sf::Mouse::getPosition(window).y - int(window.getSize().y) / 2), window);
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
                edittingView = sf::View(sf::FloatRect(
                    xCamera - window.getSize().x * 0.5f,
                    yCamera - window.getSize().y * 0.5f,
                    window.getSize().x,
                    window.getSize().y));

                int camX = int(roundf(xCamera));
                int camY = int(roundf(yCamera));
                kartPositionLabel->setText("Camera:  X: " + std::to_string(camX) + "   Y: " + std::to_string(camY), window);
                mousePositionLabel->setText("Mouse:  X: " + std::to_string(camX + sf::Mouse::getPosition(window).x - int(window.getSize().x) / 2) + "   Y: " + std::to_string(camY + sf::Mouse::getPosition(window).y - int(window.getSize().y) / 2), window);
            }
            break;
        }

        window.clear();

        switch (state)
        {
        case EditorState::MainMenu:
            newMapButton->render(window);
            break;
        case EditorState::Editting:
            resetPositionButton->render(window);
            kartPositionLabel->render(window);
            useKartToggle->render(window);
            mousePositionLabel->render(window);

            editWallToggle->render(window);
            editCheckpointToggle->render(window);

            saveButton->render(window);
            quitButton->render(window);

            window.setView(edittingView);
            window.draw(*timeTrialStart);
            if (currentlyUsingKart)
            {
                editorKart->render(window);
            }
            break;
        }

        window.display();
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
    timeTrialStart = new sf::RectangleShape(sf::Vector2f(KART_LENGTH, KART_WIDTH));
    timeTrialStart->setFillColor(sf::Color::Transparent);
    timeTrialStart->setOutlineColor(sf::Color::White);
    timeTrialStart->setOutlineThickness(5);
    timeTrialStart->setOrigin(KART_LENGTH / 2.0f, KART_WIDTH / 2.0f);
    setStartAngle(0.0f);

    editorKeyboardInput = new KeyboardInput();
    editorKart = new Kart("Edit Mode", editorKeyboardInput, sf::Color::Yellow, 0, 0, startAngle, nullptr, 0);

    resetPositionButton = new Button([this]() { clickResetPositionButton(); }, "Reset Position", 16, font, Transform(5, 125, 5, 30, 0.0f, 0.0f, 0.0f, 0.0f), window);
    kartPositionLabel = new Label(LabelAlign::LeftMiddle, "", 16, font, Transform(135, 135, 5, 30, 0.0f, 0.0f, 0.0f, 0.0f), window);
    useKartToggle = new Toggle([this](bool active) { toggleUseKartToggle(active); }, nullptr, true, LabelAlign::LeftMiddle, "Kart Camera", 16, font, Transform(100, 125, 40, 65, 0.0f, 0.0f, 0.0f, 0.0f), window);
    mousePositionLabel = new Label(LabelAlign::LeftMiddle, "", 16, font, Transform(135, 135, 40, 65, 0.0f, 0.0f, 0.0f, 0.0f), window);

    saveButton = new Button([this]() { clickSaveButton(); }, "Save", 16, font, Transform(-105, -60, 5, 30, 1.0f, 1.0f, 0.0f, 0.0f), window);
    quitButton = new Button([this]() { clickQuitButton(); }, "Quit", 16, font, Transform(-50, -5, 5, 30, 1.0f, 1.0f, 0.0f, 0.0f), window);

    currentEditSelection = new ToggleGroup(2);
    editWallToggle = new Toggle([this](bool active) { (toggleEditWallToggle(active)); }, currentEditSelection, true, LabelAlign::RightMiddle, "Walls", 16, font, Transform(5, 30, 100, 125, 0.0f, 0.0f, 0.0f, 0.0f), window);
    editCheckpointToggle = new Toggle([this](bool active) { (toggleEditCheckpointToggle(active)); }, currentEditSelection, false, LabelAlign::RightMiddle, "Checkpoints", 16, font, Transform(5, 30, 135, 160, 0.0f, 0.0f, 0.0f, 0.0f), window);
    currentEditSelection->setToggle(0, editWallToggle);
    currentEditSelection->setToggle(1, editCheckpointToggle);

    currentlyUsingKart = true;
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

void Editor::toggleEditWallToggle(bool active)
{
    // TODO
}

void Editor::toggleEditCheckpointToggle(bool active)
{
    // TODO
}

void Editor::clickSaveButton()
{
    // TODO
}

void Editor::clickQuitButton()
{
    // TODO
}

