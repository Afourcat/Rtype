/*
** EPITECH PROJECT, 2019
** Bullet.hpp
** File description:
** Bullet header
*/

#ifndef BULLET_HPP
	#define BULLET_HPP

#include "Entity.hpp"

class Bullet : public Entity {
    public:
        Bullet();
        Bullet(const sf::Texture *texture);
        ~Bullet() = default;
        void setTexture(const sf::Texture *texture);
        void setMovement(const sf::Vector2f &movement);
        void setMovement(const float &xMovement, const float &yMovement);
        void update(const float &deltaTime);
        void checkPosition();
        bool isOutOfMap();
    private:
        sf::Vector2f m_movement;
        size_t m_finished;
};

#endif /* !BULLET_HPP */
