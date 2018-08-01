#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Bullet.h"

static sf::Texture texture;

class Player {
private:
    //Variables generales
    int HP, speed, backSpeed, damage, exp;
    float rotation, delta, timeLastShoot, timeLastDamage;
    sf::Vector2f position;
    //Textura y Sprite
    sf::Sprite animation;
    sf::RectangleShape shape;
    sf::IntRect rectangle;
    bool swipeTextureLeft;
    sf::Clock clock;

public:
    //Getters & Setters
    sf::Vector2f getPosition();
    void setPosition(sf::Vector2f pos);
    float getLastDamage();
    void setLastDamage(float lastDamage);
    float getLastShoot();
    int getHP();
    void setHP(int hp);
    int getExp();
    void setExp(int exp);
    int getDamage();
    sf::FloatRect getBounds();
    Player();
    void Movement(sf::RenderTarget& window);
    void Update(float delta, sf::RenderTarget& window, Bullet &bullet);
};
#endif // !PLAYER_H