#include "Chess.h"

int main() {
	cout << "HELLO" << endl;
	Board b ("rNbqkb1r/pppp1ppp/8/8/4P3/5Q2/PPPP1PP1/RNB1KB1R w KQkq - 0 6");
	b.display();
	vector<Move> legalMoves;
	b.calculateLegalMoves(legalMoves);
	for (int i = 0; i < legalMoves.size(); i++) {
		cout << legalMoves[i].getNotation() << ' ';
	}
	cout << endl;
}
