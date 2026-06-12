#include "characterselect.hpp"
#include <iostream>

CharacterSelect::CharacterSelect()
{
    p1Index = 0;
    p2Index = 3;

    p1Ready = false;
    p2Ready = false;

    selectPaths[0] = "assets/imagenes/selec chavo.png";
    selectPaths[1] = "assets/imagenes/selec omniman.png";
    selectPaths[2] = "assets/imagenes/selec kratos.png";
    selectPaths[3] = "assets/imagenes/selec rengoku.png";

    fighterPaths[0] = "assets/imagenes/chavo.jpeg";
    fighterPaths[1] = "assets/imagenes/omniman.jpeg";
    fighterPaths[2] = "assets/imagenes/kratos.jpeg";
    fighterPaths[3] = "assets/imagenes/rengoku.jpeg";

    positions[0] = {180.f,120.f};
    positions[1] = {700.f,120.f};
    positions[2] = {180.f,400.f};
    positions[3] = {700.f,400.f};

    LoadCharacters();

    cursorP1.setSize({200.f,200.f});
    cursorP1.setFillColor(sf::Color::Transparent);
    cursorP1.setOutlineThickness(5.f);
    cursorP1.setOutlineColor(sf::Color::Green);

    cursorP2.setSize({200.f,200.f});
    cursorP2.setFillColor(sf::Color::Transparent);
    cursorP2.setOutlineThickness(5.f);
    cursorP2.setOutlineColor(sf::Color::Red);

    UpdateCursorPositions();
}

void CharacterSelect::LoadCharacters()
{
    sprites.clear();
    textures.clear();

    textures.resize(CHARACTER_COUNT);
    sprites.resize(CHARACTER_COUNT);

    for(int i = 0; i < CHARACTER_COUNT; i++)
    {
        if(!textures[i].loadFromFile(selectPaths[i]))
        {
            std::cout << "No se pudo cargar: " << selectPaths[i] << std::endl;
            continue;
        }

        sprites[i].setTexture(textures[i]);

        auto size = textures[i].getSize();

        if(size.x == 0 || size.y == 0)
            continue;

        float scaleX = 200.f / static_cast<float>(size.x);
        float scaleY = 200.f / static_cast<float>(size.y);

        sprites[i].setScale({scaleX, scaleY});
        sprites[i].setPosition(positions[i]);
    }
}

void CharacterSelect::UpdateCursorPositions()
{
    cursorP1.setPosition(positions[p1Index]);
    cursorP2.setPosition(positions[p2Index]);
}

void CharacterSelect::MoveP1(sf::Keyboard::Scancode key)
{
    if(p1Ready) return;

    switch(key)
    {
        case sf::Keyboard::Scancode::A:
            if(p1Index==1) p1Index=0;
            if(p1Index==3) p1Index=2;
        break;

        case sf::Keyboard::Scancode::D:
            if(p1Index==0) p1Index=1;
            if(p1Index==2) p1Index=3;
        break;

        case sf::Keyboard::Scancode::W:
            if(p1Index>=2) p1Index-=2;
        break;

        case sf::Keyboard::Scancode::S:
            if(p1Index<=1) p1Index+=2;
        break;

        default:
        break;
    }

    UpdateCursorPositions();
}

void CharacterSelect::MoveP2(sf::Keyboard::Scancode key)
{
    if(p2Ready) return;

    switch(key)
    {
        case sf::Keyboard::Scancode::Left:
            if(p2Index==1) p2Index=0;
            if(p2Index==3) p2Index=2;
        break;

        case sf::Keyboard::Scancode::Right:
            if(p2Index==0) p2Index=1;
            if(p2Index==2) p2Index=3;
        break;

        case sf::Keyboard::Scancode::Up:
            if(p2Index>=2) p2Index-=2;
        break;

        case sf::Keyboard::Scancode::Down:
            if(p2Index<=1) p2Index+=2;
        break;

        default:
        break;
    }

    UpdateCursorPositions();
}

void CharacterSelect::ConfirmP1()
{
    if(p1Index == p2Index && p2Ready)
        return;

    p1Ready = true;
    p1Result = fighterPaths[p1Index];
}

void CharacterSelect::ConfirmP2()
{
    if(p1Index == p2Index && p1Ready)
        return;

    p2Ready = true;
    p2Result = fighterPaths[p2Index];
}

void CharacterSelect::Draw(sf::RenderWindow& window)
{
    for(int i = 0; i < CHARACTER_COUNT; i++)
    {
        window.draw(sprites[i]);
    }

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
            {
                window.close();
                return;
            }

            if(auto key = event->getIf<sf::Event::KeyPressed>())
            {
                MoveP1(key->scancode);
                MoveP2(key->scancode);

                if(key->scancode == sf::Keyboard::Scancode::Space)
                    ConfirmP1();

                if(key->scancode == sf::Keyboard::Scancode::Enter)
                    ConfirmP2();
            }
        }

        window.clear(sf::Color(25,25,25));
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



