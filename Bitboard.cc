#include "Bitboard.h" 

Bitboard::Bitboard(bb &b) {
	bitboard = b;
}

void Bitboard::operator =(const bb &b) {
	bitboard = b;
}

void Bitboard::operator |=(const bb &b) {
	bitboard |= b;
}

void Bitboard::operator |(const bb &b) {
	return bitboard | b;
}

void Bitboard::operator &=(const bb &b) {
	bitboard &= b;
}

void Bitboard::operator &(const bb &b) {
	return bitboard & b;
}

bool Bitboard::getBit(int i) {
	return (bb >> (64 - i)) & 1;
}

bool Bitboard::getBit(int rank, int file) {
	// 1 1 1 1 1 1 1 1
	// 1 1 1 1 1 1 1 1
	// 1 1 1 1 1 1 1 1
	// 1 1 1 1 1 1 1 1
	// 1 1 1 1 1 1 1 1
	// 1 1 1 1 1 1 1 1
	// 1 1 1 1 1 1 1 1
	// 1 1 1 1 1 1 1 1
	rank = 7 - rank;
	file = 7 - file;

	return (bb >> (8 * rank + file)) & 1;
}