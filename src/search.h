#ifndef SEARCH_H
#define SEARCH_H
#include "movegen.h"

int negamax(Position& pos, int depth, int alpha, int beta);
Move search(Position& pos, int max_depth);

#endif
