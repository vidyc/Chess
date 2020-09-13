void calculateLegalMovesBishop(vector<move> &legalMoves) {
	bool upLeft = 1, upRight = 1, downLeft = 1, downRight = 1;

	for (int i = 0; i < 8 and (upLeft or upRight or downLeft or downRight); i++) {
		if (upLeft and x - i >= 0 and y - i >= 0) {
			Piece m = board[x - i][y - i];
			if (m.isEmpty()) {
				legalMoves.pushback(Move(board[x][y], m, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.pushback(Move(board[x][y], m, 0, 1, '-'));
				upLeft = 0;
			} else upLeft = 0;
		}

		if (upRight and x - i >= 0 and y + i < rows) {
			Piece m = board[x - i][y + i];
			if (m.isEmpty()) {
				legalMoves.pushback(Move(board[x][y], m, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.pushback(Move(board[x][y], m, 0, 1, '-'));
				upRight = 0;
			} else upRight = 0;
		}

		if (downLeft and x + i < rows and y - i >= 0) {
			Piece m = board[x + i][y - i];
			if (m.isEmpty()) {
				legalMoves.pushback(Move(board[x][y], m, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.pushback(Move(board[x][y], m, 0, 1, '-'));
				downLeft = 0;
			} else downLeft = 0;
		}

		if (downRight and x + i < rows and y + i < rows) {
			Piece m = board[x + i][y + i];
			if (m.isEmpty()) {
				legalMoves.pushback(Move(board[x][y], m, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.pushback(Move(board[x][y], m, 0, 1, '-'));
				downRight = 0;
			} else downRight = 0;
		}
	} 
}

void calculateLegalMovesRook(vector<move> &legalMoves) {
	bool up = 1, right = 1, left = 1, down = 1;

	for (int i = 0; i < 8 and (up or left or right or down); i++) {
		if (up and x - i >= 0) {
			Piece m = board[x - i][y];
			if (m.isEmpty()) {
				legalMoves.pushback(Move(board[x][y], m, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.pushback(Move(board[x][y], m, 0, 1, '-'));
				up = 0;
			} else up = 0;
		}

		if (down and x + i < rows) {
			Piece m = board[x + i][y];
			if (m.isEmpty()) {
				legalMoves.pushback(Move(board[x][y], m, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.pushback(Move(board[x][y], m, 0, 1, '-'));
				down = 0;
			} else down = 0;
		}

		if (left and y - i >= 0) {
			Piece m = board[x][y - i];
			if (m.isEmpty()) {
				legalMoves.pushback(Move(board[x][y], m, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.pushback(Move(board[x][y], m, 0, 1, '-'));
				left = 0;
			} else left = 0;
		}

		if (right and y + i < rows) {
			Piece m = board[x][y + i];
			if (m.isEmpty()) {
				legalMoves.pushback(Move(board[x][y], m, 0, 0, '-'));
			} else if (m.isWhite() != white) {
				legalMoves.pushback(Move(board[x][y], m, 0, 1, '-'));
				right = 0;
			} else right = 0;
		}
	} 
}


vector<move> calculateLegalMoves() {
	//sabemos la "x" y la "y"
	vector<move> legalMoves;

	if (white) {
		if (board[x][y].idn == "P") {
			
		} else if (board[x][y].idn == "K") {

		} else if (board[x][y].idn == "N") {

		} else if (board[x][y].idn == "B") {
			calculateLegalMovesBishop(legalMoves);
		} else if (board[x][y].idn == "R") {
			calculateLegalMovesRook(legalMoves);
		} else if (board[x][y].idn == "Q") {
			calculateLegalMovesBishop(legalMoves);
			calculateLegalMovesRook(legalMoves);
		} 
	
	} else {

	}
}