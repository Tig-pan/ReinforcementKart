#include "main.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1500, 1000), "Reinforcement Kart!");
    window.setFramerateLimit(60);

    KeyboardInput input;
    Kart kart("Test", &input, sf::Color::Yellow, 550, 550, -40);

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

    WallGroup* currentGroup = new WallGroup[1];
    currentGroup[0].count = 15;
    currentGroup[0].walls = new Wall*[15];
    currentGroup[0].walls[0] = &wA;
    currentGroup[0].walls[1] = &wB;
    currentGroup[0].walls[2] = &wC;
    currentGroup[0].walls[3] = &wD;
    currentGroup[0].walls[4] = &wE;
    currentGroup[0].walls[5] = &wF;
    currentGroup[0].walls[6] = &wG;
    currentGroup[0].walls[7] = &wH;
    currentGroup[0].walls[8] = &wI;
    currentGroup[0].walls[9] = &wJ;
    currentGroup[0].walls[10] = &wK;
    currentGroup[0].walls[11] = &wL;
    currentGroup[0].walls[12] = &wM;
    currentGroup[0].walls[13] = &wN;
    currentGroup[0].walls[14] = &wO;

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
        window.display();
    }

    return 0;
}