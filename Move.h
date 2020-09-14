#include "Piece.h"

class Move {
	private:

		Piece origin, destination;
		bool castling;
		bool capture;
		bool passant;
		char promotion; // (N, B, R, Q, -)
		string notation;
	
	public:
		Move(Piece o, Piece d, bool cast, bool capt, bool pass, char prom);	
		
		void setNotation(const string& n);
		string getNotation();

	private:
		void processNotation();
};