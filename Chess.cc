#include "Chess.h"

int main() {
	string f = "rNbqkb1r/pppp1ppp/8/8/4P3/5Q2/PPPP1PP1/RNB1KB1R w KQkq - 0 6";
	Board b();

	while (!b.isGameOver()) {
		b.display();
		string move;
		do { 
			cin >> move;
		} while (!b.isMoveLegal(move));
		b.doMove(move);
	}
}
