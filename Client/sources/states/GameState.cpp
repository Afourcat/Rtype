/*
** EPITECH PROJECT, 2019
** GameState.cpp
** File description:
** GameState implementation
*/

#include "states/GameState.hpp"
#include <iostream>

GameState::GameState(TextureBuilder &textureBuilder)
    : State(textureBuilder)
    , m_player()
    , m_bullets()
{
    this->onStart();
}

GameState::~GameState()
{
    this->onStop();
}

void GameState::onStart()
{
    std::cout << "Entering GameState..." << std::endl;
    m_player.setTexture(m_textureBuilder.getTexture("../Client/assets/r-typesheet1.gif"));
}

void GameState::onStop()
{
    std::cout << "Leaving GameState..." << std::endl;
}

void GameState::onPause()
{
    m_isPaused = true;
}

void GameState::onResume()
{
    m_isPaused = false;
}

void GameState::update()
{
    if (m_isPaused)
        return;
    m_window->draw(m_player);
    for (Bullet &bullet : m_bullets) {
        bullet.move(sf::Vector2f(400 * (*m_deltaTime), 0.0));
        m_window->draw(bullet);
    }
}

Transition GameState::handleEvent(sf::Event &event)
{
    while (m_window->pollEvent(event)) {
        switch (event.type) {
            case sf::Event::Closed:
                return Transition::QUIT;
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape)
                return Transition::QUIT;
            default:
                break;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        m_bullets.push_back(m_player.shot());
    sf::Vector2f offset(0.0, 0.0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        offset += sf::Vector2f(0.0, -320 * (*m_deltaTime));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        offset += sf::Vector2f(-200 * (*m_deltaTime), 0.0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        offset += sf::Vector2f(0.0, 320 * (*m_deltaTime));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        offset += sf::Vector2f(200 * (*m_deltaTime), 0.0);
    m_player.move(offset);
    return Transition::NONE;
}
