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
		bool gameOver;

		vector<vector<Piece> > board;
		vector<Piece*> whitePieces;
		vector<Piece*> blackPieces;
		Piece* whiteKing;
		Piece* blackKing;

		map<string, Move> legalMoves;

	public:
		Board();
		Board(const string& FEN);

		void display();
		
		void doMove(string& move);
		bool isMoveLegal(const string& move);

		bool isGameOver();

	private:
		void calculateLegalMoves();
		void calculateLegalMovesBishop(int x, int y);
		void calculateLegalMovesRook(int x, int y);

		bool isSquareAttacked(int x, int y);
		bool isKingInCheck();

		int findPiece(vector<Piece*> v, int rank, int file); 

		string getFEN();
		void setBoard();
};