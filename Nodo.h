#include <iostream>
#include "Personaje.h"

using namespace std;

class Nodo {
	private:
		float h; // Función heurística
		int g; // Coste
		Nodo* nodPadre; // Nodo padre
		
		// Posición PacMan
		int pacX;
		int pacY;

		// Posición y orientación Fantasma
		int fanX;
		int fanY;
		char ori;
		//    a -> Arriba
		//    b -> aBajo
		//    d -> Derecha
		//    i -> Izquierda

	public:
		// Constructor
		Nodo();
		Nodo(float _h, int _g, Nodo* _nodPadre, int _pacX, int _pacY, int _fanX, int _fanY, char _ori);

		// Getters
		float getH();
		int getG();











		Nodo* getPadre();

		int getPacX();
		int getPacY();

		int getFanX();
		int getFanY();
		char getOri();

		// Setters
		void setH(float _h);
		void setG(int _g);
		void setPadre(Nodo* _nodPadre);

		void setPosPac(int _pacX, int _pacY);

		void setPosFan(int _fanX, int _fanY, char _ori);

		// Operadores sobrecargados
		bool operator ==(Nodo n) const;
		bool operator !=(Nodo n) const;
		bool operator >=(Nodo n) const;
		bool operator <=(Nodo n) const;
		bool operator >(Nodo n) const;
		bool operator <(Nodo n) const;



		// Destructor
		~Nodo();
	};