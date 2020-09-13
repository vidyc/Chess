#include "Board.h"

Board::Board() {
	rows = 8;
	FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	setBoard();
	check = isKingInCheck();
}

Board::Board(const string& FEN) {
	rows = 8;
	this -> FEN = FEN;
	setBoard();
	check = isKingInCheck();
}

bool Board::isKingInCheck() {
	int x, y;
	if (white) {
		x = whiteKing -> getRank();
		y = whiteKing -> getFileAsInt();
	} else {
		x = blackKing -> getRank();
		y = blackKing -> getFileAsInt();
	}
	return isSquareAttacked(x, y);
}

bool Board::isSquareAttacked(int x, int y) {
	bool check = 0;
	bool up = 1, right = 1, left = 1, down = 1;
	bool upLeft = 1, upRight = 1, downLeft = 1, downRight = 1;

	for (int i = 1; i < 8 and !check and (up or left or right or down or upLeft 
		or upRight or downLeft or downRight); i++) {
			
		if (up and x - i >= 0) {
			Piece m = board[x - i][y];
			if (!m.isEmpty()) {
				if (m.isWhite() != white and (m.getType() == Queen or m.getType() == Rook)) {
					check = 1;
				}
				up = 0;
			}
		}

		if (!check and down and x + i < rows) {
			Piece m = board[x + i][y];
			if (!m.isEmpty()) {
				if (m.isWhite() != white and (m.getType() == Queen or m.getType() == Rook)) {
					check = 1;
				}
				down = 0;
			}
		}

		if (!check and left and y - i >= 0) {
			Piece m = board[x][y - i];
			if (!m.isEmpty()) {
				if (m.isWhite() != white and (m.getType() == Queen or m.getType() == Rook)) {
					check = 1;
				}
				left = 0;
			}
		}

		if (!check and right and y + i < rows) {
			Piece m = board[x][y + i];
			if (!m.isEmpty()) {
				if (m.isWhite() != white and (m.getType() == Queen or m.getType() == Rook)) {
					check = 1;
				}
				right = 0;
			}
		}

		if (!check and upLeft and x - i >= 0 and y - i >= 0) {
			Piece m = board[x - i][y - i];
			if (!m.isEmpty()) {
				if (m.isWhite() != white and (m.getType() == Queen or m.getType() == Bishop 
					or (m.getType() == Pawn and i == 1))) {
					check = 1;
				}
				upLeft = 0;
			}
		}

		if (!check and upRight and x - i >= 0 and y + i < rows) {
			Piece m = board[x - i][y + i];
			if (!m.isEmpty()) {
				if (m.isWhite() != white and (m.getType() == Queen or m.getType() == Bishop 
					or (m.getType() == Pawn and i == 1))) {
					check = 1;
				}
				upRight = 0;
			}
		}

		if (!check and downLeft and x + i < rows and y - i >= 0) {
			Piece m = board[x + i][y - i];
			if (!m.isEmpty()) {
				if (m.isWhite() != white and (m.getType() == Queen or m.getType() == Bishop)) {
					check = 1;
				}
				downLeft = 0;
			}
		}

		if (!check and downRight and x + i < rows and y + i < rows) {
			Piece m = board[x + i][y + i];
			if (!m.isEmpty()) {
				if (m.isWhite() != white and (m.getType() == Queen or m.getType() == Bishop)) {
					check = 1;
				}
				downRight = 0;
			}
		}
	} 

	if (!check) {
		int m1[2] = {-2, 2};
		int m2[2] = {-1, 1};

		for (int i = 0; !check and i < 2; i++) {
			for (int j = 0; !check and j < 2; j++) {
				if (x + m1[i] >= 0 and x + m1[i] < rows and y + m2[j] >= 0 
					and y + m2[j] < rows) {
					Piece m = board[x + m1[i]][y + m2[j]];
					if (!m.isEmpty() and m.isWhite() != white and m.getType() == Knight) check = 1;
				} 

				if (x + m2[i] >= 0 and x + m2[i] < rows and y + m1[j] >= 0 
					and y + m1[j] < rows) {
					Piece m = board[x + m2[i]][y + m1[j]];
					if (!m.isEmpty() and m.isWhite() != white and m.getType() == Knight) check = 1;
				} 
			}
		}
	}	
	return check;
}

string Board::getFEN() {
	return FEN;
}

void Board::setBoard() {
	int length = FEN.length();
	int field = 1;
	int file = 0, rank = 0;
	bool found = false;
	string cast = "";
	string pass = "";
	castlingRights = vector<bool> (4);
	board = vector<vector<Piece> > (rows, vector<Piece> (rows));
	for (int i = 0; i < length and !found; i++) {
		if (FEN[i] == ' ') {
			field++;
		} else if (field == 1) {
			if (FEN[i] == '/') {
				file = 0;
				rank++;
			} else if (FEN[i] >= '1' and FEN[i] <= '8') {
				int n = FEN[i] - '0';
				for (int i = 0; i < n; i++) {
					board[rank][file] = Piece(1, 0, '0', rank, file);
				}
				file += n;
			} else {
				board[rank][file] = Piece(0, FEN[i] < 'a', FEN[i], rank, file);
				if (FEN[i] >= 'A' and FEN[i] <= 'Z') whitePieces.push_back(&board[rank][file]);
				else if (FEN[i] >= 'a' and FEN[i] <= 'z') blackPieces.push_back(&board[rank][file]);

				if (FEN[i] == 'K') whiteKing = &board[rank][file];
				else if (FEN[i] == 'k') blackKing = &board[rank][file];
				file++;
			}
		} else if (field == 2) {
			white = (FEN[i] == 'w');
		} else if (field == 3) {
			cast.push_back(FEN[i]);
		} else if (field == 4) {
			pass.push_back(FEN[i]);
		} else if (field == 5) {
			halfmoves = FEN[i] - '0';
		} else if (field == 6) {
			turn = FEN[i] - '0';
		}
	}

	string test = "KQkq";

	for (int i = 0; i < 4; i++) {
		castlingRights[i] = (cast.find(test[i]) != string::npos);
	}

	passant = pass;
}

void Board::display() {
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < rows; j++) {
			cout << board[i][j].getIdn() << ' ';
		}
		cout << endl;
	}
	cout << endl;
}

void Board::doMove(const string& move) {
	//pre: move is a string that represents a legal move in this position
	//pos: the move is executed and the FEN is modified

	if (white) {
		
	} else {
		
	}
}

vector<Move> Board::calculateLegalMoves() {
	vector<Move> moves;
	return moves;
}