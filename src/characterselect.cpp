#include "characterselect.hpp"

CharacterSelect::CharacterSelect()
{
    
    p1Index = 0;
    p2Index = 3;

    p1Ready = false;
    p2Ready = false;

    std::string paths[4] =
    {
        "assets/imagenes/selec chavo.jpeg",
        "assets/imagenes/selec omniman.jpeg",
        "assets/imagenes/selec kratos.jpeg",
        "assets/imagenes/selec rengoku.jpeg"
    };

    sprites.clear();
    sprites.reserve(4);

    for(int i = 0; i < 4; i++)
{
    textures[i].loadFromFile(paths[i]);
    sprites.emplace_back(textures[i]);
    sprites[i].setScale({0.25f, 0.25f});
}

    positions[0] = {250, 150};
    positions[1] = {750, 150};
    positions[2] = {250, 450};
    positions[3] = {750, 450};

    for(int i = 0; i < 4; i++)
    {
        sprites[i].setPosition(positions[i]);
    }

    cursorP1.setSize({200, 200});
    cursorP1.setFillColor(sf::Color::Transparent);
    cursorP1.setOutlineThickness(5);
    cursorP1.setOutlineColor(sf::Color::Green);

    cursorP2.setSize({200, 200});
    cursorP2.setFillColor(sf::Color::Transparent);
    cursorP2.setOutlineThickness(5);
    cursorP2.setOutlineColor(sf::Color::Red);
}

void CharacterSelect::MoveP1(sf::Keyboard::Scancode key)
{
    if(p1Ready) return;

    if(key == sf::Keyboard::Scancode::A) p1Index--;
    if(key == sf::Keyboard::Scancode::D) p1Index++;
    if(key == sf::Keyboard::Scancode::W) p1Index -= 2;
    if(key == sf::Keyboard::Scancode::S) p1Index += 2;

    if(p1Index < 0) p1Index = 0;
    if(p1Index > 3) p1Index = 3;
}

void CharacterSelect::MoveP2(sf::Keyboard::Scancode key)
{
    if(p2Ready) return;

    if(key == sf::Keyboard::Scancode::Left) p2Index--;
    if(key == sf::Keyboard::Scancode::Right) p2Index++;
    if(key == sf::Keyboard::Scancode::Up) p2Index -= 2;
    if(key == sf::Keyboard::Scancode::Down) p2Index += 2;

    if(p2Index < 0) p2Index = 0;
    if(p2Index > 3) p2Index = 3;
}

void CharacterSelect::ConfirmP1()
{
    p1Ready = true;

    std::string paths[4] =
    {
        "assets/imagenes/selec chavo.jpeg",
        "assets/imagenes/selec omniman.jpeg",
        "assets/imagenes/selec kratos.jpeg",
        "assets/imagenes/selec rengoku.jpeg"
    };

    p1Result = paths[p1Index];
}

void CharacterSelect::ConfirmP2()
{
    p2Ready = true;

    std::string paths[4] =
    {
        "assets/imagenes/selec chavo.jpeg",
        "assets/imagenes/selec omniman.jpeg",
        "assets/imagenes/selec kratos.jpeg",
        "assets/imagenes/selec rengoku.jpeg"
    };

    p2Result = paths[p2Index];
}

void CharacterSelect::Draw(sf::RenderWindow& window)
{
    for(int i = 0; i < 4; i++)
    {
        window.draw(sprites[i]);
    }

    cursorP1.setPosition(positions[p1Index]);
    cursorP2.setPosition(positions[p2Index]);

    window.draw(cursorP1);
    window.draw(cursorP2);
}

void CharacterSelect::Run(sf::RenderWindow& window)
{
    while(window.isOpen())
    {
        while(auto event = window.pollEvent())
        {
            if(event->is<sf::Event::Closed>())
                window.close();

            if(event->is<sf::Event::KeyPressed>())
            {
                auto key = event->getIf<sf::Event::KeyPressed>();

                if(!key) continue;

                // Player 1
                MoveP1(key->scancode);

                if(key->scancode == sf::Keyboard::Scancode::Space)
                    ConfirmP1();

                // Player 2
                MoveP2(key->scancode);

                if(key->scancode == sf::Keyboard::Scancode::Enter)
                    ConfirmP2();
            }
        }

        window.clear();

        Draw(window);

        window.display();

        if(p1Ready && p2Ready)
            return;
    }
}

std::string CharacterSelect::GetPlayer1() const
{
    return p1Result;
}

std::string CharacterSelect::GetPlayer2() const
{
    return p2Result;
}

