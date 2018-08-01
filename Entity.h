#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"
#include "Bullet.h"
#include <list>

static float timeLastEnemyCreation, timeLastPackCreation;
static sf::Vector2f size;
//Texturas
static sf::Texture textureEnemy1, textureEnemy2, texturePack;

class Entity {
private:
    std::list<Entity> entities;
    //Variables generales
    int HP, speed, speedRotation, damage, type, orientation;
    float angle;
    sf::Vector2f position;
    //Sprite
    sf::Sprite image;
    sf::IntRect rectangle;
    sf::RectangleShape shape;
    bool swipeTextureLeft;
    //Otros
    sf::Clock clock;
    sf::Time time;
    enum Category { Enemy, Pack };
    Category category;

public:
    Entity();
    Entity(int tipo, int x, int y);
    void Add(int type, sf::RenderTarget& target);
    void individualUpdate(sf::RenderTarget& window, float delta, Player& player, Bullet& bullets);
    void Updates(sf::RenderTarget& window, float delta, Player& player, Bullet& bullets);
    void touchPlayer(Player& player);
    void Damage(Player& player, Bullet& bullet);
    void create(float delta, sf::RenderTarget &target, Player& player);
};