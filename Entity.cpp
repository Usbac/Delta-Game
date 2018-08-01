#include "Entity.h"


//***CONSTRUCTOR***
Entity::Entity() {
    if (!textureEnemy1.loadFromFile("Recursos/Enemigo1.png")) std::cout << "ADV! Error al cargar textura del enemigo 1" << std::endl;
    if (!textureEnemy2.loadFromFile("Recursos/Enemigo2.png")) std::cout << "ADV! Error al cargar textura del enemigo 2" << std::endl;
    if (!texturePack.loadFromFile("Recursos/Pack.png")) std::cout << "ADV! Error al cargar textura del Pack" << std::endl;
    timeLastEnemyCreation = 0;
    timeLastPackCreation = 0;
}


//***CONSTRUCTOR ENTIDAD***
Entity::Entity(int t, int x, int y) {
    category = Enemy;
    switch (t) {
        case 1: //>Enemigo tipo 1
            image.setTexture(textureEnemy1);
            speedRotation = 150;
            speed = 150;
            damage = 5;
            break;
        case 2: //>Enemigo tipo 2
            image.setTexture(textureEnemy2);
            speedRotation = 70;
            speed = 100;
            damage = 10;
            break;
        case 3: //>Pack de Salud
            image.setTexture(texturePack);
            speedRotation = 30;
            speed = 100;
            category = Pack;
            break;
    }
    //Variables normales
    type = t;
    HP = 100;
    position = sf::Vector2f(x, y);
    rectangle.width = 30;
    rectangle.height = 30;
    float size = (t == 1 || t == 2) ? 1.6f : 1.2f;
    //Imagen y figura
    image.setScale(size, size);
    image.setTextureRect(rectangle);
    image.setOrigin(image.getTextureRect().width / 2, image.getTextureRect().height / 2);
    image.setPosition(position);
    shape.setSize(sf::Vector2f(rectangle.width, rectangle.height));
    shape.setOrigin(image.getTextureRect().width / 2, image.getTextureRect().height / 2);
    shape.setScale(image.getScale());
    swipeTextureLeft = false;
}


//***GENERAR ENTIDAD***
void Entity::create(float delta, sf::RenderTarget &target, Player& player) {
    timeLastEnemyCreation += delta;
    timeLastPackCreation += delta;
    //Crear Enemigos
    if (timeLastEnemyCreation > 2) {
        timeLastEnemyCreation = 0;
        Entity::Add(1, target);
        Entity::Add(2, target);
    }
    //Crear Pack Salud
    if (timeLastPackCreation > 15) {
        timeLastPackCreation = 0;
        Entity::Add(3, target);
    }
}


//***ANHADIR ENTIDAD A LA LISTA***
void Entity::Add(int type, sf::RenderTarget& target) {
    int tempOrientation = rand() % 4;
    int x = 1000, y = 1000;
    switch (tempOrientation) {
        case 0: //>Abajo
            x = rand() % target.getSize().x;
            y = target.getSize().y*-1.5f;
            break;
        case 1: //>Arriba
            x = rand() % target.getSize().x;
            y = target.getSize().y*1.5f;
            angle = 0;
            break;
        case 2: //>Derecha
            x = target.getSize().x*1.5f;
            y = rand() % target.getSize().y;
            break;
        case 3: //>Izquierda
            x = target.getSize().x*-1.5f;
            y = rand() % target.getSize().y;
            break;
    }
    //Creacion de Entidad y adicion a Lista
    Entity nuevo = Entity(type, x, y);
    nuevo.orientation = tempOrientation;
    entities.push_back(nuevo);
}


//***ACCIONARSE AL TOCAR JUGADOR***
void Entity::touchPlayer(Player& player) {
    if (player.getBounds().intersects(shape.getGlobalBounds())) {
        //Accionar enemigo (Ataque/Danho a jugador)
        if (category==Enemy) {
            player.setHP(player.getHP()-damage);
            player.setExp(player.getExp()-20);
            player.setLastDamage(0);
        }
        //Accionar HP Pack (Vida a jugador)
        else {
            player.setHP(player.getHP()+20);
            player.setExp(player.getExp()+30);
        }
        HP = 0;
    }
}


//***DANHO A LA ENTIDAD***
void Entity::Damage(Player& player, Bullet& bullet) {
    if (bullet.bullets.size() != 0) {
        std::list<Bullet>::iterator temp;
        for (temp = bullet.bullets.begin(); temp != bullet.bullets.end(); temp++) {
            if (temp->rect.getGlobalBounds().intersects(shape.getGlobalBounds())) {
                HP -= player.getDamage();
                player.setExp(player.getExp()+2);
            }
        }
    }
}


//***ACTUALIZAR ENTIDAD***
void Entity::individualUpdate(sf::RenderTarget& window, float delta, Player& player, Bullet& bullets) {
    int distanceX = position.x - player.getPosition().x;
    int distanceY = position.y - player.getPosition().y;
    //MOVIMIENTO

    //>Enemigo
    if (category==Enemy) {
        if (distanceX > 20) position.x -= delta*speed;
        if (distanceX < -20) position.x += delta*speed;
        if (distanceY > 20) position.y -= delta*speed;
        if (distanceY < -20) position.y += delta*speed;
    }
    //>Pack salud
    else {
        switch (orientation) {
            case 0: position.y += speed*delta; break;
            case 1: position.y -= speed*delta; break;
            case 2: position.x -= speed*delta; break;
            case 3: position.x += speed*delta; break;
        }
    }
    Entity::touchPlayer(player);
    //Rotar y actualizar posicion
    image.rotate(delta * speedRotation);
    image.setPosition(position);
    shape.setPosition(position);
    //Animacion sprite
    if (clock.getElapsedTime().asSeconds() > 0.1f) {
        if (swipeTextureLeft) rectangle.left -= 30;
        else rectangle.left += 30;
        image.setTextureRect(rectangle);
        clock.restart();
    }
    //Arreglar animacion sprite
    if (rectangle.left == 120) swipeTextureLeft = true;
    if (rectangle.left == 0) swipeTextureLeft = false;
    //Danho
    if (type != 3) Damage(player, bullets);
    //Imprimir
    window.draw(image);
}


//***ACTUALIZAR TODAS LAS ENTIDADES***
void Entity::Updates(sf::RenderTarget& window, float delta, Player& player, Bullet& bullets) {
    Entity::create(delta, window, player);
    if (entities.size() != 0) {
        std::list<Entity>::iterator aux;
        for (aux = entities.begin(); aux != entities.end(); aux++) {
            aux->individualUpdate(window, delta, player, bullets);
            if (aux->HP <= 0) aux = entities.erase(aux);
        }
    }
}