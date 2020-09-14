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

	passant = Piece(1, !white, '0', pass[1] + '0' - 1, pass[0] - 'a');
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

void Board::calculateLegalMovesBishop(vector<Move> &legalMoves, int x, int y) {
	bool upLeft = 1, upRight = 1, downLeft = 1, downRight = 1;

	for (int i = 0; i < 8 and (upLeft or upRight or downLeft or downRight); i++) {
		if (upLeft and x - i >= 0 and y - i >= 0) {
			Piece m = board[x - i][y - i];
			if (m.isEmpty()) {
				legalMoves.push_back(Move(board[x][y], m, 0, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.push_back(Move(board[x][y], m, 0, 1, 0, '-'));
				upLeft = 0;
			} else upLeft = 0;
		}

		if (upRight and x - i >= 0 and y + i < rows) {
			Piece m = board[x - i][y + i];
			if (m.isEmpty()) {
				legalMoves.push_back(Move(board[x][y], m, 0, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.push_back(Move(board[x][y], m, 0, 1, 0, '-'));
				upRight = 0;
			} else upRight = 0;
		}

		if (downLeft and x + i < rows and y - i >= 0) {
			Piece m = board[x + i][y - i];
			if (m.isEmpty()) {
				legalMoves.push_back(Move(board[x][y], m, 0, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.push_back(Move(board[x][y], m, 0, 1, 0, '-'));
				downLeft = 0;
			} else downLeft = 0;
		}

		if (downRight and x + i < rows and y + i < rows) {
			Piece m = board[x + i][y + i];
			if (m.isEmpty()) {
				legalMoves.push_back(Move(board[x][y], m, 0, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.push_back(Move(board[x][y], m, 0, 1, 0, '-'));
				downRight = 0;
			} else downRight = 0;
		}
	} 
}

void Board::calculateLegalMovesRook(vector<Move> &legalMoves, int x, int y) {
	bool up = 1, right = 1, left = 1, down = 1;

	for (int i = 0; i < 8 and (up or left or right or down); i++) {
		if (up and x - i >= 0) {
			Piece m = board[x - i][y];
			if (m.isEmpty()) {
				legalMoves.push_back(Move(board[x][y], m, 0, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.push_back(Move(board[x][y], m, 0, 1, 0, '-'));
				up = 0;
			} else up = 0;
		}

		if (down and x + i < rows) {
			Piece m = board[x + i][y];
			if (m.isEmpty()) {
				legalMoves.push_back(Move(board[x][y], m, 0, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.push_back(Move(board[x][y], m, 0, 1, 0, '-'));
				down = 0;
			} else down = 0;
		}

		if (left and y - i >= 0) {
			Piece m = board[x][y - i];
			if (m.isEmpty()) {
				legalMoves.push_back(Move(board[x][y], m, 0, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.push_back(Move(board[x][y], m, 0, 1, 0, '-'));
				left = 0;
			} else left = 0;
		}

		if (right and y + i < rows) {
			Piece m = board[x][y + i];
			if (m.isEmpty()) {
				legalMoves.push_back(Move(board[x][y], m, 0, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.push_back(Move(board[x][y], m, 0, 1, 0, '-'));
				right = 0;
			} else right = 0;
		}
	} 
}

void Board::calculateLegalMoves(vector<Move> &legalMoves) {
	legalMoves = vector<Move> ();

	if (white) {
		int size = whitePieces.size();
		for (int i = 0; i < size; i++) {
			Piece p = *whitePieces[i];
			int x = p.getRank();
			int y = p.getFileAsInt();
			if (p.getType() == Pawn) {

				if (board[x - 1][y].isEmpty()) {
					legalMoves.push_back(Move(board[x][y], board[x - 1][y], 0, 0, 0, '-')); 
					
					if (x == 6 and board[x - 2][y].isEmpty()) {
						legalMoves.push_back(Move(board[x][y], board[x - 2][y], 0, 0, 1, '-'));
					} 
				} 
				
				for (int i = -1; i <= 1; i += 2) {
					if (y + i >= 0 and y + i < rows and 
						((!board[x - 1][y + i].isEmpty() and !board[x - 1][y + i].isWhite()) 
							or (!passant.isWhite() and passant.getRank() == x - 1 and passant.getFileAsInt() == y + i))) {
						legalMoves.push_back(Move(board[x][y], board[x - 2][y], 0, 1, 0, '-'));
					}
				}
			} else if (p.getType() == King) {
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						if (not (i == 0 and j == 0) and x + i >= 0 and x + i < rows and y + j >= 0 and y + j < rows) {
							Piece m = board[x + i][y + j];
							if (m.isEmpty()) {
								legalMoves.push_back(Move(board[x][y], m, 0, 0, 0, '-'));
							} else if (m.isWhite() != white) {
								legalMoves.push_back(Move(board[x][y], m, 0, 1, 0, '-'));
							}
						}
					}
				}

				//CASTLING

				if (castlingRights[0] and board[x][y + 1].isEmpty() and board[x][y + 2].isEmpty() and
				!isSquareAttacked(x, y + 1) and !isSquareAttacked(x, y + 2)) {
					legalMoves.push_back(Move(board[x][y], board[x][y + 2], 1, 0, 0, '-'));
				} 

				if (castlingRights[1] and board[x][y - 1].isEmpty() and board[x][y - 2].isEmpty() 
					and board[x][y - 3].isEmpty() and !isSquareAttacked(x, y - 1) and !isSquareAttacked(x, y - 2)) {
					legalMoves.push_back(Move(board[x][y], board[x][y - 2], 1, 0, 0, '-'));
				} 
			} else if (p.getType() == Knight) {
				for (int i = -2; i <= 2; i += 4) {
					for (int j = -1; j <= 1; j += 2) {
						if (x + i >= 0 and x + i < rows and y + j >= 0 and y + j < rows) {
							Piece m = board[x + i][y + j];
							if (m.isEmpty()) {
								legalMoves.push_back(Move(board[x][y], m, 0, 0, 0, '-'));
							} else if (!m.isWhite()) {
								legalMoves.push_back(Move(board[x][y], m, 0, 1, 0, '-'));
							}
						}

						if (x + j >= 0 and x + j < rows and y + i >= 0 and y + i < rows) {
							Piece m = board[x + j][y + i];
							if (m.isEmpty()) {
								legalMoves.push_back(Move(board[x][y], m, 0, 0, 0, '-'));
							} else if (!m.isWhite()) {
								legalMoves.push_back(Move(board[x][y], m, 0, 1, 0, '-'));
							}
						}
					}
				}
			} else if (p.getType() == Bishop) {
				calculateLegalMovesBishop(legalMoves, x, y);
			} else if (p.getType() == Rook) {
				calculateLegalMovesRook(legalMoves, x, y);
			} else if (p.getType() == Queen) {
				calculateLegalMovesBishop(legalMoves, x, y);
				calculateLegalMovesRook(legalMoves, x, y);
			} 
		}	
	} else {
		int size = blackPieces.size();
		for (int i = 0; i < size; i++) {
			Piece p = *blackPieces[i];
			int x = p.getRank();
			int y = p.getFileAsInt();
			if (p.getType() == Pawn) {

				if (board[x + 1][y].isEmpty()) {
					legalMoves.push_back(Move(board[x][y], board[x + 1][y], 0, 0, 0, '-')); 
					
					if (x == 1 and board[x + 2][y].isEmpty()) {
						legalMoves.push_back(Move(board[x][y], board[x + 2][y], 0, 0, 1, '-'));
					} 
				} 
				
				for (int i = -1; i <= 1; i += 2) {
					if (y + i >= 0 and y + i < rows and 
						((!board[x + 1][y + i].isEmpty() and !board[x + 1][y + i].isWhite()) 
							or (!passant.isWhite() and passant.getRank() == x + 1 and passant.getFileAsInt() == y + i))) {
						legalMoves.push_back(Move(board[x][y], board[x + 2][y], 0, 1, 0, '-'));
					}
				}
			} else if (p.getType() == King) {
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						if (not (i == 0 and j == 0) and x + i >= 0 and x + i < rows and y + j >= 0 and y + j < rows) {
							Piece m = board[x + i][y + j];
							if (m.isEmpty()) {
								legalMoves.push_back(Move(board[x][y], m, 0, 0, 0, '-'));
							} else if (m.isWhite() != white) {
								legalMoves.push_back(Move(board[x][y], m, 0, 1, 0, '-'));
							}
						}
					}
				}

				//CASTLING

				if (castlingRights[2] and board[x][y + 1].isEmpty() and board[x][y + 2].isEmpty() and
				!isSquareAttacked(x, y + 1) and !isSquareAttacked(x, y + 2)) {
					legalMoves.push_back(Move(board[x][y], board[x][y + 2], 1, 0, 0, '-'));
				} 

				if (castlingRights[3] and board[x][y - 1].isEmpty() and board[x][y - 2].isEmpty() 
					and board[x][y - 3].isEmpty() and !isSquareAttacked(x, y - 1) and !isSquareAttacked(x, y - 2)) {
					legalMoves.push_back(Move(board[x][y], board[x][y - 2], 1, 0, 0, '-'));
				} 
			} else if (p.getType() == Knight) {
				for (int i = -2; i <= 2; i += 4) {
					for (int j = -1; j <= 1; j += 2) {
						if (x + i >= 0 and x + i < rows and y + j >= 0 and y + j < rows) {
							Piece m = board[x + i][y + j];
							if (m.isEmpty()) {
								legalMoves.push_back(Move(board[x][y], m, 0, 0, 0, '-'));
							} else if (!m.isWhite()) {
								legalMoves.push_back(Move(board[x][y], m, 0, 1, 0, '-'));
							}
						}

						if (x + j >= 0 and x + j < rows and y + i >= 0 and y + i < rows) {
							Piece m = board[x + j][y + i];
							if (m.isEmpty()) {
								legalMoves.push_back(Move(board[x][y], m, 0, 0, 0, '-'));
							} else if (!m.isWhite()) {
								legalMoves.push_back(Move(board[x][y], m, 0, 1, 0, '-'));
							}
						}
					}
				}
			} else if (p.getType() == Bishop) {
				calculateLegalMovesBishop(legalMoves, x, y);
			} else if (p.getType() == Rook) {
				calculateLegalMovesRook(legalMoves, x, y);
			} else if (p.getType() == Queen) {
				calculateLegalMovesBishop(legalMoves, x, y);
				calculateLegalMovesRook(legalMoves, x, y);
			} 
		}	
	}
}