typedef unsigned long long int bb;

class Bitboard {
	private:
		bb bitboard;
	public:
		Bitboard(bb &b);

		void operator =(const bb &b);
		void operator |=(const bb &b);
		bb operator |(const bb &b);
		void operator &=(const bb &b);
		bb operator &(const bb &b);

		bool getBit(int i);
		bool getBit(int rank, int file);

		bb getBitboard();
	private:
};