#include "Move.h"

class Board {
	private:
		int rows;
		string FEN;
		bool white;
		bool check;
		vector<bool> castlingRights;
		string passant;
		int halfmoves;
		int turn;

		vector<vector<Piece> > board;
		vector<Piece*> whitePieces;
		vector<Piece*> blackPieces;
		Piece* whiteKing;
		Piece* blackKing;

	public:
		Board();
		Board(const string& FEN);

		void display();
		void doMove(const string& move);

		vector<Move> calculateLegalMoves();
	private:
		bool isSquareAttacked();
		bool isKingInCheck();
		string getFEN();
		void setBoard();
};