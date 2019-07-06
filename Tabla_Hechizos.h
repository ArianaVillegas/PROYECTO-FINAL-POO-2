#pragma once
#include <vector>
//Valores de los hechizos
struct H {
	float w;
	float h;
	short rapidez;
	short rapidez_animacion;
	std::vector<std::vector<std::string>> sprites;
	short durabilidad;
	bool ataque;
	short poder;
	short paralisis;
	H(float w, float h, short rapidez, short rapidez_animacion, std::vector<std::vector<std::string>> sprites, short durabilidad, bool ataque, short poder, short paralisis):
		w{ w }, h{ h }, rapidez{ rapidez }, rapidez_animacion{ rapidez_animacion }, sprites{ sprites }, durabilidad{ durabilidad }, ataque{ ataque }, poder{ poder }, paralisis{paralisis}{}
};

H bola_de_fuego(366, 355, 15, 1, { {"Hechizos/0.png", "Hechizos/1.png", "Hechizos/2.png", "Hechizos/1.png"} }, 60, true, -10, 0); //Hechizo 0
H bola_de_hielo(366, 355, 15, 1, { {"Hechizos/3.png", "Hechizos/6.png", "Hechizos/4.png", "Hechizos/3.png", "Hechizos/6.png", "Hechizos/5.png"} }, 60, true, -15, 40); //Hechizo 1
H estrella(307, 320, 8, 0.5, { {"Hechizos/7.png", "Hechizos/8.png", "Hechizos/9.png", "Hechizos/8.png"} }, 45, true, -10, 40); //Hechizo 2
H bola_de_fuego_verde(366, 355, 20, 1, { {"Hechizos/14.png", "Hechizos/15.png", "Hechizos/16.png", "Hechizos/15.png"} }, 50, true, -20, 60);
H laser(389, 350, 30, 1, { {"Hechizos/17.png"} }, 60, true, -30, 50);
H sanacion1(341, 340, 0, 1.2, { {"Hechizos/10.png", "Hechizos/11.png", "Hechizos/12.png", "Hechizos/13.png", "Hechizos/12.png","Hechizos/11.png"} }, 15, false, 20, 5);
H icespell(366, 355, 20, 1, {{"Hechizos/23.png", "Hechizos/24.png"} }, 100, true, -10, 50);
H spin(366, 355, 5, 1, { {"Hechizos/18.png", "Hechizos/19.png", "Hechizos/21.png", "Hechizos/22.png"} }, 300, true, -50, 0);
/*H enredaderas(366, 355, 12, 1, { {"Hechizos/0.png", "Hechizos/1.png", "Hechizos/2.png", "Hechizos/1.png"} }, 65, true, -14, 45);
H espadas_encantadas(366, 355, 14, 1, { {"Hechizos/0.png", "Hechizos/1.png", "Hechizos/2.png", "Hechizos/1.png"} }, 55, true, -16, 75);
H sanacion2(366, 355, 14, 1, { {"Hechizos/0.png", "Hechizos/1.png", "Hechizos/2.png", "Hechizos/1.png"} }, 60, false, 6, 60);
H sanacion3(366, 355, 14, 1, { {"Hechizos/0.png", "Hechizos/1.png", "Hechizos/2.png", "Hechizos/1.png"} }, 60, false, 8, 80);
H sanacion4(366, 355, 14, 1, { {"Hechizos/0.png", "Hechizos/1.png", "Hechizos/2.png", "Hechizos/1.png"} }, 60, false, 12, 90);
H sanacion5(366, 355, 14, 1, { {"Hechizos/0.png", "Hechizos/1.png", "Hechizos/2.png", "Hechizos/1.png"} }, 60, false, 16, 70);*/
//etc

std::vector<H> tabla_hechizos = {bola_de_fuego,bola_de_hielo,estrella,bola_de_fuego_verde,sanacion1,laser,icespell,spin								};