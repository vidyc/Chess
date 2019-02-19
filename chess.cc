#include <iostream>
#include <vector>
using namespace std;

bool wqCastling = true, wkCastling = true, bqCastling = true, bkCastling = true;
bool whiteCheck = false, blackCheck = false;

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

int abs (int n) {
    if (n < 0) return -n;
    return n;
}

void insert(vector<Move>& v1, const vector<Move>& v2) {
    int size = v2.size();
    for (int i = 0; i < size; i++) v1.push_back(v2[i]);
}

Move setMove(Piece piece, Piece pieceCaptured, string type) {
    Move move;
    move.piece = piece;
    move.pieceCaptured = pieceCaptured;
    move.i = pieceCaptured.i;
    move.j = pieceCaptured.j;
    move.type = type;
    
    if (pieceCaptured.type == "king") 
        pieceCaptured.team == "white" ? whiteCheck = true : blackCheck = true;

    if (move.type == "castling") pieceCaptured.j == 7 ? move.idn = "O-O" : move.idn = "O-O-O";
    else {
        move.idn.push_back(piece.idn);
        if (move.type == "capture") move.idn.push_back('x');
        move.idn.push_back(char(move.j + 'a'));
        move.idn.push_back((8 - move.i) + '0');
    }
    return move;
}

bool isSquareAttacked(Board board, const Square &square, string team) {
    int i = square.piece.i, j = square.piece.j, inc = 1;
    bool found = false;
    while (!found and inc < 8) {
        int targ1 = i - inc, targ2 = j;
        if (targ1 >= 0 and board[targ1][targ2].piece.type != "empty") {
            Piece piece = board[targ1][targ2].piece;
            if (piece.team != team and (piece.type == "rook" or piece.type == "queen")) found = true;
            else for (int e = targ1 - 1; e >= 0; e++) board[e][targ2].piece = piece;
        }
        
        targ1 = i + inc;
        if (targ1 < 8 and board[targ1][targ2].piece.type != "empty") {
            Piece piece = board[targ1][targ2].piece;
            if (piece.team != team and (piece.type == "rook" or piece.type == "queen")) found = true;
            else for (int e = targ1 + 1; e < 8; e++) board[e][targ2] = piece;
        }

        targ1 = j - inc, targ2 = i;
        if (targ1 >= 0 and board[targ2][targ1].piece.type != "empty") {
            Piece piece = board[targ2][targ1].piece;
            if (piece.team != team and (piece.type == "rook" or piece.type == "queen")) found = true;
            else for (int e = targ1 - 1; e >= 0; e++) board[targ2][e] = piece;
        }

        targ1 = j + inc;
        if (targ1 < 8 and board[targ2][targ1].piece.type != "empty") {
            Piece piece = board[targ2][targ1].piece;
            if (piece.team != team and (piece.type == "rook" or piece.type == "queen")) found = true;
            else for (int e = targ1 + 1; e < 8; e++) board[targ2][e] = piece;
        }

        targ1 = i - inc, targ2 = j - inc;
        if (targ1 >= 0 and targ2 >= 0 and board[targ1][targ2].piece.type != "empty") {
            Piece piece = board[targ1][targ2].piece;
            if (piece.team != team and (piece.type == "bishop" or piece.type == "queen")) found = true;
        }

        ++inc;
    }
    return found;
}

vector<Move> calculateLegalMoves(const Board& board, Piece piece, Move previousMove) {
    vector<Move> legalMoves;
    int i = piece.i, j = piece.j;
    //cout << piece.type << ' ' << piece.idn << char(j + 'a') << 8 - i << endl;
    if (piece.type == "rook") {
        int inc = 1;
        bool endU = false, endD = false, endL = false, endR = false;
        while ((!endU or !endD or !endL or !endR) and inc < 8) {
            
            if (i - inc >= 0 and board[i - inc][j].piece.type == "empty")
                legalMoves.push_back(setMove(piece, board[i - inc][j].piece, "normal"));
            else if (!endU) {
                if (i - inc >= 0 and board[i - inc][j].piece.team != piece.team) {
                    legalMoves.push_back(setMove(piece, board[i - inc][j].piece, "capture"));
                }
                endU = true;
            }
            if (targ1 < 8 and board[targ1][j].piece.type == "empty") 
                legalMoves.push_back(setMove(piece, board[targ1][j].piece, "normal"));
            else if (!endD) {
                if (targ1 < 8 and board[targ1][j].piece.team != piece.team) {
                    legalMoves.push_back(setMove(piece, board[targ1][j].piece, "capture"));
                }
                endD = true;
            }
            if (j + inc < 8 and board[i][j + inc].piece.type == "empty")
                legalMoves.push_back(setMove(piece, board[i][j + inc].piece, "normal"));
            else if (!endR) {
                if (j + inc < 8 and board[i][j + inc].piece.team != piece.team) {
                    legalMoves.push_back(setMove(piece, board[i][j + inc].piece, "capture"));
                }
                endR = true;
            }
            if (targ1 >= 0 and board[i][j - inc].piece.type == "empty")
                legalMoves.push_back(setMove(piece, board[i][j - inc].piece, "normal"));
            else if (!endL) {
                if (targ1 >= 0 and board[i][j - inc].piece.team != piece.team) {
                    legalMoves.push_back(setMove(piece, board[i][j - inc].piece, "capture"));
                }
                endL = true;
            }
            inc++;
        }
    }
    else if (piece.type == "bishop") {
        int inc = 1;
        bool endUL = false, endUR = false, endDL = false, endDR = false;
        while ((!endUL or !endUR or !endDL or !endDR) and inc < 8) {
            if (i - inc >= 0 and targ1 >= 0 and board[i - inc][j - inc].piece.type == "empty")
                legalMoves.push_back(setMove(piece, board[i - inc][j - inc].piece, "normal"));
            else if (!endUL) {
                if (i - inc >= 0 and targ1 >= 0 and board[i - inc][j - inc].piece.team != piece.team) {
                    legalMoves.push_back(setMove(piece, board[i - inc][j - inc].piece, "capture"));
                }
                endUL = true;
            }
            
            if (targ1 < 8 and j + inc < 8 and board[targ1][j + inc].piece.type == "empty") 
                legalMoves.push_back(setMove(piece, board[targ1][j + inc].piece, "normal"));
            else if (!endDR) {
                if (targ1 < 8 and j + inc < 8 and board[targ1][j + inc].piece.team != piece.team) {
                    legalMoves.push_back(setMove(piece, board[targ1][j + inc].piece, "capture"));
                }
                endDR = true;
            }
                
            if (i - inc >= 0 and j + inc < 8 and board[i - inc][j + inc].piece.type == "empty")
                legalMoves.push_back(setMove(piece, board[i - inc][j + inc].piece, "normal"));
            else if (!endUR) {
                if (i - inc >= 0 and j + inc < 8 and board[i - inc][j + inc].piece.team != piece.team) {
                    legalMoves.push_back(setMove(piece, board[i - inc][j + inc].piece, "capture"));
                }
                endUR = true;
            }
                
            if (targ1 < 8 and targ1 >= 0 and board[targ1][j - inc].piece.type == "empty")
                legalMoves.push_back(setMove(piece, board[targ1][j - inc].piece, "normal"));
            else if (!endDL) {
                if (targ1 < 8 and targ1 >= 0 and board[targ1][j - inc].piece.team != piece.team) {
                    legalMoves.push_back(setMove(piece, board[targ1][j - inc].piece, "capture"));
                }
                endDL = true;
            }
            inc++;
        }
        
    }
    else if (piece.type == "queen") {
        int inc = 1;
        bool endU = false, endD = false, endL = false, endR = false, endUL = false, endUR = false, endDL = false, endDR = false;
        while ((!endU or !endD or !endL or !endR or !endUL or !endUR or !endDL or !endDR) and inc < 8) {
            if (i - inc >= 0 and board[i - inc][j].piece.type == "empty")
                legalMoves.push_back(setMove(piece, board[i - inc][j].piece, "normal"));
            else if (!endU) {
                if (i - inc >= 0 and board[i - inc][j].piece.team != piece.team) {
                    legalMoves.push_back(setMove(piece, board[i - inc][j].piece, "capture"));
                }
                endU = true;
            }
            
            if (targ1 < 8 and board[targ1][j].piece.type == "empty") 
                legalMoves.push_back(setMove(piece, board[targ1][j].piece, "normal"));
            else if (!endD) {
                if (targ1 < 8 and board[targ1][j].piece.team != piece.team) {
                    legalMoves.push_back(setMove(piece, board[targ1][j].piece, "capture"));
                }
                endD = true;
            }
                
            if (j + inc < 8 and board[i][j + inc].piece.type == "empty")
                legalMoves.push_back(setMove(piece, board[i][j + inc].piece, "normal"));
            else if (!endR) {
                if (j + inc < 8 and board[i][j + inc].piece.team != piece.team) {
                    legalMoves.push_back(setMove(piece, board[i][j + inc].piece, "capture"));
                }
                endR = true;
            }
                
            if (targ1 >= 0 and board[i][j - inc].piece.type == "empty")
                legalMoves.push_back(setMove(piece, board[i][j - inc].piece, "normal"));
            else if (!endL) {
                if (targ1 >= 0 and board[i][j - inc].piece.team != piece.team) {
                    legalMoves.push_back(setMove(piece, board[i][j - inc].piece, "capture"));
                }
                endL = true;
            }
            
            if (i - inc >= 0 and targ1 >= 0 and board[i - inc][j - inc].piece.type == "empty")
                legalMoves.push_back(setMove(piece, board[i - inc][j - inc].piece, "normal"));
            else if (!endUL) {
                if (i - inc >= 0 and targ1 >= 0 and board[i - inc][j - inc].piece.team != piece.team) {
                    legalMoves.push_back(setMove(piece, board[i - inc][j - inc].piece, "capture"));
                }
                endUL = true;
            }
            
            if (targ1 < 8 and j + inc < 8 and board[targ1][j + inc].piece.type == "empty") 
                legalMoves.push_back(setMove(piece, board[targ1][j + inc].piece, "normal"));
            else if (!endDR) {
                if (targ1 < 8 and j + inc < 8 and board[targ1][j + inc].piece.team != piece.team) {
                    legalMoves.push_back(setMove(piece, board[targ1][j + inc].piece, "capture"));
                }
                endDR = true;
            }
                
            if (i - inc >= 0 and j + inc < 8 and board[i - inc][j + inc].piece.type == "empty")
                legalMoves.push_back(setMove(piece, board[i - inc][j + inc].piece, "normal"));
            else if (!endUR) {
                if (i - inc >= 0 and j + inc < 8 and board[i - inc][j + inc].piece.team != piece.team) {
                    legalMoves.push_back(setMove(piece, board[i - inc][j + inc].piece, "capture"));
                }
                endUR = true;
            }
                
            if (targ1 < 8 and targ1 >= 0 and board[targ1][j - inc].piece.type == "empty")
                legalMoves.push_back(setMove(piece, board[targ1][j - inc].piece, "normal"));
            else if (!endDL) {
                if (targ1 < 8 and targ1 >= 0 and board[targ1][j - inc].piece.team != piece.team) {
                    legalMoves.push_back(setMove(piece, board[targ1][j - inc].piece, "capture"));
                }
                endDL = true;
            }
            inc++;
        }
    }    
    else if (piece.type == "king") {
        for (int incI = -1; incI < 2; incI++)
            for (int incJ = -1; incJ < 2; incJ++) {
                if (targ1I >= 0 and targ1I < 8 and j + incJ >= 0 and j + incJ < 8) {
                    Piece possPiece = board[targ1I][j + incJ].piece;
                    if (possPiece.type == "empty") 
                        legalMoves.push_back(setMove(piece, possPiece, "normal"));
                    else if (possPiece.team != piece.team)
                        legalMoves.push_back(setMove(piece, possPiece, "capture"));
                }
            }
        
        //enroque
        if (piece.team == "white") { 
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
        }
    }
    else if (piece.type == "pawn") {
        //movimiento normal y captura
        int inc = -1;
        if (piece.team == "black") inc = 1;
        
        if (board[targ1][j].piece.type == "empty") legalMoves.push_back(setMove(piece, board[targ1][j].piece, "normal"));
        
        for (int incJ = -1; incJ < 2; incJ += 2) {
            if (j + incJ >= 0 and j + incJ < 8) {
                Piece pieceToCapture = board[targ1][j + incJ].piece;
                if (pieceToCapture.type != "empty" and pieceToCapture.team != piece.team) 
                    legalMoves.push_back(setMove(piece, pieceToCapture, "capture")); 
            }
        }
        
        //doble avance
        if ((piece.team == "white" and i == 6) or (piece.team == "black" and i == 1)) 
            if (board[i + 2 * inc][j].piece.type == "empty" and board[targ1][j].piece.type == "empty") 
                legalMoves.push_back(setMove(piece, board[i + 2 * inc][j].piece, "normal"));
            
        //en passant
        if (previousMove.piece.type == "pawn" and abs(previousMove.piece.i - previousMove.i) == 2 and (previousMove.j - 1 == j or previousMove.j + 1 == j) and i == previousMove.i)
            legalMoves.push_back(setMove(piece, board[previousMove.i][previousMove.j].piece, "passant"));
    }
    else if (piece.type == "knight") {
        
        for (int incI = -1; incI < 2; incI += 2) {
            for (int incJ = -2; incJ < 3; incJ += 4) {
                if (targ1I >= 0 and targ1I < 8 and j + incJ >= 0 and j + incJ < 8) {
                    Piece pieceToCapture = board[targ1I][j + incJ].piece;
                    if (pieceToCapture.type == "empty") legalMoves.push_back(setMove(piece, pieceToCapture, "normal"));
                    else if (piece.team != pieceToCapture.team) 
                        legalMoves.push_back(setMove(piece, pieceToCapture, "capture"));
                }
            }
        }
        
        for (int incI = -2; incI < 3; incI += 4) {
            for (int incJ = -1; incJ < 2; incJ += 2) {
                if (targ1I >= 0 and targ1I < 8 and j + incJ >= 0 and j + incJ < 8) {
                    Piece pieceToCapture = board[targ1I][j + incJ].piece;
                    if (pieceToCapture.type == "empty") legalMoves.push_back(setMove(piece, pieceToCapture, "normal"));
                    else if (piece.team != pieceToCapture.team) 
                        legalMoves.push_back(setMove(piece, pieceToCapture, "capture"));
                }
            }
        }
    }
    return legalMoves;
}

void calculateLegalMoves(vector<Move> &whitelegalMoves, vector<Move> $blacklegalMoves, const Board& board, Move previousMove) {
    
    whitelegalMoves = vector<Move>;
    blacklegalMoves = vector<Move>;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            Piece piece = board[i][j].piece;
            if (piece.team == "white") 
                insert(whitelegalMoves, calculateLegalMoves(board, piece, previousMove));
            else if (piece.team == "black") 
                insert(blacklegalMoves, calculateLegalMoves(board, piece, previousMove));
        }
    }
}

void setPiece(Square& square, string type, string team, char idn, int i, int j) {
    square.piece.type = type;
    square.piece.team = team;
    square.piece.idn = idn;
    square.piece.i = i;
    square.piece.j = j;
} 

Board setBoard() {
    Board board(8, Row(8));
     for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 8; j++) {
            if (i == 0 and (j == 0 or j == 7)) setPiece(board[i][j], "rook", "black", 'R', i, j);
            else if (i == 0 and (j == 1 or j == 6)) setPiece(board[i][j], "knight", "black", 'K', i, j);
            else if (i == 0 and (j == 2 or j == 5)) setPiece(board[i][j], "bishop", "black", 'B', i, j);
            else if (i == 0 and j == 3) setPiece(board[i][j], "queen", "black", 'Q', i, j);
            else if (i == 0 and j == 4) setPiece(board[i][j], "king", "black", '#', i, j);
            else if (i == 1) setPiece(board[i][j], "pawn", "black", 'P', i, j);
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
        cout << v[i].idn << '   ';
    }
}

void move(Board& board, Move m, bool& whiteTurn) {
    if (m.type == "castling") {
        if (m.j == 7) {
            setPiece(board[m.piece.i][6], m.piece.type, m.piece.team, m.piece.idn, m.piece.i, 6);
            setPiece(board[m.piece.i][5], m.pieceCaptured.type, m.pieceCaptured.team, m.pieceCaptured.idn, m.piece.i, 5);
            setPiece(board[m.piece.i][7], "empty", " ", 'o', m.piece.i, 7);
        } else {
            setPiece(board[m.piece.i][2], m.piece.type, m.piece.team, m.piece.idn, m.piece.i, 2);
            setPiece(board[m.piece.i][3], m.pieceCaptured.type, m.pieceCaptured.team, m.pieceCaptured.idn, m.piece.i, 3);
            setPiece(board[m.piece.i][0], "empty", " ", 'o', m.piece.i, 0);
        }
        setPiece(board[m.piece.i][m.piece.j], "empty", " ", 'o', m.piece.i, m.piece.j);
    } else {
        int i = m.i, j = m.j;
        setPiece(board[i][j], m.piece.type, m.piece.team, m.piece.idn, i, j);
        setPiece(board[m.piece.i][m.piece.j], "empty", " ", 'o', m.piece.i, m.piece.j); 
    }
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
    Board board = setBoard();
    printBoard(board);

    Move previousMove;
    previousMove.piece.type = "empty";

    bool whiteTurn = true;

    vector<Move> whitelegalMoves, blacklegalMoves; 
    calculateLegalMoves(whitelegalMoves, blacklegalMoves, board, previousMove);

    string mov;
    while (cin >> mov) {
        Move mover = search(legalMoves, mov);
        if (mover.type != "null") {
            move(board, mover, whiteTurn);
            checkCastling(mover);
            previousMove = mover;
            calculateLegalMoves(whitelegalMoves, blacklegalMoves, board, previousMove);
            whiteTurn = !whiteTurn;
            printBoard(board);
           // printVector(legalMoves);
        } else cout << "esto es embarazoso" << endl;
    }
}
