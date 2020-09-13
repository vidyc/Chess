#include "Piece.h"

Piece::Piece() {
	empty = 0;
	white = 0;
	type = static_cast<PieceType>('0');
	idn = '0';
	rank = -1;
	file = -1;
}

Piece::Piece(bool e, bool w, char i, int r, int f) : 
empty(e), white(w), idn(i), rank(r), file(f + 'a') { 

	char t = i;
	if (t < 'a') t += ('a' - 'A');

	type = static_cast<PieceType>(t);
}

bool Piece::isEmpty() {
	return empty;
}

bool Piece::isWhite() {
	return white;
}

PieceType Piece::getType() {
	return type;
}

char Piece::getIdn() {
	return idn;
}

int Piece::getRank() {
	return rank;
}

char Piece::getFile() {
	return file;
}

int Piece::getFileAsInt() {
	return file - 'a';
}
