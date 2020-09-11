#include "Board.h"

Board::Board() {
	rows = 8;
	FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	white = 1;
}

Board::Board(const string& FEN) {
	rows = 8;
	this -> FEN = FEN;
	white = isWhiteTurn();
}

bool isWhiteTurn() {
	int length = FEN.length();
	int field = 1;
	bool found = false;
	for (int i = 0; i < length and !found; i++) {
		if (FEN[i] = ' ') {
			return FEN[i + 1] == 'w';
		}
	}
	return false;
}

void Board::display() {
	//ME LA SUDA AHORA
}

void Board::doMove(const string& move) {
	//pre: move is a string that represents a legal move in this position
	//pos: the move is executed and the FEN is modified

	if (white) {
		
	} else {
		
	}
}

vector<Move> Board::calculateLegalMoves() {

}