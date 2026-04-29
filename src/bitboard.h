#ifndef BITBOARD_H
#define BITBOARD_H
#include <cstdint>
#include <cassert>
#include "types.h"

// Constants
// LSB at A1 and MSB at H8
using Bitboard = uint64_t;
constexpr Bitboard FileABB = 0x0101010101010101ULL;
constexpr Bitboard FileBBB = FileABB << 1;
constexpr Bitboard FileCBB = FileABB << 2;
constexpr Bitboard FileDBB = FileABB << 3;
constexpr Bitboard FileEBB = FileABB << 4;
constexpr Bitboard FileFBB = FileABB << 5;
constexpr Bitboard FileGBB = FileABB << 6;
constexpr Bitboard FileHBB = FileABB << 7;
constexpr Bitboard Rank1BB = 0xFFULL;
constexpr Bitboard Rank2BB = Rank1BB << 8;
constexpr Bitboard Rank3BB = Rank1BB << 16;
constexpr Bitboard Rank4BB = Rank1BB << 24;
constexpr Bitboard Rank5BB = Rank1BB << 32;
constexpr Bitboard Rank6BB = Rank1BB << 40;
constexpr Bitboard Rank7BB = Rank1BB << 48;
constexpr Bitboard Rank8BB = Rank1BB << 56;

// Bit functions
inline int popcount(Bitboard bb) { return __builtin_popcountll(bb); }
inline int lsb(Bitboard bb)
{
        assert(bb != 0);
        return __builtin_ctzll(bb);
}
inline int msb(Bitboard bb)
{
        assert(bb != 0);
        return 63 - __builtin_clzll(bb);
}
inline int pop_lsb(Bitboard& bb)
{
        int square = lsb(bb);
        bb &= (bb - 1);
        return square;
}

// Attack table for non-sliding pieces and print debugger
extern Bitboard KnightAttacks[64];
extern Bitboard KingAttacks[64];
extern Bitboard PawnAttacks[NUM_COLORS][64];

void init_bitboards();
void print_bitboard(Bitboard bb);
#endif
