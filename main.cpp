#include "main.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Reinforcement Kart!");
    window.setFramerateLimit(60);

    KeyboardInput input;
    Kart kart("Test", &input, sf::Color::Yellow, 250, 250, 40);

    Wall wA(100, 100, 200, 200, 2.0f, sf::Color::Red, sf::Color::Blue);
    Wall wB(300, 100, 350, 200, 2.0f, sf::Color::Red, sf::Color::Blue);
    Wall wC(400, 800, 800, 400, 2.0f, sf::Color::Green, sf::Color::White);
    Wall wD(0, 980, 1000, 980, 2.0f, sf::Color::Green, sf::Color::White);

    WallGroup* currentGroup = new WallGroup[1];
    currentGroup[0].count = 4;
    currentGroup[0].walls[0] = &wA;
    currentGroup[0].walls[1] = &wB;
    currentGroup[0].walls[2] = &wC;
    currentGroup[0].walls[3] = &wD;

    kart.updateWallGroups(currentGroup, 1);

    while (window.isOpen())
    {
        sf::Event e;
        while (window.pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        kart.tick();
        kart.update();

        window.clear();
        wA.render(window);
        wB.render(window);
        wC.render(window);
        wD.render(window);
        kart.render(window);
        window.display();
    }

    return 0;
}