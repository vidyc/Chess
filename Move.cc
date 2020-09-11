#include "Move.h"

Move::Move(Piece o, Piece d, bool cast, bool capt, char prom) {		
	origin = o;
	destination = d;
	castling = cast;
	capture = capt;
	promotion = prom;
}

void Move::setNotation(const string& n) {
	this->notation = n;
}

string Move::getNotation() {
	return notation;
}

void Move::processNotation() {
	
}