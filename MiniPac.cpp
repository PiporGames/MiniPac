#include <iostream>
#include <string>
#include <fstream>
#include "Tablero.h"

using namespace std;

int main() {
    cout << R"(
 _____                                                                                                                              _____ 
( ___ )                                                                                                                            ( ___ )
 |   |~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|   | 
 |   |                                                                                                                              |   | 
 |   |        ___                       ___                       ___           ___           ___                  @@@@@@@@         |   | 
 |   |       /\__\          ___        /\__\          ___        /\  \         /\  \         /\  \             @@@@@@@@@@@@@@@      |   | 
 |   |      /::|  |        /\  \      /::|  |        /\  \      /::\  \       /::\  \       /::\  \          @@@@@@ @@@@@@@@@@@@    |   | 
 |   |     /:|:|  |        \:\  \    /:|:|  |        \:\  \    /:/\:\  \     /:/\:\  \     /:/\:\  \       @@@@@@@   @@@@@@@@@      |   | 
 |   |    /:/|:|__|__      /::\__\  /:/|:|  |__      /::\__\  /::\~\:\  \   /::\~\:\  \   /:/  \:\  \     @@@@@@@@@ @@@@@@@         |   | 
 |   |   /:/ |::::\__\  __/:/\/__/ /:/ |:| /\__\  __/:/\/__/ /:/\:\ \:\__\ /:/\:\ \:\__\ /:/__/ \:\__\    @@@@@@@@@@@@@             |   | 
 |   |   \/__/~~/:/  / /\/:/  /    \/__|:|/:/  / /\/:/  /    \/__\:\/:/  / \/__\:\/:/  / \:\  \  \/__/    @@@@@@@@@@@@@@@@@         |   | 
 |   |         /:/  /  \::/__/         |:/:/  /  \::/__/          \::/  /       \::/  /   \:\  \           @@@@@@@@@@@@@@@@@@@      |   | 
 |   |        /:/  /    \:\__\         |::/  /    \:\__\           \/__/        /:/  /     \:\  \            @@@@@@@@@@@@@@@@@@@    |   | 
 |   |       /:/  /      \/__/         /:/  /      \/__/                       /:/  /       \:\__\             @@@@@@@@@@@@@@@      |   | 
 |   |       \/__/                     \/__/                                   \/__/         \/__/                 @@@@@@@@         |   | 
 |   |                                                                                                                              |   | 
 |___|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|___| 
(_____)                                                                                                                            (_____))" << endl << "   v1.0   |   Por José Manuel de Torres Domínguez, Manuel Alonso González y David de Miguel Palomino" << endl << endl << endl;
    cout << "Elige un archivo MINIPAC eligiendo un número del 1-10, o escribe el nombre del archivo (sin incluir la extensión, solo acepta .txt)" << endl << "Escriba 'exit' para salir del programa.";

    while (true) {
        string res;
        ifstream ifile;
        cout << endl << ">> ";
        cin >> res;
        if ((2 >= (int) res.length()) && (stoi(res) <= 10) && (stoi(res) >= 1)){
            res = "MINIPAC" + res + ".txt";
            ifile.open(res);
        }
        else {
            if (res != "exit") {
                res = res + ".txt";
                ifile.open(res);
            }
            else {
                return 0;
            }
        }
        if (ifile) {
            ifile.close();
            cout << "Cargando " << res << "..." << endl << endl;
            Tablero tab(res);
            tab.mostrar();
            tab.simular();     
        }
        else {
            cout << "El archivo " + res + " no existe o no se encuentra." << endl << endl;
        }
    }
    return 0;
} 
