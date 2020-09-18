#include "Board.h"

Board::Board() {
	rows = 8;
	FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
	setBoard();
}

Board::Board(const string& FEN) {
	rows = 8;
	this -> FEN = FEN;
	setBoard();
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

bool Board::isKingInCheckmate() {
	int x, y;
	if (white) {
		x = whiteKing -> getRank();
		y = whiteKing -> getFileAsInt();
	} else {
		x = blackKing -> getRank();
		y = blackKing -> getFileAsInt();
	}

	bool mate = 1;

	for (int i = -1; mate and i <= 1; i++) {
		for (int j = -1; mate and j <= 1; j++) {
			if (!(i == 0 and j == 0) and x + i >= 0 and x + i < rows and y + j >= 0 and y + j < rows) {
				mate = (!board[x + i][y + j].isEmpty() and board[x + i][y + j].isWhite() == white) 
				or isSquareAttacked(x + i, y + j);
			}
		}
	}

	return mate;
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
	gameOver = 0;
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
					board[rank][file] = Piece(1, 0, '.', rank, file);
					file++;
				}
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

	passant = Piece(1, !white, '.', pass[1] + '0' - 1, pass[0] - 'a');

	check = isKingInCheck();
}

void Board::display() {
	cout << "  ABCDEFGH" << endl;
	for (int i = 0; i < rows; i++) {
		cout << 8 - i << "|";
		for (int j = 0; j < rows; j++) {
			cout << board[i][j].getIdn() << "";
		}
		cout << "|" << 8 - i << endl;
	}
	cout << "  ABCDEFGH" << endl;

	cout << endl;
	calculateLegalMoves();

	for (auto it = legalMoves.begin(); it != legalMoves.end(); it++) {
		cout << it -> second.getNotation() << " ";

		//Piece o = legalMoves[i].getOrigin();
		//Piece d = legalMoves[i].getDestination();

		//cout << o.getRank() << " " << o.getFileAsInt() << endl;
		//d.printType();
		//cout << " at " << d.getRank() << " " << d.getFileAsInt() << " " << d.getFile() << endl;

	}

	cout << endl;

	cout << FEN << endl;
}

bool Board::isMoveLegal(const string& move) {
	map<string, Move>::iterator it = legalMoves.find(move);
	return it != legalMoves.end();
}

int Board::findPiece(vector<Piece*> v, int rank, int file) {
	int size = v.size();
	int i = 0;
	while (!(v[i] -> getRank() == rank and v[i] -> getFileAsInt() == file) and i < size) {
		i++;
	}

	if (i == size) i = -1;
	//cout << rank << ", " << file << endl;
	//cout << i << endl;
	return i;
}

void Board::updateFEN(Move &m) {
	string aux = "";
	int size = board.size();
	for (int i = 0; i < size; i++) {
		int space = 0;
		for (int j = 0; j < size; j++) {
			Piece p = board[i][j];
			char c;
			if (p.getType() != Empty) {
				if (space != 0) {
					aux.push_back(space + '0');
					space = 0;
				}
				if (p.getType() == Pawn) {
					p.isWhite() ? c = 'P' : c = 'p';
				} else if (p.getType() == Bishop) {	
					p.isWhite() ? c = 'B' : c = 'b';
				} else if (p.getType() == Knight) {	
					p.isWhite() ? c = 'N' : c = 'n';
				} else if (p.getType() == Rook) {	
					p.isWhite() ? c = 'R' : c = 'r';
				} else if (p.getType() == Queen) {	
					p.isWhite() ? c = 'Q' : c = 'q';
				} else if (p.getType() == King) {	
					p.isWhite() ? c = 'K' : c = 'k';
				} 
				aux.push_back(c);
			} else {	
				space++;	
			}
		}
		if (space != 0) {
			aux.push_back(space + '0');
			space = 0;
		}
		if (i != size - 1) aux.push_back('/');
	}

	aux.push_back(' ');

	if (white) aux.push_back('w');
	else aux.push_back('b');

	aux.push_back(' ');

	if (castlingRights[0]) aux.push_back('K');
	else if (castlingRights[1]) aux.push_back('Q');
	else if (castlingRights[2]) aux.push_back('k');
	else if (castlingRights[3]) aux.push_back('q');

	aux.push_back(' ');

	if (m.allowsPassant()) {
		Piece d = m.getDestination();
		aux.push_back(d.getFile());
		if (white) aux.push_back(9 - d.getRank() + '0');
		else aux.push_back(7 - d.getRank() + '0');
	} else aux.push_back('-');

	aux.push_back(' ');

	aux.push_back(halfmoves + '0');

	aux.push_back(' ');

	aux.push_back(turn + '0');

	FEN = aux;
}

void Board::doMove(string& move) {
	//pre: move is a string that represents a legal move in this position
	//pos: the move is executed and the FEN is modified

	//tengo que actualizar el FEN, board y el vector de punteros de las negras si es captura.
	int index1, index2;
	Move m = legalMoves[move];
	Piece o = m.getOrigin(), d = m.getDestination();
	int x1 = o.getRank(), y1 = o.getFileAsInt();
	int x2 = d.getRank(), y2 = d.getFileAsInt();

	board[x2][y2].copyPiece(board[x1][y1]);
	board[x1][y1] = Piece(1, 0, '.', x1, y1);
	
	halfmoves++;

	if (white) {
		index1 = findPiece(whitePieces, x1, y1);
		
		whitePieces[index1] = &board[x2][y2];

		if (m.isCapture()) {
			if (!m.isPassant()) {
				index2 = findPiece(blackPieces, x2, y2);
			} else {
				board[x1][y2] = Piece(1, 0, '.', x1, y2);
				index2 = findPiece(blackPieces, x1, y2);
			}

			//cout << blackPieces[index2]->getRank() << ", " 
			//<< blackPieces[index2]->getFileAsInt() << endl;
			
			blackPieces.erase(blackPieces.begin() + index2);
			halfmoves = 0;
		} else if (m.allowsPassant()) passant = board[x2 + 1][y2];

		if (o.getType() == King) {
			castlingRights[0] = 0;
			castlingRights[1] = 0;
		} else if (o.getType() == Rook) {
			if (o.getFileAsInt() == 0) castlingRights[1] = 0;
			else if (o.getFileAsInt() == 7) castlingRights[0] = 0;
		}
	} else {
		index1 = findPiece(blackPieces, x1, y1);
		
		blackPieces[index1] = &board[x2][y2];

		if (m.isCapture()) {
			if (!m.isPassant()) {
				index2 = findPiece(whitePieces, x2, y2);	
			} else {
				board[x1][y2] = Piece(1, 0, '.', x1, y2);
				index2 = findPiece(whitePieces, x1, y2);
			}

			whitePieces.erase(whitePieces.begin() + index2);
			halfmoves = 0;
		} else if (m.allowsPassant()) passant = board[x2 - 1][y2];

		if (o.getType() == King) {
			castlingRights[2] = 0;
			castlingRights[3] = 0;
		} else if (o.getType() == Rook) {
			if (o.getFileAsInt() == 0) castlingRights[3] = 0;
			else if (o.getFileAsInt() == 7) castlingRights[2] = 0;
		}
		turn++;
	}

	if (o.getType() == Pawn) {
		halfmoves = 0;
		if (m.isPromotion()) {
			cout << "Promote to Queen Rook Bishop Knight ?" << endl;
			string s;
			do {
				cin >> s;
			} while((s != "Queen" and s != "Q") and (s != "Rook" and s != "R")
			 and (s != "Bishop" and s != "B") and (s != "Knight" and s != "K"));

			if (s == "Queen" or s == "Q") {
				board[x2][y2].setType(Queen);
			} else if (s == "Rook" or s == "R") {
				board[x2][y2].setType(Rook);
			} else if (s == "Bishop" or s == "B") {
				board[x2][y2].setType(Bishop);
			} else if (s == "Knight" or s == "K") {
				board[x2][y2].setType(Knight);
			}
		}
	}
	
	white = !white;

	check = isKingInCheck();

	if (check) {
		gameOver = isKingInCheckmate();
		cout << "CHECK!!!" << endl;

		if (gameOver) {
			cout << "CHECKMATE!!!" << endl;
			if (white) cout << "BLACK WINS!!!" << endl;
			else cout << "WHITE WINS!!!" << endl;
		}	
	}

	updateFEN(m);
}

bool Board::isGameOver() {
	return gameOver;
}

void Board::insertNewLegalMove(Move &move) {
	string key = move.getNotation(), n = move.getNotation();
	map<string, Move>::iterator it = legalMoves.find(key);

	if (it != legalMoves.end()) {
		Piece o1 = it -> second.getOrigin(), o2 = move.getOrigin();

		if (o1.getFileAsInt() != o2.getFileAsInt()) {
			key.insert(1, 1, o2.getFile());
			move.setNotation(key);
			legalMoves[key] = move;

			n.insert(1, 1, o1.getFile());
			it -> second.setNotation(n);
		} else if (o1.getRank() != o2.getRank()) {
			key.insert(1, 1, (8 - o2.getRank()) + '0');
			move.setNotation(key);
			legalMoves[key] = move;
 
			n.insert(1, 1, (8 - o1.getRank()) + '0');
			it -> second.setNotation(n);
		} else {
			//continuará...
		}

		auto node = legalMoves.extract(it);
		node.key() = n;
		legalMoves.insert(std::move(node));
	} else {
		legalMoves[key] = move;
	}
}

void Board::calculateLegalMovesBishop(int x, int y) {
	bool upLeft = 1, upRight = 1, downLeft = 1, downRight = 1;
	for (int i = 1; i < 8 and (upLeft or upRight or downLeft or downRight); i++) {
		if (upLeft and x - i >= 0 and y - i >= 0) {
			Piece m = board[x - i][y - i];
			if (m.isEmpty()) {
				Move move(board[x][y], m, 0, 0, 0, 0, 0); 
				insertNewLegalMove(move);;
			} else if (m.isWhite() != white) {
				Move move(board[x][y], m, 0, 1, 0, 0, 0); 
				insertNewLegalMove(move);;
				upLeft = 0;
			} else upLeft = 0;
		}

		if (upRight and x - i >= 0 and y + i < rows) {
			Piece m = board[x - i][y + i];
			if (m.isEmpty()) {
				Move move(board[x][y], m, 0, 0, 0, 0, 0); 
				insertNewLegalMove(move);;
			} else if (m.isWhite() != white) {
				Move move(board[x][y], m, 0, 1, 0, 0, 0); 
				insertNewLegalMove(move);;
				upRight = 0;
			} else upRight = 0;
		}

		if (downLeft and x + i < rows and y - i >= 0) {
			Piece m = board[x + i][y - i];
			if (m.isEmpty()) {
				Move move(board[x][y], m, 0, 0, 0, 0, 0); 
				insertNewLegalMove(move);;
			} else if (m.isWhite() != white) {
				Move move(board[x][y], m, 0, 1, 0, 0, 0); 
				insertNewLegalMove(move);;
				downLeft = 0;
			} else downLeft = 0;
		}

		if (downRight and x + i < rows and y + i < rows) {
			Piece m = board[x + i][y + i];
			if (m.isEmpty()) {
				Move move(board[x][y], m, 0, 0, 0, 0, 0); 
				insertNewLegalMove(move);;
			} else if (m.isWhite() != white) {
				Move move(board[x][y], m, 0, 1, 0, 0, 0); 
				insertNewLegalMove(move);;
				downRight = 0;
			} else downRight = 0;
		}
	} 
}

void Board::calculateLegalMovesRook(int x, int y) {
	bool up = 1, right = 1, left = 1, down = 1;

	for (int i = 1; i < 8 and (up or left or right or down); i++) {
		if (up and x - i >= 0) {
			Piece m = board[x - i][y];
			if (m.isEmpty()) {
				Move move(board[x][y], m, 0, 0, 0, 0, 0); 
				insertNewLegalMove(move);;
			} else if (m.isWhite() != white) {
				Move move(board[x][y], m, 0, 1, 0, 0, 0); 
				insertNewLegalMove(move);;
				up = 0;
			} else up = 0;
		}

		if (down and x + i < rows) {
			Piece m = board[x + i][y];
			if (m.isEmpty()) {
				Move move(board[x][y], m, 0, 0, 0, 0, 0); 
				insertNewLegalMove(move);;
			} else if (m.isWhite() != white) {
				Move move(board[x][y], m, 0, 1, 0, 0, 0); 
				insertNewLegalMove(move);;
				down = 0;
			} else down = 0;
		}

		if (left and y - i >= 0) {
			Piece m = board[x][y - i];
			if (m.isEmpty()) {
				Move move(board[x][y], m, 0, 0, 0, 0, 0); 
				insertNewLegalMove(move);;
			} else if (m.isWhite() != white) {
				Move move(board[x][y], m, 0, 1, 0, 0, 0); 
				insertNewLegalMove(move);;
				left = 0;
			} else left = 0;
		}

		if (right and y + i < rows) {
			Piece m = board[x][y + i];
			if (m.isEmpty()) {
				Move move(board[x][y], m, 0, 0, 0, 0, 0); 
				insertNewLegalMove(move);;
			} else if (m.isWhite() != white) {
				Move move(board[x][y], m, 0, 1, 0, 0, 0); 
				insertNewLegalMove(move);;
				right = 0;
			} else right = 0;
		}
	} 
}

void Board::calculateLegalMoves() {
	legalMoves.clear();

	if (white) {
		int size = whitePieces.size();
		for (int i = 0; i < size; i++) {
			Piece p = *whitePieces[i];
			int x = p.getRank();
			int y = p.getFileAsInt();
			if (p.getType() == Pawn) {

				if (board[x - 1][y].isEmpty()) {
					
					if (x - 1 == 0) {
						Move move(board[x][y], board[x - 1][y], 0, 0, 0, 0, 1); 
						insertNewLegalMove(move);;
					} else {
						Move move(board[x][y], board[x - 1][y], 0, 0, 0, 0, 0); 
						insertNewLegalMove(move);;
					}					
					
					if (x == 6 and board[x - 2][y].isEmpty()) {
						Move move2(board[x][y], board[x - 2][y], 0, 0, 0, 1, 0); 
						legalMoves[move2.getNotation()] = move2;
					} 
				} 
				
				for (int i = -1; i <= 1; i += 2) {
					if (y + i >= 0 and y + i < rows) { 
						if (!board[x - 1][y + i].isEmpty() and !board[x - 1][y + i].isWhite()) {
							if (x - 1 == 0) {
								Move move(board[x][y], board[x - 1][y + i], 0, 1, 0, 0, 1); 
								insertNewLegalMove(move);;
							} else {
								Move move(board[x][y], board[x - 1][y + i], 0, 1, 0, 0, 0); 
								insertNewLegalMove(move);;
							}
						} else if (!passant.isWhite() and passant.getRank() == x - 1 and passant.getFileAsInt() == y + i) {
							Move move(board[x][y], board[x - 1][y + i], 0, 1, 1, 0, 0); 
							insertNewLegalMove(move);;
						}
					}
				}
			} else if (p.getType() == King) {
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						if (not (i == 0 and j == 0) and x + i >= 0 and x + i < rows and y + j >= 0 and y + j < rows) {
							Piece m = board[x + i][y + j];
							if (m.isEmpty()) {
								Move move(board[x][y], m, 0, 0, 0, 0, 0); 
								insertNewLegalMove(move);;
							} else if (m.isWhite() != white) {
								Move move(board[x][y], m, 0, 1, 0, 0, 0); 
								insertNewLegalMove(move);;
							}
						}
					}
				}

				//CASTLING

				if (castlingRights[0] and board[x][y + 1].isEmpty() and board[x][y + 2].isEmpty() and
				!isSquareAttacked(x, y + 1) and !isSquareAttacked(x, y + 2)) {
					Move move(board[x][y], board[x][y + 2], 1, 0, 0, 0, 0); 
					insertNewLegalMove(move);;
				} 

				if (castlingRights[1] and board[x][y - 1].isEmpty() and board[x][y - 2].isEmpty() 
					and board[x][y - 3].isEmpty() and !isSquareAttacked(x, y - 1) and !isSquareAttacked(x, y - 2)) {
						Move move(board[x][y], board[x][y - 2], 1, 0, 0, 0, 0);
						insertNewLegalMove(move);;
				} 
			} else if (p.getType() == Knight) {
				for (int i = -2; i <= 2; i += 4) {
					for (int j = -1; j <= 1; j += 2) {
						if (x + i >= 0 and x + i < rows and y + j >= 0 and y + j < rows) {
							Piece m = board[x + i][y + j];
							if (m.isEmpty()) {
								Move move(board[x][y], m, 0, 0, 0, 0, 0); 
								insertNewLegalMove(move);;
							} else if (!m.isWhite()) {
								Move move(board[x][y], m, 0, 1, 0, 0, 0); 
								insertNewLegalMove(move);;
							}
						}

						if (x + j >= 0 and x + j < rows and y + i >= 0 and y + i < rows) {
							Piece m = board[x + j][y + i];
							if (m.isEmpty()) {
								Move move(board[x][y], m, 0, 0, 0, 0, 0); 
								insertNewLegalMove(move);;
							} else if (!m.isWhite()) {
								Move move(board[x][y], m, 0, 1, 0, 0, 0); 
								insertNewLegalMove(move);;
							}
						}
					}
				}
			} else if (p.getType() == Bishop) {
				calculateLegalMovesBishop(x, y);
			} else if (p.getType() == Rook) {
				calculateLegalMovesRook(x, y);
			} else if (p.getType() == Queen) {
				calculateLegalMovesBishop(x, y);
				calculateLegalMovesRook(x, y);
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
					if (x + 1 == rows - 1) {
						Move move(board[x][y], board[x + 1][y], 0, 0, 0, 0, 1); 
						insertNewLegalMove(move);;
					} else {
						Move move(board[x][y], board[x + 1][y], 0, 0, 0, 0, 0); 
						insertNewLegalMove(move);;
					}
					
					if (x == 1 and board[x + 2][y].isEmpty()) {
						Move move(board[x][y], board[x + 2][y], 0, 0, 0, 1, 0);
						insertNewLegalMove(move);;
					} 
				} 
				
				for (int i = -1; i <= 1; i += 2) {
					if (y + i >= 0 and y + i < rows) {
						if (!board[x + 1][y + i].isEmpty() and board[x + 1][y + i].isWhite()) {
							if (x + 1 == rows - 1) {
								Move move(board[x][y], board[x + 1][y + i], 0, 1, 0, 0, 1); 
								insertNewLegalMove(move);;
							} else {
								Move move(board[x][y], board[x + 1][y + i], 0, 1, 0, 0, 0); 
								insertNewLegalMove(move);;
							}
						} else if (passant.isWhite() and passant.getRank() == x + 1 and passant.getFileAsInt() == y + i) {
							Move move(board[x][y], board[x + 1][y + i], 0, 1, 1, 0, 0); 
							insertNewLegalMove(move);;
						}
					}
				}
			} else if (p.getType() == King) {
				for (int i = -1; i <= 1; i++) {
					for (int j = -1; j <= 1; j++) {
						if (not (i == 0 and j == 0) and x + i >= 0 and x + i < rows and y + j >= 0 and y + j < rows) {
							Piece m = board[x + i][y + j];
							if (m.isEmpty()) {
								Move move(board[x][y], m, 0, 0, 0, 0, 0); 
								insertNewLegalMove(move);;
							} else if (m.isWhite() != white) {
								Move move(board[x][y], m, 0, 1, 0, 0, 0); 
								insertNewLegalMove(move);;
							}
						}
					}
				}

				//CASTLING

				if (castlingRights[2] and board[x][y + 1].isEmpty() and board[x][y + 2].isEmpty() and
				!isSquareAttacked(x, y + 1) and !isSquareAttacked(x, y + 2)) {
					Move move(board[x][y], board[x][y + 2], 1, 0, 0, 0, 0); 
					insertNewLegalMove(move);;
				} 

				if (castlingRights[3] and board[x][y - 1].isEmpty() and board[x][y - 2].isEmpty() 
					and board[x][y - 3].isEmpty() and !isSquareAttacked(x, y - 1) and !isSquareAttacked(x, y - 2)) {
							Move move(board[x][y], board[x][y - 2], 1, 0, 0, 0, 0);
							insertNewLegalMove(move);;
				} 
			} else if (p.getType() == Knight) {
				for (int i = -2; i <= 2; i += 4) {
					for (int j = -1; j <= 1; j += 2) {
						if (x + i >= 0 and x + i < rows and y + j >= 0 and y + j < rows) {
							Piece m = board[x + i][y + j];
							if (m.isEmpty()) {
								Move move(board[x][y], m, 0, 0, 0, 0, 0); 
								insertNewLegalMove(move);;
							} else if (m.isWhite()) {
								Move move(board[x][y], m, 0, 1, 0, 0, 0); 
								insertNewLegalMove(move);;
							}
						}

						if (x + j >= 0 and x + j < rows and y + i >= 0 and y + i < rows) {
							Piece m = board[x + j][y + i];
							if (m.isEmpty()) {
								Move move(board[x][y], m, 0, 0, 0, 0, 0); 
								insertNewLegalMove(move);;
							} else if (m.isWhite()) {
								Move move(board[x][y], m, 0, 1, 0, 0, 0); 
								insertNewLegalMove(move);;
							}
						}
					}
				}
			} else if (p.getType() == Bishop) {
				calculateLegalMovesBishop(x, y);
			} else if (p.getType() == Rook) {
				calculateLegalMovesRook(x, y);
			} else if (p.getType() == Queen) {
				calculateLegalMovesBishop(x, y);
				calculateLegalMovesRook(x, y);
			} 
		}	
	}
}