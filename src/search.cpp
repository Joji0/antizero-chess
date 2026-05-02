#include <iostream>
#include "search.h"
#include "evaluate.h"

int negamax(Position& pos, int depth, int alpha, int beta)
{
        if (depth == 0) return evaluate(pos);
        MoveList moves;
        generate_moves(pos, moves);
        if (moves.empty())
        {
                // checkmate
                if (pos.is_in_check()) return -MATE_SCORE + depth;
                return 0; // stalemate
        }
        for (auto& move : moves)
        {
                pos.make_move(move);
                int score = -negamax(pos, depth - 1, alpha, beta);
                pos.unmake_move(move);
                if (score >= beta) return beta;
                if (score > alpha) alpha = score;
        }
        return alpha;
}

Move search(Position& pos, int max_depth)
{
        Move best_move = 0;
        for (int depth = 1; depth <= max_depth; depth++)
        {
                int best_score = -MATE_SCORE - 1;
                MoveList moves;
                generate_moves(pos, moves);
                for (auto& move : moves)
                {
                        pos.make_move(move);
                        int score = -negamax(pos, depth - 1, -MATE_SCORE, MATE_SCORE);
                        pos.unmake_move(move);
                        if (score > best_score)
                        {
                                best_score = score;
                                best_move = move;
                        }
                }
                std::cout << "depth " << depth << " score " << best_score << std::endl;
        }
        return best_move;
}
