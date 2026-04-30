#ifndef MOVEGEN_H
#define MOVEGEN_H
#include "position.h"
#include "magics.h"

class MoveList
{
        private:
                int count = 0;
                Move moves[256];
        public:
                void add(Move move) { moves[count++] = move; }
                Move operator[](int i) const { return moves[i]; }
                int size() const { return count; }
                bool empty() const { return count == 0; }
                Move* begin() { return moves; }
                Move* end() { return moves + count; }
};

void generate_moves(Position& pos, MoveList& moves);

#endif
