#include "Piece.h"

class Move {
	private:

		Piece origin, destination;
		bool castling;
		bool capture;
		bool passant;
		bool promotion; // (N, B, R, Q, -)
		string notation;
	
	public:
		Move();
		Move(Piece o, Piece d, bool cast, bool capt, bool pass, bool prom);	
		
		void setNotation(const string& n);
		string getNotation();

		Piece getOrigin();
		Piece getDestination();
		bool isCapture();
		bool allowsPassant();
		bool isPromotion();
	private:
		void processNotation();
};