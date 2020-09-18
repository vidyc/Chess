all: Chess.o Board.o Move.o Piece.o
	g++ Chess.o Board.o Move.o Piece.o -o Chess.exe -Wall -std=c++17

Chess.o: Chess.cc
	g++ -c Chess.cc -Wall -std=c++17

Board.o: Board.cc
	g++ -c Board.cc -Wall -std=c++17

Move.o: Move.cc
	g++ -c Move.cc -Wall -std=c++17

Piece.o: Piece.cc
	g++ -c Piece.cc -Wall -std=c++17

clean: 
	rm *.o Chess.exe

