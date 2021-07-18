#include "main.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 1000), "Reinforcement Kart!");
    window.setFramerateLimit(60);

    KeyboardInput input;
    Kart kart("Test", &input, sf::Color::Yellow, 250, 250, 40);

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
        kart.render(window);
        window.display();
    }

    return 0;
}