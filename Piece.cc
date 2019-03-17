#include <iostream>

struct Pos {
    int x, y;
    Pos(int x, int y) : x(x), y(y) {}
};

class Piece {
    public: 
        Piece();
        virtual void calculateLegalMoves() = 0;  
        void display();
        std::string getType();
    private:
        char idn;
        std::string type;
        Pos position;
};

class Pawn : public Piece {
    
};

int main () {}  