#include "evaluate.h"


// piece values and PST are taken from https://www.chessprogramming.org/Simplified_Evaluation_Function
const int PieceValue[6] = {100, 320, 330, 500, 900, 20000};
const int PawnPST[64] =
{
         0,  0,  0,  0,  0,  0,  0,  0,
        50, 50, 50, 50, 50, 50, 50, 50,
        10, 10, 20, 30, 30, 20, 10, 10,
         5,  5, 10, 25, 25, 10,  5,  5,
         0,  0,  0, 20, 20,  0,  0,  0,
         5, -5,-10,  0,  0,-10, -5,  5,
         5, 10, 10,-20,-20, 10, 10,  5,
         0,  0,  0,  0,  0,  0,  0,  0
};
const int KnightPST[64] =
{
        -50,-40,-30,-30,-30,-30,-40,-50,
        -40,-20,  0,  0,  0,  0,-20,-40,
        -30,  0, 10, 15, 15, 10,  0,-30,
        -30,  5, 15, 20, 20, 15,  5,-30,
        -30,  0, 15, 20, 20, 15,  0,-30,
        -30,  5, 10, 15, 15, 10,  5,-30,
        -40,-20,  0,  5,  5,  0,-20,-40,
        -50,-40,-30,-30,-30,-30,-40,-50,
};
const int BishopPST[64] =
{
        -20,-10,-10,-10,-10,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5, 10, 10,  5,  0,-10,
        -10,  5,  5, 10, 10,  5,  5,-10,
        -10,  0, 10, 10, 10, 10,  0,-10,
        -10, 10, 10, 10, 10, 10, 10,-10,
        -10,  5,  0,  0,  0,  0,  5,-10,
        -20,-10,-10,-10,-10,-10,-10,-20,
};
const int RookPST[64] =
{
         0,  0,  0,  0,  0,  0,  0,  0,
         5, 10, 10, 10, 10, 10, 10,  5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
        -5,  0,  0,  0,  0,  0,  0, -5,
         0,  0,  0,  5,  5,  0,  0,  0
};
const int QueenPST[64] =
{
        -20,-10,-10, -5, -5,-10,-10,-20,
        -10,  0,  0,  0,  0,  0,  0,-10,
        -10,  0,  5,  5,  5,  5,  0,-10,
         -5,  0,  5,  5,  5,  5,  0, -5,
          0,  0,  5,  5,  5,  5,  0, -5,
        -10,  5,  5,  5,  5,  5,  0,-10,
        -10,  0,  5,  0,  0,  0,  0,-10,
        -20,-10,-10, -5, -5,-10,-10,-20
};
const int KingMidPST[64] =
{
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -30,-40,-40,-50,-50,-40,-40,-30,
        -20,-30,-30,-40,-40,-30,-30,-20,
        -10,-20,-20,-20,-20,-20,-20,-10,
         20, 20,  0,  0,  0,  0, 20, 20,
         20, 30, 10,  0,  0, 10, 30, 20
};
// ignore this right now. Will be used in the future when the evaluation is upgraded
// const int KingEndPST[64] =
// {
//         -50,-40,-30,-20,-20,-30,-40,-50,
//         -30,-20,-10,  0,  0,-10,-20,-30,
//         -30,-10, 20, 30, 30, 20,-10,-30,
//         -30,-10, 30, 40, 40, 30,-10,-30,
//         -30,-10, 30, 40, 40, 30,-10,-30,
//         -30,-10, 20, 30, 30, 20,-10,-30,
//         -30,-30,  0,  0,  0,  0,-30,-30,
//         -50,-30,-30,-30,-30,-30,-30,-50
// };
const int* PST[6] = { PawnPST, KnightPST, BishopPST, RookPST, QueenPST, KingMidPST };

// evaluate this position for current side to move color
int evaluate(Position &pos)
{
        int score = 0;
        for (int piece = PAWN; piece <= KING; piece++)
        {
                Bitboard white_bb = pos.get_piece_bb(WHITE, static_cast<PieceType>(piece));
                while (white_bb)
                {
                        Square sq = static_cast<Square>(pop_lsb(white_bb));
                        score += PieceValue[piece] + PST[piece][sq ^ 56];
                }
                Bitboard black_bb = pos.get_piece_bb(BLACK, static_cast<PieceType>(piece));
                while (black_bb)
                {
                        Square sq = static_cast<Square>(pop_lsb(black_bb));
                        score -= PieceValue[piece] + PST[piece][sq];
                }
        }
        return (pos.get_side_to_move() == WHITE ? score : -score);
}
