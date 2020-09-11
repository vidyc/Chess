
struct piece {
	bool empty;
	char idn;
	int rank;
	char file;
}

struct move {
	piece origin, destination;
	bool castling;
	bool capture;
	char promotion; // (N, B, R, Q, -)
	string notation;
	
	move(piece o, piece d, bool cast, bool capt, char prom) {
		origin = o;
		destination = d;
		castling = cast;
		capture = capt;
		promotion = prom;
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

			bool upLeft = 1, upRight = 1, downLeft = 1, downRight = 1;

			for (int i = 0; i < 8 and (upLeft or upRight or downLeft or downRight); i++) {
				if (upLeft) {
					piece m = board[x - i][y - i];
					if (m.empty) {
						legalMoves.pushback(move(board[x][y], m, 0, 0, '-'));
					} else if (m.idn >= "a" and m.idn <= "z") {
						legalMoves.pushback(move(board[x][y], m, 0, 1, '-'));
						upLeft = 0;
					} else upLeft = 0;
				}

				if (upRight) {
					piece m = board[x + i][y - i];
					if (m.empty) {
						legalMoves.pushback(/* e falta definir lo que es un movimiento todavia*/)
					} else if (m.idn >= "a" and m.idn <= "z") {
						legalMoves.pushback(/**/);
						upRight = 0;
					} else upRight = 0;
				}

				if (downLeft) {
					piece m = board[x - i][y + i];
					if (m.empty) {
						legalMoves.pushback(/* me falta definir lo que es un movimiento todavia*/)
					} else if (m.idn >= "a" and m.idn <= "z") {
						legalMoves.pushback(/**/);
						downLeft = 0;
					} else downLeft = 0;
				}

				if (downRight) {
					piece m = board[x + i][y + i];
					if (m.empty) {
						legalMoves.pushback(/* me falta definir lo que es un movimiento todavia*/)
					} else if (m.idn >= "a" and m.idn <= "z") {
						legalMoves.pushback(/**/);
						downRight = 0;
					} else downRight = 0;
				}
			} 

		} else if (board[x][y].idn == "R") {

		} else if (board[x][y].idn == "Q") {

		} 
	
	} else {

	}
}