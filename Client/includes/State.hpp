/*
** EPITECH PROJECT, 2019
** State.hpp
** File description:
** State header
*/

#ifndef STATE_HPP
	#define STATE_HPP

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "TextureBuilder.hpp"

enum States {
    EMPTY,
    LOAD,
    MENU,
    PAUSE,
    GAME
};

class State {
    public:
        State(TextureBuilder &textureBuilder)
        : m_window(nullptr), m_textureBuilder(textureBuilder), m_event(), m_isPaused(false) {};
        virtual ~State() {};
        void linkWindow(sf::RenderWindow *window) { m_window = window; };
        virtual void onStart() = 0;
        virtual void onStop() = 0;
        virtual void onPause() = 0;
        virtual void onResume() = 0;
        virtual void update() = 0;
        virtual void handleEvent() = 0;
    protected:
        sf::RenderWindow *m_window;
        TextureBuilder &m_textureBuilder;
        sf::Event m_event;
        bool m_isPaused;
};

#endif /* !STATE_HPP */
