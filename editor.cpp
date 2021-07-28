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
                    positionLabel->resize(e.size.width, e.size.height);
                    useKartToggle->resize(e.size.width, e.size.height);
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
                break;
            }
        }

        switch (state)
        {
        case EditorState::Editting:
            editorKart->tick();
            editorKart->update();
            edittingView = sf::View(sf::FloatRect(
                editorKart->getX() - window.getSize().x * 0.5f,
                editorKart->getY() - window.getSize().y * 0.5f,
                window.getSize().x,
                window.getSize().y));

            positionLabel->setText("X: " + std::to_string(int(roundf(editorKart->getX()))) + "   Y: " + std::to_string(int(roundf(editorKart->getY()))), window);
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
            positionLabel->render(window);
            useKartToggle->render(window);

            window.setView(edittingView);
            window.draw(*timeTrialStart);
            editorKart->render(window);
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

    resetPositionButton = new Button([this]() { clickResetPositionButton(); }, "Reset Position", 16, font, Transform(5, 120, 5, 30, 0.0f, 0.0f, 0.0f, 0.0f), window);
    positionLabel = new Label(LabelAlign::LeftMiddle, "", 16, font, Transform(130, 130, 5, 30, 0.0f, 0.0f, 0.0f, 0.0f), window);
    useKartToggle = new Toggle([this](bool active) { toggleUseKartToggle(active); }, true, LabelAlign::LeftMiddle, "Kart Cam. ", 16, font, Transform(95, 120, 35, 60, 0.0f, 0.0f, 0.0f, 0.0f), window);
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
    editorKart->resetPosition(0.0f, 0.0f, startAngle);
}

void Editor::toggleUseKartToggle(bool active)
{
}

