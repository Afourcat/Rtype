/*
** EPITECH PROJECT, 2019
** GameState.cpp
** File description:
** GameState implementation
*/

#include "states/GameState.hpp"
#include <iostream>

GameState::GameState(EntityBuilder &entityBuilder)
    : State(entityBuilder)
    , m_players()
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
    m_players.push_back(static_cast<Ship *>(m_entityBuilder.create(EntityType::SHIP)));
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
    for (auto &bullet : m_bullets) {
        bullet.update(*m_deltaTime);
        m_window->draw(bullet);
    }
    for (auto &player : m_players) {
        m_window->draw(*player);
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
                break;
            case sf::Event::Resized:
                m_window->setView(sf::View(sf::FloatRect(0, 0, static_cast<float>(event.size.width), static_cast<float>(event.size.height))));
            default:
                break;
        }
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        m_players.front()->shot(m_bullets);
    sf::Vector2f offset(0.0, 0.0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        offset += sf::Vector2f(0.0, -320 * (*m_deltaTime));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        offset += sf::Vector2f(-200 * (*m_deltaTime), 0.0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        offset += sf::Vector2f(0.0, 320 * (*m_deltaTime));
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        offset += sf::Vector2f(200 * (*m_deltaTime), 0.0);
    m_players.front()->move(offset);
    return Transition::NONE;
}
