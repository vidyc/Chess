#include <iostream>
#include <vector>
#include <string>
using namespace std;

enum PieceType {
	Pawn = 'p', 
	Knight = 'n',
	Bishop = 'b', 
	Rook = 'r', 
	Queen = 'q',
	King = 'k',
	Empty = '0'
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
		PieceType getType();
		char getIdn();
		char getFile();
		int getFileAsInt();
		int getRank();
};