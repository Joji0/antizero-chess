#ifndef MAGICS_H
#define MAGICS_H
#include "bitboard.h"

// Magic number for rook and bishop
extern Bitboard RookMagics[64];
extern Bitboard BishopMagics[64];

// Mask for relevant square
extern Bitboard RookMasks[64];
extern Bitboard BishopMasks[64];

// Shift amount for rook and bishop
extern int RookShifts[64];
extern int BishopShifts[64];

// Magic lookup table
extern Bitboard RookAttacks[64][4096];
extern Bitboard BishopAttacks[64][512];

// Attacks function
inline Bitboard rook_attacks(Square sq, Bitboard occ)
{
        occ &= RookMasks[sq];
        occ *= RookMagics[sq];
        occ >>= RookShifts[sq];
        return RookAttacks[sq][occ];
}

inline Bitboard bishop_attacks(Square sq, Bitboard occ)
{
        occ &= BishopMasks[sq];
        occ *= BishopMagics[sq];
        occ >>= BishopShifts[sq];
        return BishopAttacks[sq][occ];
}

inline Bitboard queen_attacks(Square sq, Bitboard occ) { return rook_attacks(sq, occ) | bishop_attacks(sq, occ); }

// Initialize all table
void init_magics();

#endif
