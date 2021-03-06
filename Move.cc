#include "Move.h"

Move::Move() {
	
}

Move::Move(Piece o, Piece d, bool cast, bool capt, bool pass, bool apass, bool prom) {		
	origin = o;
	destination = d;
	castling = cast;
	capture = capt;
	passant = pass;
	allowspassant = apass;
	promotion = prom;
	processNotation();
}

Piece Move::getOrigin() {
	return origin;
}

Piece Move::getDestination() {
	return destination;
}

bool Move::isCapture() {
	return capture;
}

bool Move::isPassant() {
	return passant;
}

bool Move::isCastling() {
	return castling;
}

bool Move::allowsPassant() {
	return allowspassant;
}

bool Move::isPromotion() {
	return promotion;
}

void Move::setNotation(const string& n) {
	this->notation = n;
}

string Move::getNotation() {
	return notation;
}

void Move::processNotation() {
	notation = "";
	if (castling) {
		int newY = destination.getFileAsInt();
		if (newY > origin.getFileAsInt()) {
			notation = "O-O";
		} else {
			notation = "O-O-O";
		}
	} else {
		bool pawn = 0;
		switch (origin.getType()) {
			case Pawn:
				pawn = 1;
			break;
			case Knight:
				notation.push_back('N');
			break;
			case Bishop:
				notation.push_back('B');
			break;
			case Rook:
				notation.push_back('R');
			break;
			case Queen:
				notation.push_back('Q');
			break;
			case King:
				notation.push_back('K');
			break;
			case Empty:
			break;
		}

		if (capture) {
			if (pawn) notation.push_back(origin.getFile());
			notation.push_back('x');
		}
		notation.push_back(destination.getFile());
		notation.push_back((8 - destination.getRank()) + '0');
	}
}