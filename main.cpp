#include "main.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Reinforcement Kart!");
    window.setFramerateLimit(60);

    KeyboardInput input;
    Kart kart("Test", &input, sf::Color::Yellow, 250, 250, 40);

    Wall wA(100, 100, 200, 200, 2.0f, sf::Color::Red, sf::Color::Blue);
    Wall wB(300, 100, 350, 200, 2.0f, sf::Color::Red, sf::Color::Blue);

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

        kart.update();

        window.clear();
        wA.render(window);
        wB.render(window);
        kart.render(window);
        kart.intersectingWithWall(wA, window);
        window.display();
    }

    return 0;
}