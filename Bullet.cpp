#include "Bullet.h"

Bullet::Bullet() {}

//***CONSTRUCTOR***
Bullet::Bullet(sf::Vector2f pos, float ang) {
    position = pos;
    speed = 2000;
    rect.setSize(sf::Vector2f(3, 8));
    rect.setRotation(ang);
    rect.setPosition(position);
    rect.setFillColor(sf::Color().White);
}


//***ANHADIR***
void Bullet::Add(sf::Vector2f pos, float ang) {
    bullets.push_back(Bullet(pos, ang));
}


//***ACTUALIZAR BALA***
void Bullet::individualUpdate(float delta, sf::RenderTarget& target) {
    time += delta;
    position.y -= (speed*delta) * cos(3.1415*rect.getRotation() / 180);
    position.x -= (speed*delta) * -sin(3.1415*rect.getRotation() / 180);
    rect.setPosition(position);
    target.draw(rect);
}


//***ACTUALIZAR TODAS LAS BALAS***
void Bullet::Updates(float delta, sf::RenderTarget& target) {
    if (bullets.size() != 0) {
        std::list<Bullet>::iterator temp;
        for (temp = bullets.begin(); temp != bullets.end(); temp++) {
            temp->individualUpdate(delta, target);
            if (temp->time > 0.4f) temp = bullets.erase(temp);
        }
    }
}