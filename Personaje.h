#pragma once
#include <iostream>

using namespace std;

class Personaje {

    private:
        int posX; // Fila del personaje
        int posY; // Columna del personaje

    public:

        //DESC: Inicializa por defecto un objeto Personaje.
        // Constructor.
        Personaje();
        //PRE: int _posX -> Fila
        //     int _posY -> Columna
        //DESC: Inicializa un objeto Personaje dada unas coordenadas.
        // Constructor.
        Personaje(int _posX, int _posY);

        //Setters

        //PRE: int x -> Fila
        //     int y -> Columna
        //DESC: Establece la posición del personaje a la dada por parámetro.
        void setPosicion(int x, int y);

        //Getters

        //DESC: Devuelve la posición X del personaje.
        //POST: int equivalente a la fila del personaje.
        int getPosX();
        //DESC: Devuelve la posición Y del personaje.
        //POST: int equivalente a la columna del personaje.
        int getPosY();

        //Extras
        
        //PRE: char dir -> 
        //                  dir = "a" = arriba
        //                  dir = "b" = abajo
        //                  dir = "d" = derecha
        //                  dir = "i" = izquierda
        //DESC: Mueve el personaje en una dirección concreta automaticamente.
        void mover(char dir);

        //DESC: Destructor por defecto del objeto Personaje.
        // Destructor.
        ~Personaje();
};