#include "Piece.h"

Piece::Piece() {
	empty = 0;
	white = 0;
	type = Empty;
	idn = '0';
	rank = -1;
	file = -1;
}

Piece::Piece(bool e, bool w, char i, int r, int f) : 
empty(e), white(w), idn(i), rank(r), file(f + 'a') { 

	char t = i;
	if (t != '0' and t < 'a') t += ('a' - 'A');

	type = static_cast<PieceType>(t);
}

bool Piece::isEmpty() {
	return empty;
}

bool Piece::isWhite() {
	return white;
}

void Piece::copyPiece(Piece& p) {
	empty = p.empty;
	white = p.white;
	type = p.type;
	idn = p.idn;
}

void Piece::setType(PieceType t) {
	type = t;

	switch (type) {
		case Pawn:
			white ? idn = 'P' : idn = 'p';
		break;
		case Knight:
			white ? idn = 'N' : idn = 'n';
		break;
		case Bishop:
			white ? idn = 'B' : idn = 'b';
		break;
		case Rook:
			white ? idn = 'R' : idn = 'r';
		break;
		case Queen:
			white ? idn = 'Q' : idn = 'q';
		break;
		case King:
			white ? idn = 'K' : idn = 'k';
		break;
		case Empty:
			idn = '0';
		break;
	}
}

void Piece::printType() {
	switch (type) {
		case Pawn:
			cout << "Pawn";
		break;
		case Knight:
			cout << "Knight";
		break;
		case Bishop:
			cout << "Bishop";
		break;
		case Rook:
			cout << "Rook";
		break;
		case Queen:
			cout << "Queen";
		break;
		case King:
			cout << "King";
		break;
		case Empty:
			cout << "Empty";
		break;
		default:
			cout << "INTEO";
		break;
	}
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
