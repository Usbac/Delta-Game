#ifndef BULLET_H
#define BULLET_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <cstddef>
#include <list>

static int speed;

class Bullet {
public:
    std::list<Bullet> bullets;
    float time;
    sf::RectangleShape rect;
    sf::Vector2f position;

    Bullet();
    Bullet(sf::Vector2f pos, float ang);
    void Add(sf::Vector2f pos, float ang);
    void individualUpdate(float delta, sf::RenderTarget& target);
    void Updates(float delta, sf::RenderTarget& target);
};
#endif // !BULLET_H