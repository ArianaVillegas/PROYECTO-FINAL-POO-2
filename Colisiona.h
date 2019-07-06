#pragma once
#include "Collision.h"

template<typename Grafico>
bool siColisionan(Grafico a, Grafico b) {
	return Collision::PixelPerfectTest(a, b);
}