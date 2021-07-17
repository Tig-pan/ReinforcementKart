#include "main.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(500, 500), "SFML works!");
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