#include "Piece.h"

class Move {
	private:

		Piece origin, destination;
		bool castling;
		bool capture;
		char promotion; // (N, B, R, Q, -)
		string notation;
	
	public:
		Move(Piece o, Piece d, bool cast, bool capt, char prom);	
		
		void setNotation(const string& not);
		string getNotation();

	private:
		void processNotation();
};