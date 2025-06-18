#include "Personaje.h"

//Constructor
Personaje::Personaje() {
    posX = 0;
    posY = 0;
}

Personaje::Personaje(int _posX, int _posY ) {
    posX = _posX;
    posY = _posY;
}

//Setters
void Personaje::setPosicion(int x, int y) {
    posX = x;
    posY = y;
}

//Getters
int Personaje::getPosX() { return posX; }
int Personaje::getPosY() { return posY; }

//Extras
//dir = "a" = arriba
//dir = "b" = abajo
//dir = "d" = derecha
//dir = "i" = izquierda
void Personaje::mover(char dir) {
    switch(dir) {
        case 'a':
            setPosicion(posX - 1, posY);
            break;
        case 'b':
            setPosicion(posX + 1, posY);
            break;
        case 'd':
            setPosicion(posX, posY + 1);
            break;
        case 'i':
            setPosicion(posX, posY - 1);
            break;
        default:
            cerr << "Movimiento no permitido" << endl;
            break;
    }
}

//Destructor
Personaje::~Personaje() {}