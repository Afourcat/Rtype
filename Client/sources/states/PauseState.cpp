/*
** EPITECH PROJECT, 2019
** PauseState.cpp
** File description:
** PauseState implementation
*/

#include "states/PauseState.hpp"
#include <iostream>

PauseState::PauseState(TextureBuilder &textureBuilder)
    : State(textureBuilder)
{
    this->onStart();
}

PauseState::~PauseState()
{
    this->onStop();
}

void PauseState::onStart()
{
    std::cout << "Entering PauseState..." << std::endl;
}

void PauseState::onStop()
{
    std::cout << "Leaving PauseState..." << std::endl;
}

void PauseState::onPause()
{
    m_isPaused = true;
}

void PauseState::onResume()
{
    m_isPaused = false;
}

void PauseState::update()
{
    if (m_isPaused)
        return;
}

void PauseState::handleEvent() {}
