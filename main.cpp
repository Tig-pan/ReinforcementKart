#include "main.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    window.setFramerateLimit(60);

    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    KeyboardInput input;

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

        input.update();

        shape.move(sf::Vector2f(input.getSteering(), input.getMovement())); // moves the basic circle depending on the KeyboardInput
        
        shape.setFillColor(input.getDrift() ? sf::Color::Blue : sf::Color::Green); // sets the circles color to blue if the drift key is pressed

        window.clear();
        if (!input.getAction()) // doesn't draw the circle if getAction is true
        {
            window.draw(shape);
        }
        window.display();
    }

    return 0;
}