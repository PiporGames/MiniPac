#include "Nodo.h"

// Constructor
Nodo::Nodo() {
	h = 0;
	g = 0;
	nodPadre = nullptr;
	pacX = 0;
	pacY = 0;
	fanX = 0;
	fanY = 0;
	ori = '\0';
}

Nodo::Nodo(float _h, int _g, Nodo* _nodPadre, int _pacX, int _pacY, int _fanX, int _fanY, char _ori) {
	h = _h;
	g = _g;
	nodPadre = _nodPadre;
	pacX = _pacX;
	pacY = _pacY;
	fanX = _fanX;
	fanY = _fanY;
	ori = _ori;
}

// Getters
float Nodo::getH() { return h; }
int Nodo::getG() { return g; }
Nodo* Nodo::getPadre() { return nodPadre; }

int Nodo::getPacX() { return pacX; }
int Nodo::getPacY() { return pacY; }

int Nodo::getFanX() { return fanX; }
int Nodo::getFanY() { return fanY; }
char Nodo::getOri() { return ori; }

// Setters
void Nodo::setH(float _h) { h = _h; }
void Nodo::setG(int _g) { g = _g; }
void Nodo::setPadre(Nodo* _nodPadre) { nodPadre = _nodPadre; }

void Nodo::setPosPac(int _pacX, int _pacY) {
	pacX = _pacX;
	pacY = _pacY;
}

void Nodo::setPosFan(int _fanX, int _fanY, char _ori) {
	fanX = _fanX;
	fanY = _fanY;
	ori = _ori;
}

// Operadores sobrecargados
bool Nodo::operator ==(Nodo n) const {
	return (h == n.getH()) && (pacX == n.getPacX() && pacY == n.getPacY()) && ((abs(fanX - n.getFanX()) + abs(fanY - n.getFanY()))<= 10);
}
bool Nodo::operator !=(Nodo n) const {
	return (h != n.getH()) || (pacX != n.getPacX() || pacY != n.getPacY()) || ((abs(fanX - n.getFanX()) + abs(fanY - n.getFanY())) > 10);
}
bool Nodo::operator >=(Nodo n) const {
	return (h + g) >= (n.getH() + n.getG());
}
bool Nodo::operator <=(Nodo n) const {
	return (h + g) <= (n.getH() + n.getG());
}
bool Nodo::operator >(Nodo n) const {
	return (h + g) > (n.getH() + n.getG());
}
bool Nodo::operator <(Nodo n) const {
	return (h + g) < (n.getH() + n.getG());
}



// Destructor
Nodo::~Nodo() {}