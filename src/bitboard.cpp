#include <iostream>
#include "bitboard.h"

Bitboard KnightAttacks[64];
Bitboard KingAttacks[64];
Bitboard PawnAttacks[2][64];

Bitboard init_knight(Square s)
{
        Bitboard attacks = 0;
        // TODO: implement bitmasking for attacks
        return attacks;
}
Bitboard init_king(Square s)
{
        Bitboard attacks = 0;
        // TODO: implement bitmasking for attacks
        return attacks;
}
Bitboard init_pawn(Square s, Color c)
{
        Bitboard attacks = 0;
        // TODO: implement bitmasking for attacks
        return attacks;
}
void init_bitboards()
{
        for(int i = 0; i < 64; i++)
        {
                KnightAttacks[i] = init_knight((Square)i);
                KingAttacks[i] = init_king((Square)i);
                PawnAttacks[0][i] = init_pawn((Square)i, (Color)0);
                PawnAttacks[1][i] = init_pawn((Square)i, (Color)1);
        }
}
void print_bitboard(Bitboard bb)
{
        for(int i = 63; i >= 0; i--)
        {

        }
}
