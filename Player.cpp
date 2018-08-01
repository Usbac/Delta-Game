#include "Player.h"

//***GETTERS & SETTERS***
sf::Vector2f Player::getPosition() { return position; }

void Player::setPosition(sf::Vector2f pos) { position = pos; }

float Player::getLastDamage() { return timeLastDamage; }

void Player::setLastDamage(float d) { timeLastDamage = d; }

float Player::getLastShoot() { return timeLastShoot; }

int Player::getHP() { return HP; }

void Player::setHP(int H) { HP = H; }

int Player::getExp() { return exp; }

void Player::setExp(int e) { exp = e; }

int Player::getDamage() { return damage; }

sf::FloatRect Player::getBounds() { return shape.getGlobalBounds(); }

float toRadians(float angle) { return 3.1415*angle / 180; }

//***CONSTRUCTOR***
Player::Player() {
    if (!texture.loadFromFile("Recursos/Nave.png")) std::cout << "ADV! Error al cargar textura de Nave" << std::endl;
    //Variables normales
    HP = 100;
    damage = 10;
    speed = 0;
    backSpeed = 400;
    timeLastShoot = 1;
    timeLastDamage = 1;
    exp = 0;
    //Sprite y Animacion
    rectangle.height = 30;
    rectangle.width = 30;
    animation.setTexture(texture);
    animation.setTextureRect(rectangle);
    animation.setOrigin((float)animation.getTextureRect().width / 2,
                        (float)animation.getTextureRect().height / 2);
    animation.setScale(2, 2);
    //Figura
    shape.setSize(sf::Vector2f(rectangle.width, rectangle.height));
    shape.setOrigin(animation.getTextureRect().width / 2,
                    animation.getTextureRect().height / 2);
    shape.setScale(animation.getScale());
    swipeTextureLeft = false;
}


//***MOVIMIENTO***
void Player::Movement(sf::RenderTarget& window) {
    rotation = animation.getRotation();
    //Movimiento
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        speed = 900;
        position.y -= (speed*delta) * cos(toRadians(rotation));
        position.x += (speed*delta) * sin(toRadians(rotation));
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        position.y += (backSpeed*delta) * cos(toRadians(rotation));
        position.x -= (backSpeed*delta) * sin(toRadians(rotation));
    }
    else if (speed > 0) {
        position.y -= (speed*delta) * cos(toRadians(rotation));
        position.x += (speed*delta) * sin(toRadians(rotation));
        speed -= delta * 3500;
    }
    //Rotacion
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) animation.rotate(delta * -300);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) animation.rotate(delta * 300);
    //Arreglar posicion
    if (position.x <= 0) position.x = 0;
    if (position.x >= window.getSize().x) position.x = window.getSize().x;
    if (position.y <= 0) position.y = 0;
    if (position.y >= window.getSize().y) position.y = window.getSize().y;
}


//***ACTUALIZAR***
void Player::Update(float d, sf::RenderTarget& window, Bullet &bullet) {
    //Variables
    delta = d;
    timeLastShoot += delta;
    timeLastDamage += delta;
    animation.setPosition(position);
    shape.setPosition(position);
    //Animacion
    if (rectangle.left == 120) swipeTextureLeft = true;
    if (rectangle.left == 0) swipeTextureLeft = false;
    if (clock.getElapsedTime().asSeconds() > 0.08f) {
        if (swipeTextureLeft) rectangle.left -= 30;
        else rectangle.left += 30;
        animation.setTextureRect(rectangle);
        clock.restart();
    }
    //Movimiento
    Player::Movement(window);
    //Disparo
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && timeLastShoot > 0.06f) {
        bullet.Add(position, rotation);
        timeLastShoot = 0;
    }
    //Impresion
    window.draw(animation);
}