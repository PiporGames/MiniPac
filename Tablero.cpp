#include "Tablero.h"
#include <set>

using namespace std::chrono;

Tablero::Tablero(string arch) { //Se lee desde el fichero
    //Inicialización de variables
    ifstream ifs(arch);
    salX = -1;
    salY = -1;
    oriFantasma = 'd';
    int pacX = 0, pacY = 0, fasX = 0, fasY = 0;

    //Lectura secuencial de archivo
    if (ifs.is_open()) {
        string aux;
        int i = 0, j = 0;
        while(!ifs.eof()) { //Hasta que lleguemos al final del archivo...
            getline(ifs, aux, ',');
            if (aux == "M" || aux == "\nM") mapa[i][j] = false; //Si es muro, entones esa coordenada del mapa es falsa (no es transitable)
            else if (i < MAX) { //Comprobación adicional: No nos podemos salir del máximo del mapa.
                mapa[i][j] = true; //Si no es muro, verdadero (es transitable).
                if (aux == "P") { // Si es PACMAN...
                    pacX = i;
                    pacY = j;
                } else if (aux == "F") { // Si es FANTASMA...
                    fasX = i;
                    fasY = j;
                } else if (aux == "S"|| aux == "\nS") { // Si es SALIDA...
                    salX = i;
                    salY = j;
                }
            }
            if (j < MAX - 1) j++;
            else {i++; j = 0;}
        }
        
        ifs.close();

    } else {
        cerr << "No se pudo abrir el archivo";
        exit(-1);
    }

    //Establecemos los valores a los miembros
    pacman = Personaje(pacX, pacY);
    fantasma = Personaje(fasX, fasY);
}

char Tablero::moverFantasma(int x, int y, char ori)
{
    int newX = x, newY = y;
    char newOri = 0;
    switch (ori) {
    case 'a':
        newX--;
        newOri = 'd';
        break;
    case 'b':
        newX++;
        newOri = 'i';
        break;
    case 'd':
        newY++;
        newOri = 'b';
        break;
    case 'i':
        newY--;
        newOri = 'a';
        break;
    }

    if (mapa[newX][newY] && (salX != newX && salY != newY)) {
        fantasma.setPosicion(newX, newY);
        return (ori);
    }
    else {
        ori = newOri;
        return (moverFantasma(x,y,ori));
    }
}

bool Tablero::algEscalada(list<char>& movPac, list<char>& movFan, int& nodGen) {
    bool res = false;
    bool finArbol = false;
    int pacX = pacman.getPosX();
    int pacY = pacman.getPosY();

    char orientacion = oriFantasma;

    float distSal;
    float umbral = 0;

    while ((pacX != salX || pacY != salY) && !finArbol && (pacX != fantasma.getPosX() || pacY != fantasma.getPosY())) {
        distSal = funcH(pacX, pacY);
        //Izquierda
        if ((pacY > 0 && mapa[pacX][pacY - 1]) && (funcH(pacX, pacY - 1) < (distSal + umbral))) {
            pacman.mover('i');
            nodGen++;
            movPac.push_back('I');
        }
        //Abajo
        else if ((pacX < MAX - 1 && mapa[pacX + 1][pacY]) && (funcH(pacX + 1, pacY) < (distSal + umbral))) {
            pacman.mover('b');
            nodGen++;
            movPac.push_back('B');
        }
        //Derecha
        else if ((pacY < MAX - 1 && mapa[pacX][pacY + 1]) && (funcH(pacX, pacY + 1) < (distSal + umbral))) {
            pacman.mover('d');
            nodGen++;
            movPac.push_back('D');
        }
        //Arriba
        else if ((pacX > 0 && mapa[pacX - 1][pacY]) && (funcH(pacX - 1, pacY) < (distSal + umbral))) {
            pacman.mover('a');
            nodGen++;
            movPac.push_back('A');
        }
        //Utilizamos el umbral para relajar al algoritmo
        else if (umbral != 0.0) {
            umbral = 0.0;
        }
        //No encontró la salida
        else {
            finArbol = true;
        }

        if (!finArbol) {
            orientacion = moverFantasma(fantasma.getPosX(), fantasma.getPosY(), orientacion);
            movFan.push_back(toupper(orientacion));
        }

        pacX = pacman.getPosX();
        pacY = pacman.getPosY();
    }

    if (pacX == salX && pacY == salY)
        res = true;
     
    return res;
}

//Secuencia de búsqueda -> i, b, d, a
bool Tablero::algPriMejor(list<char>& movPac, list<char>& movFan, int& nodGen) {
    int pacX = pacman.getPosX();
    int pacY = pacman.getPosY();
    
    bool res = false;

    //Crear lista de nodos abiertos y cerrados (los nodos una clase nueva con operadores sobreescritos)

    list<Nodo *> listNodoAbierto;
    list<Nodo *> listNodoCerrado;

    bool sal = false; // -sa y picante 🗣🗣🗣🔥🔥🔥

    // Nodo raiz
    Nodo * nodAbr = new Nodo(funcH(pacX, pacY), 0, nullptr, pacX, pacY, fantasma.getPosX(), fantasma.getPosY(), oriFantasma);
    Nodo * nodAux;
    listNodoAbierto.push_front(nodAbr);

    while(!sal) { 
        //Seleccionar nodo de la lista con menos coste.
        //Como en la primera ejecución solo habrá un elemento, pondremos el .sort() más abajo.
        if(listNodoAbierto.size() > 0) { 

            nodAbr = listNodoAbierto.front(); //coger nodo
            listNodoAbierto.pop_front(); // quitarlo de la lista
            listNodoCerrado.push_front(nodAbr); // ponerlo en la lista de cerradosS

            //Obtener posición (local) del pacman
            pacX = nodAbr->getPacX();
            pacY = nodAbr->getPacY();

            if (pacX == salX && pacY == salY) {
                sal = true; // Si estamos en la salida
                res = true;
            }
            else if (pacX != fantasma.getPosX() || pacY != fantasma.getPosY()) { // Si NO colisionamos con el fantasma en esta imagen
                //Mover fantasma y obtener su orientación
                char orientacion = moverFantasma(nodAbr->getFanX(), nodAbr->getFanY(), nodAbr->getOri());

                // Mover Pacman
                //Izquierda
                if (pacY > 0 && mapa[pacX][pacY - 1]) {
                    //Añadir a lista nodos abiertos izquierda
                    nodAux = new Nodo(funcH(pacX, pacY - 1), 0, nodAbr, pacX, pacY - 1, fantasma.getPosX(), fantasma.getPosY(), orientacion);
                    listNodoAbierto.push_front(nodAux);
                }
                //Abajo
                if (pacX < MAX - 1 && mapa[pacX + 1][pacY]) {
                    //Añadir a lista nodos abiertos abajo
                    nodAux = new Nodo(funcH(pacX + 1, pacY), 0, nodAbr, pacX + 1, pacY, fantasma.getPosX(), fantasma.getPosY(), orientacion);
                    listNodoAbierto.push_front(nodAux);
                }
                //Derecha
                if (pacY < MAX - 1 && mapa[pacX][pacY + 1]) {
                    //Añadir a lista nodos abiertos derecha
                    nodAux = new Nodo(funcH(pacX, pacY + 1), 0, nodAbr, pacX, pacY + 1, fantasma.getPosX(), fantasma.getPosY(), orientacion);
                    listNodoAbierto.push_front(nodAux);
                }
                //Arriba
                if (pacX > 0 && mapa[pacX - 1][pacY]) {
                    //Añadir a lista nodos abiertos arriba
                    nodAux = new Nodo(funcH(pacX - 1, pacY), 0, nodAbr, pacX - 1, pacY, fantasma.getPosX(), fantasma.getPosY(), orientacion);
                    listNodoAbierto.push_front(nodAux);
                }

                //Ordenar lista
                listNodoAbierto.sort([](Nodo* n1, Nodo* n2) { return *n1 < *n2; });

                //Eliminar items repetidos
                std::set<Nodo> objetosUnicos; // set de nodos abiertos únicos ?
                std::set<Nodo> nodosCerrados; // set de nodos cerrados
                std::list<Nodo*>::iterator it;
                std::list<Nodo*>::iterator it_aux;
                // Meter nodos cerrados en el set suyo
                for (it = listNodoCerrado.begin(); it != listNodoCerrado.end(); it++) {
                    nodosCerrados.insert(**it);
                }

                it = listNodoAbierto.begin();
                while (it != listNodoAbierto.end()) {
                    Nodo* aux = *it;

                    if (std::find(objetosUnicos.begin(), objetosUnicos.end(), *aux) == objetosUnicos.end()
                        && std::find(nodosCerrados.begin(), nodosCerrados.end(), *aux) == nodosCerrados.end()) {
                        //Objeto único
                        objetosUnicos.insert(*aux);
                        it++;
                    }
                    else {
                        //Objeto duplicado                   
                        it = listNodoAbierto.erase(it);
                        delete aux;  // 💀
                        aux = nullptr;
                        if (it == listNodoAbierto.end()) {
                            break;
                        }
                    }
                }
            }
        } else{
            sal = true;
        }

        
    }


    //Número de nodos generados y procesados
    nodGen = (int) listNodoCerrado.size() + (int) listNodoAbierto.size();

    //Generar secuencia pacman y fantasma
    //  Basicamente lo que hace este algoritmo es coger el último nodo generado, que debería coincidir con el de la salida,
    //  y ve escalando hacia arriba a través de los padres de cada nodo hasta llegar a la raiz, que es nulltpr.
    //  Con cada nodo se hace una comparación con el anterior para descubrir que movimiento se había realizado.
    //  Luego, se hace lo mismo con el fantasma en el mismo cíclo del pacman.
    Nodo * nodHijo = nodAbr;
    Nodo * nodPadre = nodAbr->getPadre();
    while (nodPadre != nullptr) {

        // PAC-MAN ///
        int hx = nodHijo->getPacX();
        int px = nodPadre->getPacX();
        int hy = nodHijo->getPacY();
        int py = nodPadre->getPacY();

        if ((py - hy == 1)) { //izquierda
            movPac.push_front('I');
        }
        else if ((py - hy == -1)) { //derecha
            movPac.push_front('D');
        }
        else if ((px - hx == -1)) { //abajo
            movPac.push_front('B');
        }
        else if ((px - hx == 1)) { //arriba
            movPac.push_front('A');
        }
        //cout << "pacman: " << hx << ", " << hy << endl;

        /// FANTASMA ///
        hx = nodHijo->getFanX();
        px = nodPadre->getFanX();
        hy = nodHijo->getFanY();
        py = nodPadre->getFanY();

        if ((py - hy == 1)) { //izquierda
            movFan.push_front('I');
        }
        else if ((py - hy == -1)) { //derecha
            movFan.push_front('D');
        }
        else if ((px - hx == -1)) { //abajo
            movFan.push_front('B');
        }
        else if ((px - hx == 1)) { //arriba
            movFan.push_front('A');
        }
        //cout << "fan: " << hx << ", " << hy << endl;
        
        // Cambiar nodo
        nodHijo = nodPadre;
        nodPadre = nodHijo->getPadre();
    }

    //Liberamos memoria (abiertos)
    std::list<Nodo*>::iterator it;
    for (it = listNodoAbierto.begin(); it != listNodoAbierto.end(); it++) {  
        delete *it;  // 💀
        *it = nullptr;
        it = listNodoAbierto.erase(it);
        if (it == listNodoAbierto.end()) {
            break;
        }
    }
    //Liberamos memoria (cerrados)
    for (it = listNodoCerrado.begin(); it != listNodoCerrado.end(); it++) {
        delete* it;  // 💀
        *it = nullptr;
        it = listNodoCerrado.erase(it);
        if (it == listNodoCerrado.end()) {
            break;
        }
    }

    return res;
}

bool Tablero::algAestr(list<char>& movPac, list<char>& movFan, int& nodGen) {
    int pacX = pacman.getPosX();
    int pacY = pacman.getPosY();

    bool res = false;

    //Crear lista de nodos abiertos y cerrados (los nodos una clase nueva con operadores sobreescritos)

    list<Nodo*> listNodoAbierto;
    list<Nodo*> listNodoCerrado;

    bool sal = false; // -sa y picante 🗣🗣🗣🔥🔥🔥

    // Nodo raiz
    Nodo* nodAbr = new Nodo(funcH(pacX, pacY), 0, nullptr, pacX, pacY, fantasma.getPosX(), fantasma.getPosY(), oriFantasma);
    Nodo* nodAux;
    listNodoAbierto.push_front(nodAbr);

    while (!sal) {
        if (listNodoAbierto.size() > 0) {
            //Seleccionar nodo de la lista con menos coste.
            //Como en la primera ejecución solo habrá un elemento, pondremos el .sort() más abajo.
            nodAbr = listNodoAbierto.front(); //coger nodo
            listNodoAbierto.pop_front(); // quitarlo de la lista
            listNodoCerrado.push_front(nodAbr); // ponerlo en la lista de cerrados

            //Obtener posición (local) del pacman
            pacX = nodAbr->getPacX();
            pacY = nodAbr->getPacY();

            if (pacX == salX && pacY == salY) { 
                sal = true; // Si estamos en la salida 
                res = true;
            }
            else if (pacX != fantasma.getPosX() || pacY != fantasma.getPosY()) { // Si NO colisionamos con el fantasma en esta imagen
                //Mover fantasma y obtener su orientación
                char orientacion = moverFantasma(nodAbr->getFanX(), nodAbr->getFanY(), nodAbr->getOri());

                // Mover Pacman
                //Izquierda
                if (pacY > 0 && mapa[pacX][pacY - 1]) {
                    //Añadir a lista nodos abiertos izquierda
                    nodAux = new Nodo(funcH(pacX, pacY - 1), nodAbr->getG() + 1, nodAbr, pacX, pacY - 1, fantasma.getPosX(), fantasma.getPosY(), orientacion);
                    listNodoAbierto.push_front(nodAux);
                }
                //Abajo
                if (pacX < MAX - 1 && mapa[pacX + 1][pacY]) {
                    //Añadir a lista nodos abiertos abajo
                    nodAux = new Nodo(funcH(pacX + 1, pacY), nodAbr->getG() + 1, nodAbr, pacX + 1, pacY, fantasma.getPosX(), fantasma.getPosY(), orientacion);
                    listNodoAbierto.push_front(nodAux);
                }
                //Derecha
                if (pacY < MAX - 1 && mapa[pacX][pacY + 1]) {
                    //Añadir a lista nodos abiertos derecha
                    nodAux = new Nodo(funcH(pacX, pacY + 1), nodAbr->getG() + 1, nodAbr, pacX, pacY + 1, fantasma.getPosX(), fantasma.getPosY(), orientacion);
                    listNodoAbierto.push_front(nodAux);
                }
                //Arriba
                if (pacX > 0 && mapa[pacX - 1][pacY]) {
                    //Añadir a lista nodos abiertos arriba
                    nodAux = new Nodo(funcH(pacX - 1, pacY), nodAbr->getG() + 1, nodAbr, pacX - 1, pacY, fantasma.getPosX(), fantasma.getPosY(), orientacion);
                    listNodoAbierto.push_front(nodAux);
                }

                //Ordenar lista
                listNodoAbierto.sort([](Nodo* n1, Nodo* n2) { return *n1 < *n2; });

                //Eliminar items repetidos
                std::set<Nodo> objetosUnicos; // set de nodos abiertos únicos ?
                std::set<Nodo> nodosCerrados; // set de nodos cerrados
                std::list<Nodo*>::iterator it;
                std::list<Nodo*>::iterator it_aux;
                // Meter nodos cerrados en el set suyo
                for (it = listNodoCerrado.begin(); it != listNodoCerrado.end(); it++) {
                    nodosCerrados.insert(**it);
                }

                it = listNodoAbierto.begin();
                while (it != listNodoAbierto.end()) {
                    Nodo* aux = *it;

                    if (std::find(objetosUnicos.begin(), objetosUnicos.end(), *aux) == objetosUnicos.end()
                        && std::find(nodosCerrados.begin(), nodosCerrados.end(), *aux) == nodosCerrados.end()) {
                        //Objeto único
                        objetosUnicos.insert(*aux);
                        it++;
                    }
                    else {
                        //Objeto duplicado                   
                        it = listNodoAbierto.erase(it);
                        delete aux;  // 💀
                        aux = nullptr;
                        if (it == listNodoAbierto.end()) {
                            break;
                        }
                    }
                }
            }
        } else {
            sal = true;
        }
    }


    //Número de nodos generados y procesados
    nodGen = (int)listNodoCerrado.size() + (int)listNodoAbierto.size();

    //Generar secuencia pacman y fantasma
    //  Basicamente lo que hace este algoritmo es coger el último nodo generado, que debería coincidir con el de la salida,
    //  y ve escalando hacia arriba a través de los padres de cada nodo hasta llegar a la raiz, que es nulltpr.
    //  Con cada nodo se hace una comparación con el anterior para descubrir que movimiento se había realizado.
    //  Luego, se hace lo mismo con el fantasma en el mismo cíclo del pacman.
    Nodo* nodHijo = nodAbr;
    Nodo* nodPadre = nodAbr->getPadre();
    while (nodPadre != nullptr) {

        // PAC-MAN ///
        int hx = nodHijo->getPacX();
        int px = nodPadre->getPacX();
        int hy = nodHijo->getPacY();
        int py = nodPadre->getPacY();

        if ((py - hy == 1)) { //izquierda
            movPac.push_front('I');
        }
        else if ((py - hy == -1)) { //derecha
            movPac.push_front('D');
        }
        else if ((px - hx == -1)) { //abajo
            movPac.push_front('B');
        }
        else if ((px - hx == 1)) { //arriba
            movPac.push_front('A');
        }
        //cout << "pacman: " << hx << ", " << hy << endl;

        /// FANTASMA ///
        hx = nodHijo->getFanX();
        px = nodPadre->getFanX();
        hy = nodHijo->getFanY();
        py = nodPadre->getFanY();

        if ((py - hy == 1)) { //izquierda
            movFan.push_front('I');
        }
        else if ((py - hy == -1)) { //derecha
            movFan.push_front('D');
        }
        else if ((px - hx == -1)) { //abajo
            movFan.push_front('B');
        }
        else if ((px - hx == 1)) { //arriba
            movFan.push_front('A');
        }
        //cout << "fan: " << hx << ", " << hy << endl;

        // Cambiar nodo
        nodHijo = nodPadre;
        nodPadre = nodHijo->getPadre();
    }

    //Liberamos memoria (abiertos)
    std::list<Nodo*>::iterator it;
    for (it = listNodoAbierto.begin(); it != listNodoAbierto.end(); it++) {
        delete* it;  // 💀
        *it = nullptr;
        it = listNodoAbierto.erase(it);
        if (it == listNodoAbierto.end()) {
            break;
        }
    }
    //Liberamos memoria (cerrados)
    for (it = listNodoCerrado.begin(); it != listNodoCerrado.end(); it++) {
        delete* it;  // 💀
        *it = nullptr;
        it = listNodoCerrado.erase(it);
        if (it == listNodoCerrado.end()) {
            break;
        }
    }

    return res;
}

float Tablero::funcH(int posX, int posY) {
    return (float) (sqrt( pow(abs(salX - posX), 2) + pow(abs(salY - posY),2)));
}

void Tablero::reportarInfo(bool res, list<char>& movPac, list<char>& movFan, int &nodGen, float tiempo) {
    // Resultado
    cout << "RESULTADO DE LA EJECUCIÓN: ";
    if (res) {
        cout << "FINALIZADO" << endl;
    }
    else {
        cout << "FRACASO" << endl;
    }

    // MovPac
    cout << "SECUENCIA DE MOVIMIENTOS (PACMAN): ";
    // Iterador para recorrer la lista
    list<char>::iterator mp = movPac.begin();
    // Avanzar al siguiente elemento si no hemos llegado al final de la lista
    while (mp != movPac.end()) {
        cout << *mp << " ";
        advance(mp, 1); // Avanzar un elemento
    }
    cout << endl;

    // MovPac
    cout << "SECUENCIA DE MOVIMIENTOS (FANTASMA): ";
    // Iterador para recorrer la lista
    list<char>::iterator mf = movFan.begin();
    // Avanzar al siguiente elemento si no hemos llegado al final de la lista
    while (mf != movFan.end()) {
        cout << *mf << " ";
        advance(mf, 1); // Avanzar un elemento
    }
    cout << endl;

    // NodGen
    cout << "NÚMERO DE NODOS GENERADOS: " << nodGen << endl;

    // NodGen
    cout << "TIEMPO DE EJECUCIÓN: " << tiempo << endl;
}

//void Tablero::mostrarNodo(Nodo * nod) {
//    for (int i = 0; i < MAX; i++) {
//        for (int j = 0; j < MAX; j++) {
//            if (i == nod->getPacX() && j == nod->getPacY()) { cout << "P"; }
//            else if (i == nod->getFanX() && j == nod->getFanY()) { cout << "F"; }
//            else if (mapa[i][j]) cout << " ";
//            else cout << "#";
//        }
//        cout << endl;
//    }
//}

void Tablero::simular() { // Proceso principal 
    //Variables
    bool res = false;
    list<char> movPac;
    list<char> movFan;
    int nodoGen = 0;

    Personaje fantasmaOriginal = fantasma;
    Personaje pacmanOriginal = pacman;
    cout << endl << "[[[ Ejecutando simulación con el Algorítmo de Escalada..." << endl;
    auto start = high_resolution_clock::now();
    res = algEscalada(movPac, movFan, nodoGen);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    reportarInfo(res, movPac, movFan, nodoGen, (float) duration.count());
    movPac.clear();
    movFan.clear();
    nodoGen = 0;
    cout << "Fin del reporte del Algoritmo de Escalada ]]]" << endl;

    fantasma = fantasmaOriginal;
    pacman = pacmanOriginal;
    oriFantasma = 'd';
    cout << endl << "[[[ Ejecutando simulación con el Algorítmo de Busqueda del Primero Mejor..." << endl;
    start = high_resolution_clock::now();
    res = algPriMejor(movPac, movFan, nodoGen);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    reportarInfo(res, movPac, movFan, nodoGen, (float) duration.count());
    movPac.clear();
    movFan.clear();
    nodoGen = 0;
    cout << "Fin del reporte del Algoritmo de Busqueda del Primero Mejor]]]" << endl;

    fantasma = fantasmaOriginal;
    pacman = pacmanOriginal;
    oriFantasma = 'd';
    cout << endl << "[[[ Ejecutando simulación con el Algorítmo de A*..." << endl;
    start = high_resolution_clock::now();
    res = algAestr(movPac, movFan, nodoGen);
    stop = high_resolution_clock::now();
    duration = duration_cast<microseconds>(stop - start);
    reportarInfo(res, movPac, movFan, nodoGen, (float) duration.count());
    movPac.clear();
    movFan.clear();
    nodoGen = 0;
    cout << "Fin del reporte del Algoritmo de A*]]]" << endl;
}

void Tablero::mostrar() {
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (i == pacman.getPosX() && j == pacman.getPosY()) { cout << "ᗤ "; }
            else if (i == fantasma.getPosX() && j == fantasma.getPosY()) { cout << "ᗣ "; }
            else if (i == salX && j == salY) { cout << "  "; }
            else if (!mapa[i][j]) cout << "▩ ";
            else cout << "  ";
        }
        cout << endl;
    }

    cout << "Posicion de PacMan: (" << pacman.getPosX() << ", " << pacman.getPosY() << ")" << endl;
    cout << "Posicion de Fantasma: (" << fantasma.getPosX() << ", " << fantasma.getPosY() << ")" << endl;
    cout << "Posicion de la Salida: (" << salX << ", " << salY << ")" << endl;
}

Tablero::~Tablero() { }
