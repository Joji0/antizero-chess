#include "movegen.h"

void generate_pawn_moves(Position& pos, MoveList& moves)
{
        Color us = pos.get_side_to_move();
        Bitboard our_piece = pos.get_occupied(us);
        Bitboard enemy_piece = pos.get_occupied(us == WHITE ? BLACK : WHITE);
        Bitboard pawn = pos.get_piece_bb(us, PAWN);
        while (pawn)
        {
                Square from = static_cast<Square>(pop_lsb(pawn));
                Bitboard target = PawnAttacks[us][from] & enemy_piece;
                while (target)
                {
                        Square to = static_cast<Square>(pop_lsb(target));
                        if ((us == WHITE && rank_of(to) == RANK_8)  ||
                            (us == BLACK && rank_of(to) == RANK_1))
                        {
                                moves.add(make_move(from, to, PROMO_KNIGHT));
                                moves.add(make_move(from, to, PROMO_BISHOP));
                                moves.add(make_move(from, to, PROMO_ROOK));
                                moves.add(make_move(from, to, PROMO_QUEEN));
                        }
                        else
                        {
                                moves.add(make_move(from, to, CAPTURE));
                        }
                }
                if (us == WHITE)
                {
                        Square once = static_cast<Square>(from + 8);
                        if (pos.get_board(once) == NO_PIECE)
                        {
                                if (rank_of(once) == RANK_8)
                                {
                                        moves.add(make_move(from, once, PROMO_KNIGHT));
                                        moves.add(make_move(from, once, PROMO_BISHOP));
                                        moves.add(make_move(from, once, PROMO_ROOK));
                                        moves.add(make_move(from, once, PROMO_QUEEN));
                                }
                                else
                                {
                                        moves.add(make_move(from, once, NORMAL));
                                        if (rank_of(from) == RANK_2)
                                        {
                                                Square twice = static_cast<Square>(from + 16);
                                                if (pos.get_board(twice) == NO_PIECE)
                                                        moves.add(make_move(from, twice, NORMAL));
                                        }
                                }
                        }
                }
                else
                {
                        Square once = static_cast<Square>(from - 8);
                        if (pos.get_board(once) == NO_PIECE)
                        {
                                if (rank_of(once) == RANK_1)
                                {
                                        moves.add(make_move(from, once, PROMO_KNIGHT));
                                        moves.add(make_move(from, once, PROMO_BISHOP));
                                        moves.add(make_move(from, once, PROMO_ROOK));
                                        moves.add(make_move(from, once, PROMO_QUEEN));
                                }
                                else
                                {
                                        moves.add(make_move(from, once, NORMAL));
                                        if (rank_of(from) == RANK_7)
                                        {
                                                Square twice = static_cast<Square>(from - 16);
                                                if (pos.get_board(twice) == NO_PIECE)
                                                        moves.add(make_move(from, twice, NORMAL));
                                        }
                                }
                        }
                }
                if (pos.get_en_passant_sq() != NO_SQUARE && abs(file_of(from) - file_of(pos.get_en_passant_sq())) == 1)
                {
                        if (us == WHITE && rank_of(from) == RANK_5)
                                moves.add(make_move(from, pos.get_en_passant_sq(), EN_PASSANT));
                        else if (us == BLACK && rank_of(from) == RANK_4)
                                moves.add(make_move(from, pos.get_en_passant_sq(), EN_PASSANT));
                }
        }
}

void generate_knight_moves(Position& pos, MoveList& moves)
{
        Color us = pos.get_side_to_move();
        Bitboard our_piece = pos.get_occupied(us);
        Bitboard knight = pos.get_piece_bb(us, KNIGHT);
        while (knight)
        {
                Square from = static_cast<Square>(pop_lsb(knight));
                Bitboard target = KnightAttacks[from] & ~our_piece;
                while (target)
                {
                        Square to = static_cast<Square>(pop_lsb(target));
                        if (pos.get_board(to) == NO_PIECE) moves.add(make_move(from, to, NORMAL));
                        else moves.add(make_move(from, to, CAPTURE));
                }
        }
}

void generate_bishop_moves(Position& pos, MoveList& moves)
{
        Color us = pos.get_side_to_move();
        Bitboard our_piece = pos.get_occupied(us);
        Bitboard all_piece = pos.get_all_occupied();
        Bitboard bishop = pos.get_piece_bb(us, BISHOP);
        while (bishop)
        {
                Square from = static_cast<Square>(pop_lsb(bishop));
                Bitboard target = bishop_attacks(from, all_piece) & ~our_piece;
                while (target)
                {
                        Square to = static_cast<Square>(pop_lsb(target));
                        if (pos.get_board(to) == NO_PIECE) moves.add(make_move(from, to, NORMAL));
                        else moves.add(make_move(from, to, CAPTURE));
                }
        }
}

void generate_rook_moves(Position& pos, MoveList& moves)
{
        Color us = pos.get_side_to_move();
        Bitboard our_piece = pos.get_occupied(us);
        Bitboard all_piece = pos.get_all_occupied();
        Bitboard rook = pos.get_piece_bb(us, ROOK);
        while (rook)
        {
                Square from = static_cast<Square>(pop_lsb(rook));
                Bitboard target = rook_attacks(from, all_piece) & ~our_piece;
                while (target)
                {
                        Square to = static_cast<Square>(pop_lsb(target));
                        if (pos.get_board(to) == NO_PIECE) moves.add(make_move(from, to, NORMAL));
                        else moves.add(make_move(from, to, CAPTURE));
                }
        }
}

void generate_queen_moves(Position& pos, MoveList& moves)
{
        Color us = pos.get_side_to_move();
        Bitboard our_piece = pos.get_occupied(us);
        Bitboard all_piece = pos.get_all_occupied();
        Bitboard queen = pos.get_piece_bb(us, QUEEN);
        while (queen)
        {
                Square from = static_cast<Square>(pop_lsb(queen));
                Bitboard target = queen_attacks(from, all_piece) & ~our_piece;
                while (target)
                {
                        Square to = static_cast<Square>(pop_lsb(target));
                        if (pos.get_board(to) == NO_PIECE) moves.add(make_move(from, to, NORMAL));
                        else moves.add(make_move(from, to, CAPTURE));
                }
        }
}

void generate_king_moves(Position& pos, MoveList& moves)
{
        Color us = pos.get_side_to_move();
        Bitboard our_piece = pos.get_occupied(us);
        Bitboard king = pos.get_piece_bb(us, KING);
        int castling_rights = pos.get_castling_rights();
        if (us == WHITE)
        {
                if (castling_rights & WHITE_OO)
                {
                        if (pos.get_board(F1) == NO_PIECE && pos.get_board(G1) == NO_PIECE)
                                moves.add(make_move(E1, G1, CASTLE));
                }
                if (castling_rights & WHITE_OOO)
                {
                        if (pos.get_board(B1) == NO_PIECE && pos.get_board(C1) == NO_PIECE && pos.get_board(D1) == NO_PIECE)
                                moves.add(make_move(E1, C1, CASTLE));
                }
        }
        else
        {
                if (castling_rights & BLACK_OO)
                {
                        if (pos.get_board(F8) == NO_PIECE && pos.get_board(G8) == NO_PIECE)
                                moves.add(make_move(E8, G8, CASTLE));
                }
                if (castling_rights & BLACK_OOO)
                {
                        if (pos.get_board(B8) == NO_PIECE && pos.get_board(C8) == NO_PIECE && pos.get_board(D8) == NO_PIECE)
                                moves.add(make_move(E8, C8, CASTLE));
                }
        }
        Square from = static_cast<Square>(pop_lsb(king));
        Bitboard target = KingAttacks[from] & ~our_piece;
        while (target)
        {
                Square to = static_cast<Square>(pop_lsb(target));
                if (pos.get_board(to) == NO_PIECE) moves.add(make_move(from, to, NORMAL));
                else moves.add(make_move(from, to, CAPTURE));
        }
}

void generate_moves(Position& pos, MoveList& moves)
{
        generate_pawn_moves(pos, moves);
        generate_knight_moves(pos, moves);
        generate_bishop_moves(pos, moves);
        generate_rook_moves(pos, moves);
        generate_queen_moves(pos, moves);
        generate_king_moves(pos, moves);
}
