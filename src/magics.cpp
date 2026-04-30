#include "magics.h"
#include <random>
#include <cstring>
#include <cstdint>
#include <limits>

Bitboard RookMagics[64];
Bitboard BishopMagics[64];

Bitboard RookMasks[64];
Bitboard BishopMasks[64];

int RookShifts[64];
int BishopShifts[64];

Bitboard RookAttacks[64][4096];
Bitboard BishopAttacks[64][512];

// rook mask attack pattern (+ sign)
Bitboard rook_mask(Square sq)
{
        Bitboard mask = 0;
        Rank rank = rank_of(sq);
        File file = file_of(sq);
        for (int i = rank + 1; i < 7; i++) mask |= 1ULL << (i * 8 + file);
        for (int i = rank - 1; i > 0; i--) mask |= 1ULL << (i * 8 + file);
        for (int i = file + 1; i < 7; i++) mask |= 1ULL << (rank * 8 + i);
        for (int i = file - 1; i > 0; i--) mask |= 1ULL << (rank * 8 + i);
        return mask;
}

// bishop mask attack pattern (x sign)
Bitboard bishop_mask(Square sq)
{
        Bitboard mask = 0;
        Rank rank = rank_of(sq);
        File file = file_of(sq);
        for (int i = rank + 1, j = file + 1; i < 7 && j < 7; i++, j++) mask |= 1ULL << (i * 8 + j);
        for (int i = rank - 1, j = file + 1; i > 0 && j < 7; i--, j++) mask |= 1ULL << (i * 8 + j);
        for (int i = rank + 1, j = file - 1; i < 7 && j > 0; i++, j--) mask |= 1ULL << (i * 8 + j);
        for (int i = rank - 1, j = file - 1; i > 0 && j > 0; i--, j--) mask |= 1ULL << (i * 8 + j);
        return mask;
}

// simulate rook attacks on a particular square and occupied position
Bitboard rook_attacks_slow(Square sq, Bitboard occ)
{
        Bitboard mask = 0;
        Rank rank = rank_of(sq);
        File file = file_of(sq);
        for (int i = rank + 1; i < 8; i++)
        {
                Bitboard n_mask = 1ULL << (i * 8 + file);
                mask |= n_mask;
                if (occ & n_mask) break;
        }
        for (int i = rank - 1; i >= 0; i--)
        {
                Bitboard n_mask = 1ULL << (i * 8 + file);
                mask |= n_mask;
                if (occ & n_mask) break;
        }
        for (int i = file + 1; i < 8; i++)
        {
                Bitboard n_mask = 1ULL << (rank * 8 + i);
                mask |= n_mask;
                if (occ & n_mask) break;
        }
        for (int i = file - 1; i >= 0; i--)
        {
                Bitboard n_mask = 1ULL << (rank * 8 + i);
                mask |= n_mask;
                if (occ & n_mask) break;
        }
        return mask;
}

// simulate bishop attacks on a particular square and occupied position
Bitboard bishop_attacks_slow(Square sq, Bitboard occ)
{
        Bitboard mask = 0;
        Rank rank = rank_of(sq);
        File file = file_of(sq);
        for (int i = rank + 1, j = file + 1; i < 8 && j < 8; i++, j++)
        {
                Bitboard n_mask = 1ULL << (i * 8 + j);
                mask |= n_mask;
                if (occ & n_mask) break;
        }
        for (int i = rank - 1, j = file + 1; i >= 0 && j < 8; i--, j++)
        {
                Bitboard n_mask = 1ULL << (i * 8 + j);
                mask |= n_mask;
                if (occ & n_mask) break;
        }
        for (int i = rank + 1, j = file - 1; i < 8 && j >= 0; i++, j--)
        {
                Bitboard n_mask = 1ULL << (i * 8 + j);
                mask |= n_mask;
                if (occ & n_mask) break;
        }
        for (int i = rank - 1, j = file - 1; i >= 0 && j >= 0; i--, j--)
        {
                Bitboard n_mask = 1ULL << (i * 8 + j);
                mask |= n_mask;
                if (occ & n_mask) break;
        }
        return mask;
}

static std::mt19937_64 gen(std::random_device{}());

// random uint64_t generator
uint64_t random_u64()
{
        std::uniform_int_distribution<uint64_t> distr(0ULL, std::numeric_limits<uint64_t>::max());
        return distr(gen);
}

uint64_t random_sparse() { return random_u64() & random_u64() & random_u64(); }

// find magic number for each square
Bitboard find_magic(Square sq, bool is_rook)
{
        if (is_rook)
        {
                Bitboard mask = RookMasks[sq];
                int set_bit = popcount(mask);
                int shift = 64 - set_bit;
                RookShifts[sq] = shift;
                Bitboard attacks[1 << set_bit], occupancies[1 << set_bit];
                Bitboard subset = 0;
                int count = 0;
                // Carry-Rippler algorithm to generate 2^N subset of a set
                do
                {
                        attacks[count] = rook_attacks_slow(sq, subset);
                        occupancies[count] = subset;
                        count++;
                        subset = (subset - mask) & mask;
                } while (subset);
                while (true)
                {
                        uint64_t candidate = random_sparse();
                        bool failed = false;
                        memset(RookAttacks[sq], 0, sizeof(RookAttacks[sq]));
                        for (int i = 0; i < count; i++)
                        {
                                int index = (occupancies[i] * candidate) >> shift;
                                if (RookAttacks[sq][index] == 0) RookAttacks[sq][index] = attacks[i];
                                else if (RookAttacks[sq][index] != attacks[i]) { failed = true; break; }
                        }
                        if (!failed) return candidate;
                }
        }
        else
        {
                Bitboard mask = BishopMasks[sq];
                int set_bit = popcount(mask);
                int shift = 64 - set_bit;
                BishopShifts[sq] = shift;
                Bitboard attacks[1 << set_bit], occupancies[1 << set_bit];
                Bitboard subset = 0;
                int count = 0;
                do
                {
                        attacks[count] = bishop_attacks_slow(sq, subset);
                        occupancies[count] = subset;
                        count++;
                        subset = (subset - mask) & mask;
                } while (subset);
                while (true)
                {
                        uint64_t candidate = random_sparse();
                        bool failed = false;
                        memset(BishopAttacks[sq], 0, sizeof(BishopAttacks[sq]));
                        for (int i = 0; i < count; i++)
                        {
                                int index = (occupancies[i] * candidate) >> shift;
                                if (BishopAttacks[sq][index] == 0) BishopAttacks[sq][index] = attacks[i];
                                else if (BishopAttacks[sq][index] != attacks[i]) { failed = true; break; }
                        }
                        if (!failed) return candidate;
                }
        }
}

void init_rook()
{
        for (int i = 0; i < 64; i++)
        {
                RookMasks[i] = rook_mask(static_cast<Square>(i));
                RookMagics[i] = find_magic(static_cast<Square>(i), true);
        }

}

void init_bishop()
{
        for (int i = 0; i < 64; i++)
        {
                BishopMasks[i] = bishop_mask(static_cast<Square>(i));
                BishopMagics[i] = find_magic(static_cast<Square>(i), false);
        }
}

void init_magics()
{
        init_rook();
        init_bishop();
}
