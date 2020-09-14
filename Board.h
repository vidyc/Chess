#include "Move.h"

class Board {
	private:
		int rows;
		string FEN;
		bool white;
		bool check;
		vector<bool> castlingRights;
		Piece passant;
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

		void calculateLegalMoves(vector<Move> &legalMoves);
	private:
		void calculateLegalMovesBishop(vector<Move> &legalMoves, int x, int y);
		void calculateLegalMovesRook(vector<Move> &legalMoves, int x, int y);

		bool isSquareAttacked(int x, int y);
		bool isKingInCheck();

		string getFEN();
		void setBoard();
};