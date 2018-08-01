#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <list>
#include "Player.h"
#include "Entity.h"

enum game_state {
    Playing, DeathScreen, Menu
};

float delta;
sf::Texture imageBackground, imageSelector, imageTitle;
sf::Sprite background, selector, title;
sf::Font text;
sf::Text HP, options, deathText;
static Player player;
Bullet bullet;
Entity entities;
sf::Vector2f center;
game_state state;

sf::SoundBuffer bufferHit, bufferShoot;
sf::Sound soundHit, soundShoot;


//***INICIALIZAR TODO***
void initialize(sf::RenderWindow &window) {
	player = Player();
	entities = Entity();
    state = Menu;
	//Fuente y texturas
	if (!text.loadFromFile("Recursos/fixedsys.ttf")) std::cout << "Error al cargar fuente" << std::endl;
	if (!imageSelector.loadFromFile("Recursos/selector.png")) std::cout << "Error al cargar textura del selector" << std::endl;
	if (!imageBackground.loadFromFile("Recursos/space.jpeg")) std::cout << "Error al cargar textura del fondo" << std::endl;
	if (!imageTitle.loadFromFile("Recursos/Titulo.png")) std::cout << "Error al cargar textura del titulo" << std::endl;
	title.setTexture(imageTitle);
	background.setTexture(imageBackground);
	imageSelector.setSmooth(false);
	selector.setTexture(imageSelector);
	selector.setScale(2, 2);
	const_cast <sf::Texture&> (text.getTexture(24)).setSmooth(false);
	center = sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2);
	//Texto de HP y Titulo
	HP.setFont(text);
	HP.setPosition(10, 10);
	deathText.setFont(text);
	deathText.setScale(1.2f, 1.2f);
	deathText.setString("Has muerto \nExp: ");
	deathText.setPosition(center.x - deathText.getGlobalBounds().width / 2, center.y - deathText.getGlobalBounds().height);
	//Opciones
	options.setFont(text);
	options.setString("Iniciar Juego \nSalir");
	options.setPosition(center.x - (title.getGlobalBounds().width*0.11), center.y);
	//Inicializar posicion jugador
	player.setPosition(center);
	//Variables & Off-Topic
	title.setPosition(center.x - (title.getGlobalBounds().width*0.14f), center.y - title.getGlobalBounds().height / 3);
	title.scale(0.3f, 0.3f);
	selector.setPosition(center.x - (title.getGlobalBounds().width*0.4), center.y + 12);
}


//***MENU***
void menu(sf::RenderWindow& window) {
	window.draw(options);
	window.draw(selector);
	window.draw(title);
    int exitOption = center.y + 45,
        playOption = center.y + 12;
	//Movimiento
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        selector.setPosition(center.x - (title.getGlobalBounds().width*0.4), playOption);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        selector.setPosition(center.x - (title.getGlobalBounds().width*0.4), exitOption);
	//Seleccionar
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) {
	        if (selector.getPosition().y == exitOption || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			window.close();
		else
            state = Playing;
	}
}


//***ACTUALIZACION TOTAL JUGADOR***
void deathScreen(sf::RenderWindow& window) {
	window.draw(deathText);
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		player = Player();
		state = Menu;
	}
}


//***VERIFICAR MUERTE***
void playerUpdate(sf::RenderWindow& window) {
	//Sonido
    if (player.getLastShoot()==0) soundShoot.play();
    if (player.getLastDamage()==0) soundHit.play();
    //Actualizacion general
    player.Update(delta, window, bullet);
    //Muerte
	if (player.getHP() <= 0) {
		state = DeathScreen;
		deathText.setString("HAS MUERTO! \nExp: " + std::to_string(player.getExp()));
	} 
}


//***J U E G O***
void mainLoop(sf::RenderWindow& window) {
	window.draw(background);
	//Actualizar elementos
    playerUpdate(window);
	bullet.Updates(delta, window);
	entities.Updates(window, delta, (player), bullet);
	//Texto Salud y Verificacion Muerte
	HP.setString("HP " + std::to_string(player.getHP()));
	window.draw(HP);
}


//***MAIN***
int main() {
    sf::RenderWindow window(sf::VideoMode(1100, 680), "Delta - Juego indie");
	window.setFramerateLimit(60);
    sf::Clock clock;
    sf::Time time;
	initialize(window);
    if (!bufferHit.loadFromFile("Recursos/damage.wav")) std::cout << "Error al cargar sonido" << std::endl;
    if (!bufferShoot.loadFromFile("Recursos/Disparo.wav")) std::cout << "Error al cargar sonido" << std::endl;
    soundHit.setBuffer(bufferHit);
	soundShoot.setBuffer(bufferShoot);

	//**********GAME LOOP**********
	while (window.isOpen()) {
        sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) window.close();
		}
		window.clear();
		time = clock.restart();
		delta = time.asSeconds();
        switch (state) {
            case Playing: mainLoop(window);
                break;
            case Menu: menu(window);
                break;
            case DeathScreen: deathScreen(window);
                break;
        }
		window.display();
	}
	return 0;
}
