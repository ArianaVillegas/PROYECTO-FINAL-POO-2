#pragma once

#include <iostream> //Para testear y debugear

#include <functional> //Puntero a funciones

#include <vector>
#include <stack>
#include <algorithm>//Contenedores

#include <thread>

#include <random>

#include "Lista.h" //Estructura simple y especializada en iterar por todos los elementos

#include <SFML/Graphics.hpp> //Librería gráfica

#include "Tabla_Hechizos.h" //Tabla de valores

#include "Colisiona.h" //Detectar colisiones


struct Componente {
	//Atributos indispensables
	sf::RenderWindow* w;
	Lista<Componente*>* Componentes;
	bool eliminar = false;

	//Función de destruir
	void Destruir() { eliminar = true; };

	//Funciones de tecla/raton
	std::function<void(sf::Keyboard::Key)> AlPresionarTecla;
	std::function<void(sf::Keyboard::Key)> AlLiberarTecla; 
	std::function<void()> AlPresionarRaton;

	//Funciones de dibujo
	std::function<void()> Dibujar;
	std::function<sf::Sprite()> GetSprite;

	//Función de tiempo: animación y hechizo (proximamente libro)
	std::function<void()> AlCulminarCicloDeReloj;
	
	//Función de movil
	std::function<int()> GetId = []() {return -1; };

	//Funciones de hechizo
	std::function<int()> Getn_Hechizo;
	
	//Funciones de jugador
	std::function<void()> MoverAlRevez;

	//Funciones de identificacion
	std::function<int()> QuienSoy = []() {return -1; }; //0: obstaculo, 1: hechizo, 2:player

	//Funciones de colision
	std::function<void(Componente*)>  DeterminarColision;

};

typedef Lista<Componente*>& lista_componentes;

struct Dibujo: public Componente {
	sf::Sprite sprite;
	Dibujo(lista_componentes componentes_, sf::RenderWindow* w_) { 
		Componentes = &componentes_;  
		w = w_; 
		Dibujar = [this]() {w->draw(sprite); };
		GetSprite = [this]() {return sprite; };
	};
};



struct Animacion: public Dibujo {
	int reloj = 0;
	std::vector<std::vector<sf::Texture>> animaciones;
	
	int rapidez_animacion;
	int sprite_actual = 0;
	int animacion_actual = 0;

	void animar() {
		reloj++;
		if ((reloj > rapidez_animacion)) {

			sprite.setTexture(animaciones[animacion_actual][sprite_actual]);
			sprite_actual++;
			if (sprite_actual >= animaciones[animacion_actual].size()) {
				sprite_actual = 0;
			}
			reloj = 0;
		}
	}


	Animacion(int rapidez_animacion, std::vector<std::vector<std::string>> archivos, lista_componentes componentes, sf::RenderWindow* w) : Dibujo{componentes, w},
		rapidez_animacion{ rapidez_animacion }{

		//Carga imagenes
		for (auto archivo : archivos) {
			std::vector<sf::Texture> ani_temp;
			sf::Texture spr_temp;
			std::for_each(begin(archivo), end(archivo), [&ani_temp, &spr_temp](std::string imagen) {Collision::CreateTextureAndBitmask(spr_temp, imagen); ani_temp.push_back(spr_temp); });
			animaciones.push_back(ani_temp);
		}

		AlCulminarCicloDeReloj = [this]() {animar(); };

	}

};

struct Movil : public Animacion {
	int id;
	int x;
	int y;

	int durabilidad;

	int rapidez;
	bool Movimiento[4] = { false,false,false,false };

	void establecer_angulo_hacia_movimiento() {
		if (Movimiento[2]) { sprite.setRotation(270.0f); } //--Left
		if (Movimiento[3]) { sprite.setRotation(90.0f); } //--Right
		if (Movimiento[0]) { sprite.setRotation(360.0f); } //--Up
		if (Movimiento[1]) { sprite.setRotation(180.0f); } //--Down

		if (Movimiento[1] && Movimiento[2]) sprite.setRotation(225.0f); //--Down--Left
		if (Movimiento[1] && Movimiento[3]) sprite.setRotation(135.0f); //--Down--Right
		if (Movimiento[0] && Movimiento[2]) sprite.setRotation(315.0f); //--Up--Left
		if (Movimiento[0] && Movimiento[3]) sprite.setRotation(45.0f); //--Up--Right
	}

	virtual void delimitar() {
		if (x < 0) { x = 0; }
		else if (x > 1499) { x = 1499; }

		if (y < 0) { y = 0; }
		else if (y > 1061) { y = 1061; }
	}


	virtual void mover() {
		
		if (Movimiento[2]) { x -= rapidez; } //--Left
		if (Movimiento[3]) { x += rapidez; } //--Right
		if (Movimiento[0]) { y -= rapidez; } //--Up
		if (Movimiento[1]) { y += rapidez; } //--Down

		delimitar();

		sprite.setPosition(x, y);
		//sf::sleep(sf::milliseconds(0.5));
		
	}

	Movil(int id, int x, int y, float w, float h, int rapidez, int rapidez_animacion, std::vector<std::vector<std::string>> archivos, int durabilidad, lista_componentes componentes, sf::RenderWindow* window) :
		id{id}, x{ x }, y{ y }, rapidez{ rapidez }, durabilidad{ durabilidad }, Animacion{ rapidez_animacion, archivos, componentes, window }
	{
		sprite.setOrigin(w / 2., h / 2.);
		sprite.setPosition(x, y);
		Dibujar = [this]() {this->mover(); this->w->draw(sprite); };
		GetId = [this]() {return this->id; };

	};
	
};


struct Hechizo : public Movil {
	int n_hechizo;

	Hechizo(int n_hechizo, bool _Movimiento[4], int id, int x, int y, float w, float h, int rapidez, int rapidez_animacion, std::vector<std::vector<std::string>> archivos, int durabilidad, lista_componentes componentes, sf::RenderWindow* window) :
		n_hechizo{ n_hechizo }, Movil {
	   id, x, y, w, h, rapidez, rapidez_animacion, archivos, durabilidad, componentes, window
	} {
		AlCulminarCicloDeReloj = [this]() {this->durabilidad--; animar(); if (this->durabilidad <= 0) { Destruir(); } };

		Getn_Hechizo = [this]() {
			return this->n_hechizo;
		};
		QuienSoy = []() {return 1; };

		for (int i = 0; i < 4; i++) {
			Movimiento[i] = _Movimiento[i];
		}
		establecer_angulo_hacia_movimiento();
	}

	void delimitar()override {
		if (x < 0) { Destruir(); }
		else if (x > 1499) { Destruir(); }

		if (y < 0) { Destruir(); }
		else if (y > 1061) { Destruir(); }
	}
};


struct Jugador : public Movil {
	std::stack<int> hechizos;
	sf::Keyboard::Key arriba;
	sf::Keyboard::Key abajo;
	sf::Keyboard::Key izquierda;
	sf::Keyboard::Key derecha;
	sf::Keyboard::Key ataque;
	sf::Texture vidaTexture;
	sf::Sprite vidaSprite;
	int ultimo_movimiento = 0;
	short espera = 5;
	short Paralisis = 0;
	bool estaParalizado = false;

	void mover() override {
		if (!estaParalizado) {
			if (Movimiento[2]) { x -= rapidez; } //--Left
			if (Movimiento[3]) { x += rapidez; } //--Right
			if (Movimiento[0]) { y -= rapidez; } //--Up
			if (Movimiento[1]) { y += rapidez; } //--Down

			delimitar();

			sprite.setPosition(x, y);
			//sf::sleep(sf::milliseconds(0.5));
		}
	}

	void seleccionar_animacion(){
		if ((Movimiento[0] || Movimiento[1] || Movimiento[2] || Movimiento[3])) { if (animacion_actual != 1) { animacion_actual = 1; sprite_actual = 0; } }
		else { if (animacion_actual != 0) { animacion_actual = 0; sprite_actual = 0; } }
	}

	void iniciar_accion(sf::Keyboard::Key k) {
		if (k == arriba) {
			Movimiento[0] = true; seleccionar_animacion(); establecer_angulo_hacia_movimiento();
			
		}
		else {
			if (k == abajo) {
				Movimiento[1] = true; seleccionar_animacion(); establecer_angulo_hacia_movimiento();
				
			}
			else {
				if (k == izquierda) {
					Movimiento[2] = true; seleccionar_animacion(); establecer_angulo_hacia_movimiento();
					
				}
				else {
					if (k == derecha) {
						Movimiento[3] = true; seleccionar_animacion(); establecer_angulo_hacia_movimiento();

					}
					else {
						if (k == ataque) {
							if (!(hechizos.empty())) {
								if ((Movimiento[0] || Movimiento[1] || Movimiento[2] || Movimiento[3])) {
									Componentes->agregar(new Hechizo(hechizos.top(), Movimiento, this->id, x, y, tabla_hechizos[hechizos.top()].w, tabla_hechizos[hechizos.top()].h, tabla_hechizos[hechizos.top()].rapidez, tabla_hechizos[hechizos.top()].rapidez_animacion, tabla_hechizos[hechizos.top()].sprites, tabla_hechizos[hechizos.top()].durabilidad, *Componentes, w));
								}
								else {
									bool temp[4] = { false, false, false ,false };
									temp[ultimo_movimiento] = true;
									Componentes->agregar(new Hechizo(hechizos.top(), temp, this->id, x, y, tabla_hechizos[hechizos.top()].w, tabla_hechizos[hechizos.top()].h, tabla_hechizos[hechizos.top()].rapidez, tabla_hechizos[hechizos.top()].rapidez_animacion, tabla_hechizos[hechizos.top()].sprites, tabla_hechizos[hechizos.top()].durabilidad, *Componentes, w));
								}
								
								hechizos.pop();
							}
						}
					}
				}
			}
		}
	};
	void terminar_accion(sf::Keyboard::Key k) {
		if (k == arriba) {
			Movimiento[0] = false; seleccionar_animacion(); establecer_angulo_hacia_movimiento();
			ultimo_movimiento = 0;
		}
		else {
			if (k == abajo) {
				Movimiento[1] = false; seleccionar_animacion(); establecer_angulo_hacia_movimiento();
				ultimo_movimiento = 1;
			}
			else {
				if (k == izquierda) {
					Movimiento[2] = false; seleccionar_animacion(); establecer_angulo_hacia_movimiento();
					ultimo_movimiento = 2;
				}
				else {
					if (k == derecha) {
						Movimiento[3] = false; seleccionar_animacion(); establecer_angulo_hacia_movimiento();
						ultimo_movimiento = 3;
					}
				}
			}
		}
	};
	
	void recibirEfecto(int n_hech) {
			durabilidad += tabla_hechizos[n_hech].poder;
			vidaSprite.setTextureRect(sf::IntRect(0, 0, 2*durabilidad, 20));
			if (tabla_hechizos[n_hech].paralisis > 0) { Paralisis = tabla_hechizos[n_hech].paralisis; estaParalizado = true; }
			if (durabilidad <= 0) { Destruir(); }
	}

	Jugador(sf::Keyboard::Key arriba, sf::Keyboard::Key abajo, sf::Keyboard::Key izquierda, sf::Keyboard::Key derecha, sf::Keyboard::Key ataque, int id, int x, int y, float w, float h, int rapidez, int rapidez_animacion, std::vector<std::vector<std::string>> archivos, int durabilidad, lista_componentes componentes, sf::RenderWindow* window):
		arriba {arriba}, abajo{ abajo }, izquierda{ izquierda }, derecha{ derecha }, ataque{ ataque }, Movil{
		id, x, y, w, h, rapidez, rapidez_animacion, archivos, durabilidad, componentes, window
	} {
		Collision::CreateTextureAndBitmask(vidaTexture, "Rectangles.png");
		vidaSprite.setTexture(vidaTexture);
		vidaSprite.setTextureRect(sf::IntRect(0, 0, 2*durabilidad, 20));
		switch (this->id)
		{
		case 0:
			vidaSprite.setPosition(300, 150);
			vidaSprite.setColor(sf::Color::Green);
			break;
		case 1:
			vidaSprite.setPosition(630, 150);
			vidaSprite.setColor(sf::Color::Blue);
			break;
		case 2:
			vidaSprite.setPosition(1000, 150);
			vidaSprite.setColor(sf::Color::Red);
			break;
		}
		
		AlCulminarCicloDeReloj = [this]() { animar(); if (estaParalizado) { Paralisis--; if (Paralisis == 0) { estaParalizado = false; } }; };
		AlPresionarTecla = [this](sf::Keyboard::Key k) {iniciar_accion(k); };
		AlLiberarTecla = [this](sf::Keyboard::Key k) {terminar_accion(k); };
		QuienSoy = []() {return 2; };
		Dibujar = [this]() {this->mover(); this->w->draw(sprite); this->w->draw(this->vidaSprite); };
		DeterminarColision = [this, id](Componente* c) {
			if (c->QuienSoy() == 1) { 
			if (this->GetSprite && c->GetSprite) {
				if (siColisionan(this->GetSprite(), c->GetSprite())) {
					if (id != c->GetId()) {
						int n = c->Getn_Hechizo();
						if (tabla_hechizos[n].ataque) {
							if (espera == 0) { c->Destruir(); recibirEfecto(n); espera = 5; }
							else { espera--; }
						}
					}
					else {
						int n = c->Getn_Hechizo();
						if (!(tabla_hechizos[n].ataque)) {
							if (espera == 0) { c->Destruir(); recibirEfecto(n); espera = 5; }
							else { espera--; }
						}
					}
				}
			}
		}
			if (c->QuienSoy() == 3) {
				if (this->GetSprite && c->GetSprite) {
					if (siColisionan(this->GetSprite(), c->GetSprite())) {
						hechizos.push(rand()%tabla_hechizos.size());	//rand() % numHechuzos
						c->Destruir();
					}
				}
			}
		};
			

		MoverAlRevez = [this, rapidez]() {
			if (Movimiento[2]) { this->x -= 2*rapidez; } //--Left
			if (Movimiento[3]) { this->x += 2*rapidez; } //--Right
			if (Movimiento[0]) { this->y -= 2*rapidez; } //--Up
			if (Movimiento[1]) { this->y += 2*rapidez; } //--Down

			sprite.setPosition(this->x, this->y);
			//sf::sleep(sf::milliseconds(0.5));
		};

	};
};

struct Obstaculo : public Dibujo {
	sf::Texture MapTexture;

	Obstaculo(lista_componentes componentes_, sf::RenderWindow* w_, float h, float w, float x, float y, float rot): Dibujo(componentes_, w_) {
		
		Collision::CreateTextureAndBitmask(MapTexture, "Rectangles.png");
		sprite.setTexture(MapTexture);

		sprite.setTextureRect(sf::IntRect(0, 0, h, w));
		sprite.setPosition(x, y);
		sprite.setRotation(rot);
		sprite.setColor(sf::Color::Transparent);
		QuienSoy = []() {return 0; };
		DeterminarColision = [this](Componente* c) {if (c->QuienSoy() == 2) {
			if (this->GetSprite && c->GetSprite) {
				if (siColisionan(this->GetSprite(), c->GetSprite())) {
					c->MoverAlRevez();
				}
			}
		}};
	}
	
};

struct Libros : public Dibujo {
	sf::Texture Texture;

	Libros(lista_componentes componentes_, sf::RenderWindow* w_, int x, int y):Dibujo(componentes_, w_) {
		Collision::CreateTextureAndBitmask(Texture, "Book.png");
		sprite.setTexture(Texture);
		sprite.setPosition(x, y);

		QuienSoy = []() {return 3; };
	}
};

struct Fondo : public Animacion {

	Fondo(int rapidez_animacion, std::vector<std::vector<std::string>> archivos, lista_componentes componentes, sf::RenderWindow* w) :
		Animacion{ rapidez_animacion, archivos, componentes, w }
	{
		sprite.setPosition(0, 0);

		Dibujar = [this, w]() {
			w->draw(sprite);
		};
	};


};
