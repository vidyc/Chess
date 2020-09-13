
void calculateLegalMovesBishop(vector<move> &legalMoves, int x, int y) {
	bool upLeft = 1, upRight = 1, downLeft = 1, downRight = 1;

	for (int i = 0; i < 8 and (upLeft or upRight or downLeft or downRight); i++) {
		if (upLeft and x - i >= 0 and y - i >= 0) {
			Piece m = board[x - i][y - i];
			if (m.isEmpty()) {
				legalMoves.push_back(Move(board[x][y], m, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.push_back(Move(board[x][y], m, 0, 1, '-'));
				upLeft = 0;
			} else upLeft = 0;
		}

		if (upRight and x - i >= 0 and y + i < rows) {
			Piece m = board[x - i][y + i];
			if (m.isEmpty()) {
				legalMoves.push_back(Move(board[x][y], m, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.push_back(Move(board[x][y], m, 0, 1, '-'));
				upRight = 0;
			} else upRight = 0;
		}

		if (downLeft and x + i < rows and y - i >= 0) {
			Piece m = board[x + i][y - i];
			if (m.isEmpty()) {
				legalMoves.push_back(Move(board[x][y], m, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.push_back(Move(board[x][y], m, 0, 1, '-'));
				downLeft = 0;
			} else downLeft = 0;
		}

		if (downRight and x + i < rows and y + i < rows) {
			Piece m = board[x + i][y + i];
			if (m.isEmpty()) {
				legalMoves.push_back(Move(board[x][y], m, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.push_back(Move(board[x][y], m, 0, 1, '-'));
				downRight = 0;
			} else downRight = 0;
		}
	} 
}

void calculateLegalMovesRook(vector<move> &legalMoves, int x, int y) {
	bool up = 1, right = 1, left = 1, down = 1;

	for (int i = 0; i < 8 and (up or left or right or down); i++) {
		if (up and x - i >= 0) {
			Piece m = board[x - i][y];
			if (m.isEmpty()) {
				legalMoves.push_back(Move(board[x][y], m, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.push_back(Move(board[x][y], m, 0, 1, '-'));
				up = 0;
			} else up = 0;
		}

		if (down and x + i < rows) {
			Piece m = board[x + i][y];
			if (m.isEmpty()) {
				legalMoves.push_back(Move(board[x][y], m, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.push_back(Move(board[x][y], m, 0, 1, '-'));
				down = 0;
			} else down = 0;
		}

		if (left and y - i >= 0) {
			Piece m = board[x][y - i];
			if (m.isEmpty()) {
				legalMoves.push_back(Move(board[x][y], m, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.push_back(Move(board[x][y], m, 0, 1, '-'));
				left = 0;
			} else left = 0;
		}

		if (right and y + i < rows) {
			Piece m = board[x][y + i];
			if (m.isEmpty()) {
				legalMoves.push_back(Move(board[x][y], m, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.push_back(Move(board[x][y], m, 0, 1, '-'));
				right = 0;
			} else right = 0;
		}
	} 
}


vector<move> calculateLegalMoves() {
	vector<move> legalMoves;

	if (white) {
		int size = whitePieces.size();
		for (int i = 0; i < size; i++) {
			Piece p = *whitePieces[i];
			int x = p.getRank();
			int y = p.getFileAsInt();
			if (p.getType() == Pawn) {
				if (board[x - 1][y].isEmpty()) {
					legalMoves.push_back(Move(board[x][y], board[x - 1][y], 0, 0, '-')); 
				} else if (x == 6 and board[x - 2][y].isEmpty()) {
					legalMoves.push_back(Move(board[x][y], board[x - 2][y], 0, 0, '-'));
				} else {
					for (int i = -1; i <= 1; i += 2) {
						if (y + i >= 0 and y - i < rows and 
							((!board[x - 1][y + i].isEmpty() and !board[x - 1][y + i].isWhite()) 
								or (!passant.isWhite() and passant.getRank() == x - 1 and passant.getFileAsInt() == y + i))) {
							legalMoves.push_back(Move(board[x][y], board[x - 1][y + i], 0, 1, '-'));
						}
					}
				}
			} else if (p.getType() == King) {
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						if (not (i == 0 and j == 0) and x + i >= 0 and x + i < rows and y + j >= 0 and y + j < rows) {
							Piece m = board[x + i][y + j];
							if (m.isEmpty()) {
								legalMoves.push_back(Move(board[x][y], m, 0, 0, '-'));
							} else if (m.isWhite() != white) {
								legalMoves.push_back(Move(board[x][y], m, 0, 1, '-'));
							}
						}
					}
				}

				//CASTLING

				if (castlingRights[0] and !isSquareAttacked(x, y + 1)
				 and !isSquareAttacked(x, y + 2)) {
					legalMoves.push_back(Move(board[x][y], board[x][y + 2], 1, 0, '-'));
				} 

				if (castlingRights[1] and !isSquareAttacked(x, y - 1)
				 and !isSquareAttacked(x, y - 2)) {
					legalMoves.push_back(Move(board[x][y], board[x][y - 2], 1, 0, '-'));
				} 
			} else if (p.getType() == Knight) {
				for (int i = -2; i <= 2; i += 4) {
					for (int j = -1; j <= 1; j += 2) {
						if (x + i >= 0 and x + i < rows and y + j >= 0 and y + j < rows) {
							Piece m = board[x + i][y + j];
							if (m.isEmpty()) {
								legalMoves.push_back(Move(board[x][y], m, 0, 0, '-'));
							} else if (!m.isWhite()) {
								legalMoves.push_back(Move(board[x][y], m, 0, 1, '-'));
							}
						}

						if (x + j >= 0 and x + j < rows and y + i >= 0 and y + i < rows) {
							Piece m = board[x + j][y + i];
							if (m.isEmpty()) {
								legalMoves.push_back(Move(board[x][y], m, 0, 0, '-'));
							} else if (!m.isWhite()) {
								legalMoves.push_back(Move(board[x][y], m, 0, 1, '-'));
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

	}
}