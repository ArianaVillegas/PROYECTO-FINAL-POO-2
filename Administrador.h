#pragma once

#include <iostream> //Para testear y debugear

#include <algorithm>
#include <vector> //Contenedores

#include <stdlib.h>
#include <time.h> //Conseguir el tiempo

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp> //Librería gráfica (eventos: mensaje de programación reactiva)

#include "Lista.h" //Estructura simple y especializada en iterar por todos los elementos

#include "Componente.h" //Componentes del juego (observador de programación reactiva)


void DetectarColiciones(sf::Sprite Object1, sf::Sprite Object2, bool& Collision) {
	if (Collision::PixelPerfectTest(Object1, Object2)) { Collision = true; }
}

class Administrador {
	//Ventana
	sf::RenderWindow* window;

	//Tiempo global
	const float unidad_clock = 0.03f;
	sf::Clock clock;

	//componentes
	Lista<Componente*> componentes;

	//Posiciones permitidas (las tres primeras son las posiciones de los jugadores, las otras de los hechizos)
	std::vector<std::vector<int>> posiciones = {
		{144, 280},
		{1365, 849},
		{637, 856}
	};

	//Obstaculos
	std::vector < std::vector<float> > obstaculos = { 
		{ 260, 260, 782, 350, 45 }, 
		{ 390, 95, 392, 335, 90 }, 
		{ 390, 95, 1264, 350, 90 } 
	};

	//Controles por defecto para tres jugadores máximo (se puede agregar más pero se necesita un teclado más grande)
	std::vector<std::vector<sf::Keyboard::Key>> controles = {
		{sf::Keyboard::Up, sf::Keyboard::Down, sf::Keyboard::Left, sf::Keyboard::Right, sf::Keyboard::Numpad1},
		{sf::Keyboard::W, sf::Keyboard::S, sf::Keyboard::A, sf::Keyboard::D, sf::Keyboard::Q},
		{sf::Keyboard::U, sf::Keyboard::J, sf::Keyboard::H, sf::Keyboard::K, sf::Keyboard::Y}
	};

	//componentes
	std::vector<std::vector<std::vector<std::string>>> players_sprites = {
		{
		{"Player 1/idle0.png","Player 1/idle1.png", "Player 1/idle2.png", "Player 1/idle1.png"},
		{"Player 1/walk0.png","Player 1/walk1.png", "Player 1/walk0.png","Player 1/walk2.png", "Player 1/walk3.png","Player 1/walk2.png" },
	},
		{
		{"Player 2/idle0.png","Player 2/idle1.png", "Player 2/idle2.png", "Player 2/idle1.png"},
		{"Player 2/walk0.png","Player 2/walk1.png", "Player 2/walk0.png","Player 2/walk2.png", "Player 2/walk3.png","Player 2/walk2.png" },
	},
		{
		{"Player 3/idle0.png","Player 3/idle1.png", "Player 3/idle2.png", "Player 3/idle1.png"},
		{"Player 3/walk0.png","Player 3/walk1.png", "Player 3/walk0.png","Player 3/walk2.png", "Player 3/walk3.png","Player 3/walk2.png" },
	}
	};
	std::vector<std::vector<std::string>> fondo_sprites = { {"Background/0.png","Background/1.png"} };

	//Valores de libro
	const int lim_inf_x = 225;
	const int lim_sup_x = 1350;
	const int lim_inf_Y = 185;
	const int lim_sup_Y = 1380;
	int ciclos_para_libros = 5;
	int ciclos_minimus_libros = 50;
	int ciclos_maximus_libros = 150;

	//Valores del fondo y pantalla
	const int screen_ancho = 1499;
	const int screen_alto = 1061;
	const int rapidez_fondo = 2;

	//Valores del jugador
	const int velocidad_mago = 3;
	const int rapidez_ani_mago = 1;
	const int ancho_mago = 161;
	const int alto_mago = 168;
	const int n_jugadores = 3;
	const int vida_jugador = 100;

public:
	//Crear Obstaculos
	void crear_obsta_culos() {
		for (int i = 0; i < n_jugadores; i++) {
			componentes.agregar(new Obstaculo(componentes, window, obstaculos[i][0], obstaculos[i][1], obstaculos[i][2], obstaculos[i][3], obstaculos[i][4]));
		}
	}

	//Detectar Colisiones
	void detectar_colisiones() {
		componentes.comparar_todos([](Componente* a, Componente* b) { if (a->DeterminarColision) { a->DeterminarColision(b); }  });
	}
	
	//Tiempo
	void actualizar_tiempo() {
		if (clock.getElapsedTime().asSeconds() > unidad_clock) {
			componentes.para_cada([](Componente* d) {if (d->AlCulminarCicloDeReloj) { d->AlCulminarCicloDeReloj(); }  });
			detectar_colisiones();
			if (ciclos_para_libros <= 0) { crear_libro(); }
			else { ciclos_para_libros--; }
			componentes.eliminar();
			clock.restart();
		}
	}

	//Fondo
	void crear_fondo() {
		componentes.agregar(new Fondo(rapidez_fondo,fondo_sprites,componentes,window));

	}

	//Libros
	void crear_libro() {
		int x = (rand() % (lim_sup_x - lim_inf_x)) + lim_inf_x;
		int y = ( rand() % (lim_sup_Y - lim_inf_Y) ) + lim_inf_Y;
		componentes.agregar(new Libros(componentes, window, x, y));
		ciclos_para_libros = (rand() % (ciclos_maximus_libros - ciclos_minimus_libros)) + ciclos_minimus_libros;
	}

	//Magos
	void crear_magos() {
		for (int i = 0; i < n_jugadores; i++) {
			//int random = rand() % posiciones.size();
			componentes.agregar(new Jugador(controles[i][0], controles[i][1], controles[i][2], controles[i][3], controles[i][4], i, posiciones[i][0], posiciones[i][1], ancho_mago, alto_mago, velocidad_mago, rapidez_ani_mago, players_sprites[i], vida_jugador, componentes, window));
		}
	}

	//Dibujar
	void dibujar() {
		componentes.para_cada([](Componente* d) {if (d->Dibujar) { d->Dibujar(); } });
	}

	//Juego
	Administrador()
	{
		//Al iniciar
		srand(time(NULL));
		window = new sf::RenderWindow(sf::VideoMode(screen_ancho, screen_alto), "Battle of Spells");
		window->setVerticalSyncEnabled(true);
		window->setKeyRepeatEnabled(false);
		crear_fondo();
		crear_obsta_culos();
		crear_magos();
		
		sf::Music music;
		if (!music.openFromFile("MUSIC.ogg")) {}
		music.setLoop(true);
		music.play();
		
		//Repetir
		while (window->isOpen()) {
			sf::Event event;
			while (window->pollEvent(event)) {

				if (event.type == sf::Event::Closed)
					window->close();

				if (event.type == sf::Event::KeyPressed) {
					componentes.para_cada([&event](Componente* d) {if (d->AlPresionarTecla) { d->AlPresionarTecla(event.key.code); } });
				}
				if (event.type == sf::Event::KeyReleased) {
					componentes.para_cada([&event](Componente* d) {if (d->AlLiberarTecla) { d->AlLiberarTecla(event.key.code); } });
				}


			}
			window->clear();
			actualizar_tiempo();
			dibujar();
			window->display();

		}
	}
};
