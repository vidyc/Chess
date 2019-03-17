#include <iostream>
#include <vector>
using namespace std;

struct Piece {
    string type, team;
    char idn;
    int i, j;
};

struct Square {
    Piece piece;
};

struct Move {
    string type, idn;
    Piece piece, pieceCaptured;
    int i, j; //coordenadas de destino
};

typedef vector<Square> Row;
typedef vector<Row> Board;

bool wqCastling = true, wkCastling = true, bqCastling = true, bkCastling = true;
bool whiteCheck = false, blackCheck = false;

int abs (int n) {
    if (n < 0) return -n;
    return n;
}

void insert(vector<Move>& v1, const vector<Move>& v2) {
    int size = v2.size();
    for (int i = 0; i < size; i++) v1.push_back(v2[i]);
}

void updatePieceArray(vector<Piece> &pieces, int oldI, int oldJ, int newI, int newJ) {
    int x = 0, size = pieces.size();

    while (x < size and !(pieces[x].i == oldI and pieces[x].j == oldJ)) ++x;
    if (x < size) {
        pieces[x].i = newI;
        pieces[x].j = newJ;
    } 
}

Piece getKing(vector<Piece> pieces) {
    int size = pieces.size();
    for (int i = 0; i < size; i++) if (pieces[i].type == "king") return pieces[i];
    
    return pieces[0];
}

Move setMove(Piece piece, Piece pieceCaptured, string type) {
    Move move;
    move.piece = piece;
    move.pieceCaptured = pieceCaptured;
    move.i = pieceCaptured.i;
    move.j = pieceCaptured.j;
    move.type = type;
    
    if (move.type == "castling") pieceCaptured.j == 7 ? move.idn = "O-O" : move.idn = "O-O-O";
    else {
        move.idn.push_back(piece.idn);
        if (move.type == "capture") move.idn.push_back('x');
        move.idn.push_back(char(move.j + 'a'));
        move.idn.push_back((8 - move.i) + '0');
    }
    return move;
}

void setPiece(Square& square, string type, string team, char idn, int i, int j) {
    square.piece.type = type;
    square.piece.team = team;
    square.piece.idn = idn;
    square.piece.i = i;
    square.piece.j = j;

} 

void move(Board& board, Move m, int &turn, vector<Piece> &pieces) {
    if (m.type == "castling") {
        if (m.j == 7) {
            setPiece(board[m.piece.i][6], m.piece.type, m.piece.team, m.piece.idn, m.piece.i, 6);
            setPiece(board[m.piece.i][5], m.pieceCaptured.type, m.pieceCaptured.team, m.pieceCaptured.idn, m.piece.i, 5);
            setPiece(board[m.piece.i][7], "empty", " ", 'o', m.piece.i, 7);

            updatePieceArray(pieces, m.piece.i, m.piece.j, m.piece.i, 6);
            updatePieceArray(pieces, m.piece.i, m.pieceCaptured.j, m.piece.i, 5);
        } else {
            setPiece(board[m.piece.i][2], m.piece.type, m.piece.team, m.piece.idn, m.piece.i, 2);
            setPiece(board[m.piece.i][3], m.pieceCaptured.type, m.pieceCaptured.team, m.pieceCaptured.idn, m.piece.i, 3);
            setPiece(board[m.piece.i][0], "empty", " ", 'o', m.piece.i, 0);

            updatePieceArray(pieces, m.piece.i, m.piece.j, m.piece.i, 2);
            updatePieceArray(pieces, m.piece.i, m.pieceCaptured.j, m.piece.i, 3);
        }
        setPiece(board[m.piece.i][m.piece.j], "empty", " ", 'o', m.piece.i, m.piece.j);
    } else {
        int i = m.i, j = m.j;
        /*if (m.type == "coronacion" and m.piece.type == "pawn" and m.i % 7 == 0) {
            string type;
            cout << "Promote to: " << endl;
            cout << "knight --- bishop --- rook --- queen" << endl;
            cin >> type;
            while (type != "knight" and type != "bishop" and type != "rook" and type != "queen") {
                cout << "no me muevo de aqui hasta que me des un tipo correcto" << endl;
                cin >> type;
            }
            m.piece.type = type;
            if (type == "knight") m.piece.idn = 'K';
            else if (type == "bishop") m.piece.idn = 'B';
            else if (type == "rook") m.piece.idn = 'R';
            else m.piece.idn = 'Q';
        } */
        updatePieceArray(pieces, m.piece.i, m.piece.j, i, j);
        setPiece(board[m.piece.i][m.piece.j], "empty", " ", 'o', m.piece.i, m.piece.j); 
        setPiece(board[i][j], m.piece.type, m.piece.team, m.piece.idn, i, j);
    }

    turn++;
}
    
bool isSquareAttacked(Board board, const Square &square, string team) {
    int i = square.piece.i, j = square.piece.j, inc = 1;
    bool found = false, endUL = false, endUR = false, endDL = false, endDR = false;
    while (!found and inc < 8) {
        int targ1 = i - inc, targ2 = j;
        if (targ1 >= 0 and board[targ1][targ2].piece.type != "empty") {
            Piece piece = board[targ1][targ2].piece;
            if (piece.team != team and (piece.type == "rook" or piece.type == "queen")) found = true;
            else for (int e = targ1 - 1; e >= 0; e--) board[e][targ2].piece = piece;
        }
        
        targ1 = i + inc;
        if (targ1 < 8 and board[targ1][targ2].piece.type != "empty") {
            Piece piece = board[targ1][targ2].piece;
            if (piece.team != team and (piece.type == "rook" or piece.type == "queen")) found = true;
            else for (int e = targ1 + 1; e < 8; e++) board[e][targ2].piece = piece;
        }

        targ1 = j - inc, targ2 = i;
        if (targ1 >= 0 and board[targ2][targ1].piece.type != "empty") {
            Piece piece = board[targ2][targ1].piece;
            if (piece.team != team and (piece.type == "rook" or piece.type == "queen")) found = true;
            else for (int e = targ1 - 1; e >= 0; e--) board[targ2][e].piece = piece;
        }

        targ1 = j + inc;
        if (targ1 < 8 and board[targ2][targ1].piece.type != "empty") {
            Piece piece = board[targ2][targ1].piece;
            if (piece.team != team and (piece.type == "rook" or piece.type == "queen")) found = true;
            else for (int e = targ1 + 1; e < 8; e++) board[targ2][e].piece = piece;
        }

        targ1 = i - inc, targ2 = j - inc;
        if (!endUL and targ1 >= 0 and targ2 >= 0 and board[targ1][targ2].piece.type != "empty") {
            Piece piece = board[targ1][targ2].piece;
            if (piece.team != team and (piece.type == "bishop" or piece.type == "queen")) found = true; 
            else endUL = true;
        }
        
        targ1 = i - inc, targ2 = j + inc;
        if (!endUR and targ1 >= 0 and targ2 < 8 and board[targ1][targ2].piece.type != "empty") {
            Piece piece = board[targ1][targ2].piece;
            if (piece.team != team and (piece.type == "bishop" or piece.type == "queen")) found = true; 
            else endUR = true;
        }
        
        targ1 = i + inc, targ2 = j - inc;
        if (!endDL and targ1 < 8 and targ2 >= 0 and board[targ1][targ2].piece.type != "empty") {
            Piece piece = board[targ1][targ2].piece;
            if (piece.team != team and (piece.type == "bishop" or piece.type == "queen")) found = true; 
            else endDL = true;
        }
        
        targ1 = i + inc, targ2 = j + inc;
        if (!endDR and targ1 < 8 and targ2 < 8 and board[targ1][targ2].piece.type != "empty") {
            Piece piece = board[targ1][targ2].piece;
            if (piece.team != team and (piece.type == "bishop" or piece.type == "queen")) found = true; 
            else endDR = true;
        }      
        ++inc;
    }
    
    //caballos
    for (int incI = -1; incI < 2; incI += 2) {
        for (int incJ = -2; incJ < 3; incJ += 4)
            if (i + incI >= 0 and i + incI < 8 and j + incJ >= 0 and j + incJ < 8) {
                Piece piece = board[i + incI][j + incJ].piece;
                if (piece.type == "knight" and piece.team != team) found = true;
            }
    }
        
    for (int incI = -2; incI < 3; incI += 4) {
        for (int incJ = -1; incJ < 2; incJ += 2) {
            if (i + incI >= 0 and i + incI < 8 and j + incJ >= 0 and j + incJ < 8) {
                Piece piece = board[i + incI][j + incJ].piece;
                if (piece.type == "knight" and piece.team != team) found = true;
            }
        }
    }

    //peones
    if (team == "white") {
    	for (int x = -1; x < 2; x += 2) 
    		if (j + x >= 0 and j + x < 8 and i > 0 and board[i - 1][j + x].piece.type == "pawn" and board[i - 1][j + x].piece.team != team) found = true; 
	} else if (team == "black") {
		for (int x = -1; x < 2; x += 2) 
    		if (j + x >= 0 and j + x < 8 and i < 7 and board[i + 1][j + x].piece.type == "pawn" and board[i + 1][j + x].piece.team != team) found = true; 
	} else {
		for (int x = -1; x < 2; x += 2) {
    		if (j + x >= 0 and j + x < 8 and ((i > 0 and board[i - 1][j + x].piece.type == "pawn") or (i < 7 and board[i + 1][j + x].piece.type == "pawn")))
                found = true; 
		}
	}
    
    return found;
}

bool isLegal(const Board &board, Move m, const vector<Piece> pieces) {
        Board copy = board;
        int n = 0;
        vector<Piece> copieces = pieces;
        move(copy, m, n, copieces);
        Piece king = getKing(copieces);
        return !isSquareAttacked(copy, copy[king.i][king.j], king.team);
}

vector<Move> calculateLegalMoves(const Board& board, Piece piece, Move previousMove, vector<Piece> &pieces) {
    vector<Move> legalMoves;
    int i = piece.i, j = piece.j;
    //cout << piece.type << ' ' << piece.idn << char(j + 'a') << 8 - i << endl;
    if (piece.type == "rook" or piece.type == "queen") {
        int inc = 1;
        bool endU = false, endD = false, endL = false, endR = false;
        while ((!endU or !endD or !endL or !endR) and inc < 8) {

            if (!endU)
                if (i - inc >= 0 and board[i - inc][j].piece.type == "empty") {
                    Move m = setMove(piece, board[i - inc][j].piece, "normal");
                    if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                } else {
                    if (i - inc >= 0 and board[i - inc][j].piece.team != piece.team) {
                        Move m = setMove(piece, board[i - inc][j].piece, "capture");
                        if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                    }
                    endU = true;
                }

            if (!endD)
                if (i + inc < 8 and board[i + inc][j].piece.type == "empty") {
                    Move m = setMove(piece, board[i + inc][j].piece, "normal");
                    if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                } else {
                    if (i + inc < 8 and board[i + inc][j].piece.team != piece.team) {
                        Move m = setMove(piece, board[i + inc][j].piece, "capture");
                        if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                    }
                    endD = true;
                }

            if (!endR)
                if (j + inc < 8 and board[i][j + inc].piece.type == "empty") {
                    Move m = setMove(piece, board[i][j + inc].piece, "normal");
                    if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                }
                else {
                    if (j + inc < 8 and board[i][j + inc].piece.team != piece.team) {
                        Move m = setMove(piece, board[i][j + inc].piece, "capture");
                        if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                    }
                    endR = true;
                }
            
            if (!endL)
                if (j - inc >= 0 and board[i][j - inc].piece.type == "empty") {
                    Move m = setMove(piece, board[i][j - inc].piece, "normal");
                    if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                }
                else {
                    if (j - inc >= 0 and board[i][j - inc].piece.team != piece.team) {
                        Move m = setMove(piece, board[i][j - inc].piece, "capture");
                        if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                    }
                    endL = true;
                }
            inc++;
        }
    }
    if (piece.type == "bishop" or piece.type == "queen") {
        int inc = 1;
        bool endUL = false, endUR = false, endDL = false, endDR = false;
        while ((!endUL or !endUR or !endDL or !endDR) and inc < 8) {
            
            if (!endUL)
                if (i - inc >= 0 and j - inc >= 0 and board[i - inc][j - inc].piece.type == "empty") {
                    Move m = setMove(piece, board[i - inc][j - inc].piece, "normal");
                    if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                }
                else {
                    if (i - inc >= 0 and j - inc >= 0 and board[i - inc][j - inc].piece.team != piece.team) {
                        Move m = setMove(piece, board[i - inc][j - inc].piece, "capture");
                        if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                    }
                    endUL = true;
                }
                
            if (!endDR)
                if (i + inc < 8 and j + inc < 8 and board[i + inc][j + inc].piece.type == "empty") {
                    Move m = setMove(piece, board[i + inc][j + inc].piece, "normal");
                    if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                }
                else {
                    if (i + inc < 8 and j + inc < 8 and board[i + inc][j + inc].piece.team != piece.team) {
                        Move m = setMove(piece, board[i + inc][j + inc].piece, "capture");
                        if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                    }
                    endDR = true;
                }

            if (!endUR)   
                if (i - inc >= 0 and j + inc < 8 and board[i - inc][j + inc].piece.type == "empty") {
                    Move m = setMove(piece, board[i - inc][j + inc].piece, "normal");
                    if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                }
                else {
                    if (i - inc >= 0 and j + inc < 8 and board[i - inc][j + inc].piece.team != piece.team) {
                        Move m = setMove(piece, board[i - inc][j + inc].piece, "capture");
                        if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                    }
                    endUR = true;
                }
            
            if (!endDL)
                if (i + inc < 8 and j - inc >= 0 and board[i + inc][j - inc].piece.type == "empty") {
                    Move m = setMove(piece, board[i + inc][j - inc].piece, "normal");
                    if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                }
                else {
                    if (i + inc < 8 and j - inc >= 0 and board[i + inc][j - inc].piece.team != piece.team) {
                        Move m = setMove(piece, board[i + inc][j - inc].piece, "capture");
                        if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                    }
                    endDL = true;
                }
            inc++;
        }
        
    }
    else if (piece.type == "king") {
        for (int incI = -1; incI < 2; incI++)
            for (int incJ = -1; incJ < 2; incJ++) {
                if (i + incI >= 0 and i + incI < 8 and j + incJ >= 0 and j + incJ < 8) {
                    Piece possPiece = board[i + incI][j + incJ].piece;
                    if (possPiece.type == "empty") {
                        Move m = setMove(piece, possPiece, "normal");
                        if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                    } else if (possPiece.team != piece.team) {
                        Move m = setMove(piece, possPiece, "capture");
                        if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                    }
                }
            }
        
        //enroque
      /*  if (piece.team == "white") { 
            if (wkCastling and !isSquareAttacked(board[7][piece.j + 1]) and !isSquareAttacked(board[7][piece.j + 2]) 
            and board[7][piece.j + 1].piece.type == "empty" and board[7][piece.j + 2].piece.type == "empty")
                legalMoves.push_back(setMove(piece, board[piece.i][piece.j + 3].piece, "castling"));
            if (wqCastling and !isSquareAttacked(board[7][piece.j - 1]) and !isSquareAttacked(board[7][piece.j - 2]) 
            and board[7][piece.j - 1].piece.type == "empty" and board[7][piece.j - 2].piece.type == "empty" and 
            board[7][piece.j - 3].piece.type == "empty")
                legalMoves.push_back(setMove(piece, board[piece.i][piece.j - 4].piece, "castling"));
        } else {
            if (bkCastling and !isSquareAttacked(board[0][piece.j + 1]) and !isSquareAttacked(board[0][piece.j + 2]) 
            and board[0][piece.j + 1].piece.type == "empty" and board[0][piece.j + 2].piece.type == "empty") 
                legalMoves.push_back(setMove(piece, board[piece.i][piece.j + 3].piece, "castling"));
            if (bqCastling and !isSquareAttacked(board[0][piece.j - 1]) and !isSquareAttacked(board[0][piece.j - 2]) 
            and board[0][piece.j - 1].piece.type == "empty" and board[0][piece.j -2].piece.type == "empty" and 
            board[0][piece.j - 3].piece.type == "empty") 
                legalMoves.push_back(setMove(piece, board[piece.i][piece.j - 4].piece, "castling"));
        }*/
    }
    else if (piece.type == "pawn") {
        //movimiento normal y captura
        int inc = -1;
        if (piece.team == "black") inc = 1;
        
        if (board[i + inc][j].piece.type == "empty") {
            Move m = setMove(piece, board[i + inc][j].piece, "normal");
            if ((i + inc) % 7 == 0) m = setMove(piece, board[i + inc][j].piece, "coronacion");
            if (isLegal(board, m, pieces)) legalMoves.push_back(m);
        }

        for (int incJ = -1; incJ < 2; incJ += 2) {
            if (j + incJ >= 0 and j + incJ < 8) {
                Piece pieceToCapture = board[i + inc][j + incJ].piece;
                if (pieceToCapture.type != "empty" and pieceToCapture.team != piece.team) {
                    Move m = setMove(piece, pieceToCapture, "capture");
                    if (pieceToCapture.i % 7 == 0) m = setMove(piece, pieceToCapture, "coronacion");
                    if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                }
            }
        }
        
        //doble avance
        if ((piece.team == "white" and i == 6) or (piece.team == "black" and i == 1)) 
            if (board[i + 2 * inc][j].piece.type == "empty" and board[i + inc][j].piece.type == "empty") {
                Move m = setMove(piece, board[i + 2 * inc][j].piece, "normal");
                if (isLegal(board, m, pieces)) legalMoves.push_back(m);
            }
            
        //en passant
        if (previousMove.piece.type == "pawn" and abs(previousMove.piece.i - previousMove.i) == 2 and (previousMove.j - 1 == j or previousMove.j + 1 == j) and i == previousMove.i) {
            Move m = setMove(piece, board[previousMove.i][previousMove.j].piece, "passant");
            if (isLegal(board, m, pieces)) legalMoves.push_back(m);
        }
    }
    else if (piece.type == "knight") {
        
        for (int incI = -1; incI < 2; incI += 2) {
            for (int incJ = -2; incJ < 3; incJ += 4) {
                if (i + incI >= 0 and i + incI < 8 and j + incJ >= 0 and j + incJ < 8) {
                    Piece pieceToCapture = board[i + incI][j + incJ].piece;
                    if (pieceToCapture.type == "empty") {
                        Move m = setMove(piece, pieceToCapture, "normal");
                        if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                    } else if (piece.team != pieceToCapture.team) {
                        Move m = setMove(piece, pieceToCapture, "capture");
                        if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                    }
                }
            }
        }
        
        for (int incI = -2; incI < 3; incI += 4) {
            for (int incJ = -1; incJ < 2; incJ += 2) {
                if (i + incI >= 0 and i + incI < 8 and j + incJ >= 0 and j + incJ < 8) {
                    Piece pieceToCapture = board[i + incI][j + incJ].piece;
                    if (pieceToCapture.type == "empty") {
                        Move m = setMove(piece, pieceToCapture, "normal");
                        if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                    } else if (piece.team != pieceToCapture.team) {
                        Move m = setMove(piece, pieceToCapture, "capture");
                        if (isLegal(board, m, pieces)) legalMoves.push_back(m);
                    }
                }
            }
        }
    }
    return legalMoves;
}

vector<Move> calculateLegalMoves(const Board& board, Move previousMove, int turn, vector<Piece> pieces) {
    vector<Move> legalMoves;
    if (turn % 2 != 0) {
        Piece king = getKing(pieces);
        whiteCheck = isSquareAttacked(board, board[king.i][king.j], king.team);
        if (whiteCheck) cout << "check boi !!!" << endl;
	    for (int i = 0; i < 8; i++)
	        for (int j = 0; j < 8; j++) {
	            Piece piece = board[i][j].piece;
	            if (piece.team == "white") insert(legalMoves, calculateLegalMoves(board, piece, previousMove, pieces));
	        }
	} else {
        Piece king = getKing(pieces);
        blackCheck = isSquareAttacked(board, board[king.i][king.j], king.team);
		if (blackCheck) cout << "check boi !!!" << endl;
        for (int i = 0; i < 8; i++)
	        for (int j = 0; j < 8; j++) {
	        	Piece piece = board[i][j].piece;
	            if (piece.team == "black") insert(legalMoves, calculateLegalMoves(board, piece, previousMove, pieces));
	        }
	}
	return legalMoves;
}

Board setBoard(vector<Piece> &whitePieces, vector<Piece> &blackPieces) {
    Board board(8, Row(8));
     for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 8; j++) {
            if (i == 0 and (j == 0 or j == 7)) setPiece(board[i][j], "rook", "black", 'R', i, j);
            else if (i == 0 and (j == 1 or j == 6)) setPiece(board[i][j], "knight", "black", 'K', i, j);
            else if (i == 0 and (j == 2 or j == 5)) setPiece(board[i][j], "bishop", "black", 'B', i, j);
            else if (i == 0 and j == 3) setPiece(board[i][j], "queen", "black", 'Q', i, j);
            else if (i == 0 and j == 4) setPiece(board[i][j], "king", "black", '#', i, j);
            else if (i == 1) setPiece(board[i][j], "pawn", "black", 'P', i, j);

            blackPieces.push_back(board[i][j].piece);
        }
    }
    
    for (int i = 2; i < 7; i++) 
        for (int j = 0; j < 8; j++) 
            setPiece(board[i][j], "empty", " ", 'o', i, j);    
    
    for (int i = 6; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if (i == 7 and (j == 0 or j == 7)) setPiece(board[i][j], "rook", "white", 'R', i, j);
            else if (i == 7 and (j == 1 or j == 6)) setPiece(board[i][j], "knight", "white", 'K', i, j);
            else if (i == 7 and (j == 2 or j == 5)) setPiece(board[i][j], "bishop", "white", 'B', i, j);
            else if (i == 7 and j == 3) setPiece(board[i][j], "queen", "white", 'Q', i, j);
            else if (i == 7 and j == 4) setPiece(board[i][j], "king", "white", '#', i, j);
            else if (i == 6) setPiece(board[i][j], "pawn", "white", 'P', i, j);

            whitePieces.push_back(board[i][j].piece);
        }
    }
    
    return board;
}

void printBoard(const Board& board) {
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            //syntax: "\033[numero;numero...m" << mensaje << "\033[0m"
           
            /*           foreground background
                black        30         40
                red          31         41
                green        32         42
                yellow       33         43
                blue         34         44
                magenta      35         45
                cyan         36         46
                white        37         47 
            */

            if (board[i][j].piece.team == "white") 
                cout << "\033[37m" << board[i][j].piece.idn << ' ' << "\033[0m"; //printea las piezas en blanco
            else if (board[i][j].piece.team == "black")                          // 31 = rojo; 37 = blanco
                cout << "\033[31m" << board[i][j].piece.idn << ' ' << "\033[0m"; //en rojo
            else cout << board[i][j].piece.idn << ' ';
        }
        cout << endl;
    }
}

void printVector(const vector<Move>& v) {
    int size = v.size();
    for (int i = 0; i < size; i++) {
        cout << v[i].idn << ' ';
    }
    cout << endl;
}

Move search(const vector<Move>& moves, string move) {
    int size = moves.size(), i = 0;
    while (moves[i].idn != move and i < size) 
        i++;
    if (i < size) return moves[i];
    else {
        Move mov;
        mov.type = "null";
        return mov;
    }
}

void checkCastling(Move mover) {
    if (mover.piece.type == "king") {
        if (mover.piece.team == "white") {
            wqCastling = false;
            wkCastling = false;
        } else {
            bqCastling = false;
            bkCastling = false;
        }
    } else if (mover.piece.type == "rook") {
        if (mover.piece.j == 7 and mover.piece.team == "white") wkCastling = false;
        else if (mover.piece.j == 0 and mover.piece.team == "white") wqCastling = false;
        else if (mover.piece.j == 7 and mover.piece.team == "black") bkCastling = false;
        else if (mover.piece.j == 0 and mover.piece.team == "black") bqCastling = false;
    }
}

int main () {
    vector<Piece> whitePieces, blackPieces;
    Board board = setBoard(whitePieces, blackPieces);
    printBoard(board);

    Move previousMove;
    previousMove.piece.type = "empty";

    bool endGame = false;
    int turn = 1;

    vector<Move> legalMoves = calculateLegalMoves(board, previousMove, turn, whitePieces);

    string mov;
    cin >> mov;
    while (!endGame and mov != "exit") {
        Move mover = search(legalMoves, mov);
        if (mover.type != "null") {
            checkCastling(mover);
            if (turn % 2 != 0) {
                move(board, mover, turn, whitePieces);
                legalMoves = calculateLegalMoves(board, previousMove, turn, blackPieces);
            } else {
                move (board, mover, turn, blackPieces);
                legalMoves = calculateLegalMoves(board, previousMove, turn, whitePieces);
            }
            previousMove = mover;
            //cout << (isSquareAttacked(board, board[7][4], board[7][4].piece.team) ? "white king is in shambles" : "nope") << endl;
            //cout << (isSquareAttacked(board, board[0][4], board[0][4].piece.team) ? "black king is in shambles" : "nope") << endl;
            /*for (int i = 0; i < whitePieces.size(); i++) {
                Piece piece = whitePieces[i];
                cout << piece.type << ": " << piece.i << ',' << piece.j << " " << piece.team << "             ";
                piece = blackPieces[i];
                cout << piece.type << ": " << piece.i << ',' << piece.j << " " << piece.team << endl;
            }*/
            printBoard(board);
            printVector(legalMoves);
        } else cout << "esto es embarazoso" << endl;

        if (legalMoves.size() == 0) {
            if (whiteCheck) cout << "Checkmate. Black wins!!!" << endl;
            else if (blackCheck) cout << "Checkmate. White wins!!!" << endl;
            else cout << "Stalemate! Draw :(" << endl;
            endGame = true;
        } else cin >> mov;
    }
}