#include <iostream>
#include <vector>
#include <string>
#include <map>
using namespace std;

enum PieceType {
	Pawn = 'p', 
	Knight = 'n',
	Bishop = 'b', 
	Rook = 'r', 
	Queen = 'q',
	King = 'k',
	Empty = '.'
};

class Piece {
	private:
		bool empty;
		bool white;
		PieceType type;
		char idn;
		int rank;
		char file;

	public:
		Piece();
		Piece(bool e, bool w, char i, int r, int f);
		bool isEmpty();
		bool isWhite();
		void printType();
		void printPos();
		void printStatus();
		PieceType getType();
		char getIdn();
		char getFile();
		int getFileAsInt();
		int getRank();

		void setType(PieceType t);
		void copyPiece(Piece& p);
};