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
    bool editMode = false;

    sf::Font font;
    font.loadFromFile("Assets/arial.ttf");

    sf::RenderWindow window(sf::VideoMode(1200, 800), "Reinforcement Kart!");
    window.setFramerateLimit(60);

    if (editMode)
    {
        Editor edit(window, font);
        edit.run();
    }
    else
    {
        Game game("harmony.map", new KeyboardInput(), window, font);
        game.run();
    }
    return 0;
}