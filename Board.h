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
	private:
		string getFEN();
};