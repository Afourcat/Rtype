/*
** EPITECH PROJECT, 2019
** Game.cpp
** File description:
** Game implementation
*/

#include "Game.hpp"
#include <iostream>

Game::Game()
    : m_textureBuilder()
    , m_stateBuilder()
    , m_states()
    , m_isRunning(false)
    , m_window(nullptr)
{
}

Game::~Game()
{
    while (!m_states.empty()) {
        delete m_states.top();
        m_states.pop();
    }
}

void Game::run()
{
    m_window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME);
    m_states.push(m_stateBuilder.createState(States::GAME, m_textureBuilder));
    m_states.top()->linkWindow(m_window);
    m_isRunning = true;
    this->loop();
}

void Game::loop()
{
    while (m_isRunning && m_window->isOpen()) {
        m_window->clear();
        m_states.top()->handleEvent();
        m_states.top()->update();
        m_window->display();
    }
}
