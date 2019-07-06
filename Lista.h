#pragma once
#include <functional>
#include <list>
#include <algorithm>
#include <iterator>


using namespace std;

template<typename T>
struct Lista {
	list<T> contenedor;

	void agregar(T valor) {
		contenedor.push_back(valor);
	}

	void para_cada(std::function<void(T)> f) {
		for_each(begin(contenedor), end(contenedor), f);
	}

	void comparar_todos(std::function<void(T, T)> f) {
		/*for (short i = 0; i < contenedor.size(); i++) {
			for (short j = i+1; j < contenedor.size(); j++) {
				f(contenedor[i], contenedor[j]);
			}
		}*/

		auto it = begin(contenedor)++;
		for_each(begin(contenedor), end(contenedor), [this, &it, f](T actual) {
			for_each(it, end(contenedor), [this, f, actual](T siguiente) {f(actual, siguiente); });
			it++;
		});
	}

	
	void eliminar()
	{
		contenedor.erase(remove_if(contenedor.begin(), contenedor.end(), [](T& valor) {return valor->eliminar; }), contenedor.end());
	}

	int tamaño() {
		return contenedor.size();
	}

	T getFirst() {
		return contenedor.front();
	}
};