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

    sf::RenderWindow window(sf::VideoMode(1200, 800), "Reinforcement Kart!");
    window.setFramerateLimit(60);

    if (editMode)
    {
        Editor edit(window, font);
        edit.run();
        return 0;
    }

    KeyboardInput input;

    Wall wA(Line(500, 600, 1000, 400), 2.0f, sf::Color::Blue, sf::Color::Blue);

    Wall wB(Line(200, 600, 900, 50), 2.0f, sf::Color::Red, sf::Color::Yellow);
    Wall wC(Line(600, 950, 1300, 400), 2.0f, sf::Color::Yellow, sf::Color::Red);
    Wall wD(Line(900, 50, 1450, 50), 2.0f, sf::Color::Yellow, sf::Color::Green);
    Wall wE(Line(1450, 50, 1300, 400), 2.0f, sf::Color::Green, sf::Color::Red);
    Wall wF(Line(600, 950, 50, 950), 2.0f, sf::Color::Yellow, sf::Color::Green);
    Wall wG(Line(50, 950, 200, 600), 2.0f, sf::Color::Green, sf::Color::Red);

    Wall wH(Line(500, 600, 550, 650), 2.0f, sf::Color::Blue, sf::Color::White);
    Wall wI(Line(1000, 400, 950, 350), 2.0f, sf::Color::Blue, sf::Color::White);

    Wall wJ(Line(550, 650, 450, 750), 2.0f, sf::Color::White, sf::Color::Cyan);
    Wall wK(Line(950, 350, 1050, 250), 2.0f, sf::Color::White, sf::Color::Cyan);
    Wall wL(Line(550, 650, 550, 750), 2.0f, sf::Color::White, sf::Color::Cyan);
    Wall wM(Line(950, 350, 950, 250), 2.0f, sf::Color::White, sf::Color::Cyan);
    Wall wN(Line(450, 750, 550, 750), 2.0f, sf::Color::Cyan, sf::Color::Cyan);
    Wall wO(Line(1050, 250, 950, 250), 2.0f, sf::Color::Cyan, sf::Color::Cyan);

    Wall** raceGroup = new Wall*[15];
    raceGroup[0] = &wA;
    raceGroup[1] = &wB;
    raceGroup[2] = &wC;
    raceGroup[3] = &wD;
    raceGroup[4] = &wE;
    raceGroup[5] = &wF;
    raceGroup[6] = &wG;
    raceGroup[7] = &wH;
    raceGroup[8] = &wI;
    raceGroup[9] = &wJ;
    raceGroup[10] = &wK;
    raceGroup[11] = &wL;
    raceGroup[12] = &wM;
    raceGroup[13] = &wN;
    raceGroup[14] = &wO;

    Checkpoint** race = new Checkpoint*[4];
    race[0] = new Checkpoint(Line(950, 250, 900, 50), raceGroup, 15);
    race[1] = new Checkpoint(Line(1050, 250, 1300, 400), raceGroup, 15);
    race[2] = new Checkpoint(Line(550, 750, 600, 950), raceGroup, 15);
    race[3] = new Checkpoint(Line(450, 750, 200, 600), raceGroup, 15);

    Kart kart("Test", &input, sf::Color::Yellow, 550, 550, -40, race, 4);

    Button button(&print, "abc", 32, font, Transform(70, 70, -40, 70, 0.0f, 0.1f, 0.1f, 0.2f), window);

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
            else if (e.type == sf::Event::Resized)
            {
                window.setView(sf::View(sf::FloatRect(0, 0, e.size.width, e.size.height)));
                button.resize(e.size.width, e.size.height);
                tf.resize(e.size.width, e.size.height);
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