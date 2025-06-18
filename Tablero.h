#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <chrono>
#include <cmath>
#include "Nodo.h"
#include "Personaje.h"
#include <algorithm>

using namespace std;

const int MAX = 10; // Máximo tamaño del laberinto.

class Tablero {

    private:
        bool mapa[MAX][MAX]; // Laberinto (true -> Celda vacía (transitable), false -> Muro)
        int salX; // Fila salida
        int salY; // Columna salida
        Personaje pacman; // Personaje principal
        Personaje fantasma; // Personaje adversario
        char oriFantasma; // Orientación del fantasma:
                          //    a -> Arriba
                          //    b -> aBajo
                          //    d -> Derecha
                          //    i -> Izquierda

        //PRE : int X, int Y (coordenadas del fantasma), char ori (orientación del fantasma, refierase a la anterior tabla)
        //DESC: Algoritmo interno que mueve al fantasma a la siguiente posición utilizando las coordenadas proporcionadas por parámetro (usado en el resto de algoritmos a excepción del de Escalada)
        char moverFantasma(int x, int y, char ori);

        // PRE: res -> Resultado del algoritmo
        //      list<char>& movPac -> Movimientos de PacMan
        //      list<char>& movFan -> Movimientos del Fantasma
        //      int& nodGen -> Número de nodos generados
        //      float tiempo -> Tiempo de ejecución del algoritmo (chrono)
        // DESC: Función interna utilizada para mostrar los resultados de un algoritmo por pantalla de manera simple y ordenada.
        void reportarInfo(bool res, list<char>& movPac, list<char>& movFan, int& nodGen, float tiempo);

        // PRE: Puntero a Nodo inicializado
        // DESC: Muestra por pantalla un mapa rellenado con la información del Nodo.
        void mostrarNodo(Nodo* nod);

        // PRE: int posX y posY posiciones en el mapa
        // DESC: Función interna que calcula la función heurística h'.
        // POST: float equivalente al valor h' asociado a dicho conjunto de valores pasados por parámetro.
        float funcH(int posX, int posY);

    public:
    
        // PRE: String arch -> Ruta donde se encuentra el fichero con el laberinto
        // DESC: Inicializa el Tablero y carga la memoria con los datos del archivo de texto pasado por parámetro en forma de ruta.
        // Constructor.
        Tablero(string arch); //Se lee desde el fichero

        //Escalada
        bool algEscalada(list<char>& movPac, list<char>& movFan, int& nodGen);

        //Primero mejor
        bool algPriMejor(list<char>& movPac, list<char>& movFan, int& nodGen);

        //A*
        bool algAestr(list<char>& movPac, list<char>& movFan, int& nodGen);

        //DESC: Simula el sistema con los tres algoritmos anteriores, mostrando la salida por pantalla.
        void simular();

        //DESC: Muestra por pantalla el laberinto, los personajes, y sus coordenadas, entre otros datos orientativos de depuración.
        // Depuración.
        void mostrar();

        //DESC: Destructor de Tablero por defecto.
        // Destructor.
        ~Tablero();
};
