#include <iostream>
#include "bitboard.h"

Bitboard KnightAttacks[64];
Bitboard KingAttacks[64];
Bitboard PawnAttacks[2][64];

Bitboard init_knight(Square s)
{
        Bitboard attacks = 0;
        Bitboard bb = 1ULL << s;
        // up 2, right 1
        attacks |= (bb << 17) & ~FileABB;
        // up 2, left 1
        attacks |= (bb << 15) & ~FileHBB;
        // up 1, right 2
        attacks |= (bb << 10) & ~FileABB & ~FileBBB;
        // up 1, left 2
        attacks |= (bb << 6) & ~FileHBB & ~FileGBB;
        // down 1, right 2
        attacks |= (bb >> 6) & ~FileABB & ~FileBBB;
        // down 1, left 2
        attacks |= (bb >> 10) & ~FileHBB & ~FileGBB;
        // down 2, right 1
        attacks |= (bb >> 15) & ~FileABB;
        // down 2, left 1
        attacks |= (bb >> 17) & ~FileHBB;
        return attacks;
}
Bitboard init_king(Square s)
{
        Bitboard attacks = 0;
        Bitboard bb = 1ULL << s;
        // up 1, right 1
        attacks |= (bb << 9) & ~FileABB;
        // up 1
        attacks |= (bb << 8);
        // up 1, left 1
        attacks |= (bb << 7) & ~FileHBB;
        // right 1
        attacks |= (bb << 1) & ~FileABB;
        // left 1
        attacks |= (bb >> 1) & ~FileHBB;
        // down 1, right 1
        attacks |= (bb >> 7) & ~FileABB;
        // down
        attacks |= (bb >> 8);
        // down 1, left 1
        attacks |= (bb >> 9) & ~FileHBB;
        return attacks;
}
Bitboard init_pawn(Square s, Color c)
{
        Bitboard attacks = 0;
        Bitboard bb = 1ULL << s;
        if (c == WHITE)
        {
                // right capture
                attacks |= (bb << 9) & ~FileABB;
                // left capture
                attacks |= (bb << 7) & ~FileHBB;
        }
        else
        {
                // right capture (from black POV)
                attacks |= (bb >> 9) & ~FileHBB;
                // left capture
                attacks |= (bb >> 7) & ~FileABB;
        }
        return attacks;
}
void init_bitboards()
{
        for (int i = 0; i < 64; i++)
        {
                KnightAttacks[i] = init_knight((Square)i);
                KingAttacks[i] = init_king((Square)i);
                PawnAttacks[WHITE][i] = init_pawn((Square)i, WHITE);
                PawnAttacks[BLACK][i] = init_pawn((Square)i, BLACK);
        }
}
void print_bitboard(Bitboard bb)
{
        for (int rank = 7; rank >= 0; rank--)
        {
                for (int file = 0; file < 8; file++)
                {
                        int sq = rank * 8 + file;
                        if (bb & (1ULL << sq)) std::cout << "X ";
                        else std::cout << ". ";
                        if (file == 7) std::cout << "\n";
                }
        }
}
