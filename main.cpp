#include "main.h"

void print()
{
    std::cout << "aa\n";
}

void printTextField(TextFieldResult t)
{
    std::cout << t.floatResult << "\n";
}

int main()
{
    bool editMode = true;

    sf::Font font;
    font.loadFromFile("Assets/arial.ttf");

    sf::RenderWindow window(sf::VideoMode(1500, 1000), "Reinforcement Kart!");
    window.setFramerateLimit(60);

    KeyboardInput input;

    Wall wA(500, 600, 1000, 400, 2.0f, sf::Color::Blue, sf::Color::Blue);

    Wall wB(200, 600, 900, 50, 2.0f, sf::Color::Red, sf::Color::Yellow);
    Wall wC(600, 950, 1300, 400, 2.0f, sf::Color::Yellow, sf::Color::Red);
    Wall wD(900, 50, 1450, 50, 2.0f, sf::Color::Yellow, sf::Color::Green);
    Wall wE(1450, 50, 1300, 400, 2.0f, sf::Color::Green, sf::Color::Red);
    Wall wF(600, 950, 50, 950, 2.0f, sf::Color::Yellow, sf::Color::Green);
    Wall wG(50, 950, 200, 600, 2.0f, sf::Color::Green, sf::Color::Red);

    Wall wH(500, 600, 550, 650, 2.0f, sf::Color::Blue, sf::Color::White);
    Wall wI(1000, 400, 950, 350, 2.0f, sf::Color::Blue, sf::Color::White);

    Wall wJ(550, 650, 450, 750, 2.0f, sf::Color::White, sf::Color::Cyan);
    Wall wK(950, 350, 1050, 250, 2.0f, sf::Color::White, sf::Color::Cyan);
    Wall wL(550, 650, 550, 750, 2.0f, sf::Color::White, sf::Color::Cyan);
    Wall wM(950, 350, 950, 250, 2.0f, sf::Color::White, sf::Color::Cyan);
    Wall wN(450, 750, 550, 750, 2.0f, sf::Color::Cyan, sf::Color::Cyan);
    Wall wO(1050, 250, 950, 250, 2.0f, sf::Color::Cyan, sf::Color::Cyan);

    WallGroup raceGroup;
    raceGroup.count = 15;
    raceGroup.walls = new Wall*[15];
    raceGroup.walls[0] = &wA;
    raceGroup.walls[1] = &wB;
    raceGroup.walls[2] = &wC;
    raceGroup.walls[3] = &wD;
    raceGroup.walls[4] = &wE;
    raceGroup.walls[5] = &wF;
    raceGroup.walls[6] = &wG;
    raceGroup.walls[7] = &wH;
    raceGroup.walls[8] = &wI;
    raceGroup.walls[9] = &wJ;
    raceGroup.walls[10] = &wK;
    raceGroup.walls[11] = &wL;
    raceGroup.walls[12] = &wM;
    raceGroup.walls[13] = &wN;
    raceGroup.walls[14] = &wO;

    Checkpoint** race = new Checkpoint*[4];
    race[0] = new Checkpoint(950, 250, 900, 50, raceGroup);
    race[1] = new Checkpoint(1050, 250, 1300, 400, raceGroup);
    race[2] = new Checkpoint(550, 750, 600, 950, raceGroup);
    race[3] = new Checkpoint(450, 750, 200, 600, raceGroup);

    Kart kart("Test", &input, sf::Color::Yellow, 550, 550, -40, race, 4);

    Button button(&print, Transform(70, 70, -40, 70, 0.0f, 0.1f, 0.1f, 0.2f), window);

    TextField tf(TextFieldType::FloatingNumeric, &printTextField, "Hello World.", "abc", 16, font, Transform(0, 0, 0, 24, 0.7f, 0.9f, 0.1f, 0.1f), window);

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }

            button.handleEvent(window, e);
            tf.handleEvent(window, e);
        }

        kart.tick();
        kart.update();

        window.clear();
        wA.render(window);
        wB.render(window);
        wC.render(window);
        wD.render(window);
        wE.render(window);
        wF.render(window);
        wG.render(window);
        wH.render(window);
        wI.render(window);
        wJ.render(window);
        wK.render(window);
        wL.render(window);
        wM.render(window);
        wN.render(window);
        wO.render(window);
        kart.render(window);
        button.render(window);
        tf.render(window);
        window.display();
    }

    return 0;
}