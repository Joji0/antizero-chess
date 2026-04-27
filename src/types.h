#ifndef TYPES_H
#define TYPES_H
#include <cstdint>
// Enumeration of basic types
enum Color { WHITE, BLACK, NUM_COLORS };
enum PieceType { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, NO_PIECE_TYPE };
enum Piece
{
        W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
        B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING,
        NO_PIECE
};
enum Square
{
        A1, B1, C1, D1, E1, F1, G1, H1,
        A2, B2, C2, D2, E2, F2, G2, H2,
        A3, B3, C3, D3, E3, F3, G3, H3,
        A4, B4, C4, D4, E4, F4, G4, H4,
        A5, B5, C5, D5, E5, F5, G5, H5,
        A6, B6, C6, D6, E6, F6, G6, H6,
        A7, B7, C7, D7, E7, F7, G7, H7,
        A8, B8, C8, D8, E8, F8, G8, H8,
        NO_SQUARE
};
enum File
{
        FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H
};
enum Rank
{
        RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8
};
enum MoveFlags
{
        NORMAL,
        CAPTURE,
        EN_PASSANT,
        CASTLE,
        PROMO_KNIGHT,
        PROMO_BISHOP,
        PROMO_ROOK,
        PROMO_QUEEN,
};

// Helper
Square make_square(File file, Rank rank)
{
        int square = rank * 8 + file;
        return static_cast<Square>(square);
}
File file_of(Square square)
{
        int file = square % 8;
        return static_cast<File>(file);
}
Rank rank_of(Square square)
{
        int rank = square / 8;
        return static_cast<Rank>(rank);
}
Piece make_piece(Color color, PieceType piece_type)
{
        int piece = color * 6 + piece_type;
        return static_cast<Piece>(piece);
}

/*
 * Move. Alignment idea: first 6 bit for from square, second 6 bit for to square,
 * last 4 bit for move flags.
 * bits:  15 14 13 12 | 11 10 9 8 7 6 | 5 4 3 2 1 0
           flags (4)  |     to (6)    |   from (6)
 */
using Move = uint16_t;
Move make_move(Square from, Square to, MoveFlags flag)
{
        return static_cast<Move>((uint16_t)from | ((uint16_t)to << 6) | ((uint16_t)flag << 12));
}
Square move_from(Move move)
{
        return static_cast<Square>(move & (0x3F));
}
Square move_to(Move move)
{
        return static_cast<Square>((move >> 6) & 0x03F);
}
MoveFlags move_flags(Move move)
{
        return static_cast<MoveFlags>((move >> 12) & 0xF);
}
#endif
