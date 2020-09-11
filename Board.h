#include "Move.h"

class Board {
	private:
		const int rows;
		string FEN;
		bool white;

		
	public:
		Board();
		Board(const string& FEN);

		void display();
		void doMove(const string& move);

		vector<Move> calculateLegalMoves();
	private:
		string getFEN();
};