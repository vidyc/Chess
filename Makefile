all: chess.o
	g++ Chess.o -o Chess.x -Wall -std=c++11

chess.o: chess.cc
	g++ -c Chess.cc -Wall -std=c++11

clean: 
	rm *.o Chess.x

