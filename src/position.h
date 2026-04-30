#ifndef POSITION_H
#define POSITION_H
#include <string>
#include <vector>
#include "bitboard.h"

// Position for FEN
class Position
{
        private:
                Bitboard piece_bb[NUM_COLORS][NO_PIECE_TYPE] = {};
                Bitboard occupied[NUM_COLORS] = {};
                Bitboard all_occupied = 0;
                Piece board[NO_SQUARE] = {};
                Color side_to_move = WHITE;
                int castling_rights = 0;
                Square en_passant_sq = NO_SQUARE;
                int halfmove_clock = 0;
                int fullmove_number = 1;
                void clear()
                {
                        for (int i = 0; i < NO_SQUARE; i++) { board[i] = NO_PIECE; }
                        for (int c = 0; c < NUM_COLORS; c++)
                        {
                                for (int p = 0; p < NO_PIECE_TYPE; p++) { piece_bb[c][p] = 0; }
                        }
                        side_to_move = WHITE;
                        all_occupied = 0;
                        castling_rights = 0;
                        en_passant_sq = NO_SQUARE;
                        halfmove_clock = 0;
                        fullmove_number = 1;
                }
                std::string::const_iterator parse_board(const std::string &s, std::string::const_iterator it);
                std::string::const_iterator parse_active_color(const std::string &s, std::string::const_iterator it);
                std::string::const_iterator parse_castling_rights(const std::string &s, std::string::const_iterator it);
                std::string::const_iterator parse_en_passant(const std::string &s, std::string::const_iterator it);
                std::string::const_iterator parse_halfmove(const std::string &s, std::string::const_iterator it);
                std::string::const_iterator parse_fullmove(const std::string &s, std::string::const_iterator it);
                struct UndoInfo
                {
                        int castling_rights;
                        Square en_passant_sq;
                        int halfmove_clock;
                        Piece captured_piece;
                };
                std::vector<UndoInfo> undo;
        public:
                Position() { clear(); };
                Position(const std::string& s)
                {
                        clear();
                        set_fen(s);
                        undo.clear();
                }
                inline Piece piece_on(Square sq) const { return board[sq]; }
                inline void place_piece(Piece piece, Square sq)
                {
                        assert(piece != NO_PIECE);
                        PieceType piece_type = static_cast<PieceType>(piece % 6);
                        Color color = static_cast<Color>(piece / 6);
                        piece_bb[color][piece_type] |= 1ULL << sq;
                        occupied[color] |= 1ULL << sq;
                        all_occupied |= 1ULL << sq;
                        board[sq] = piece;
                }
                inline void remove_piece(Square sq)
                {
                        Piece piece = board[sq];
                        if (piece == NO_PIECE) { return; }
                        PieceType piece_type = static_cast<PieceType>(piece % 6);
                        Color color = static_cast<Color>(piece / 6);
                        piece_bb[color][piece_type] &= ~(1ULL << sq);
                        occupied[color] &= ~(1ULL << sq);
                        all_occupied &= ~(1ULL << sq);
                        board[sq] = NO_PIECE;
                }
                void set_fen(const std::string& s);
                std::string to_fen() const;
                void print() const;
                void make_move(Move m);
                void unmake_move(Move m);
                Bitboard get_piece_bb(Color color, PieceType piece_type) { return piece_bb[color][piece_type]; }
                Bitboard get_occupied(Color color) { return occupied[color]; }
                Bitboard get_all_occupied() { return all_occupied; }
                Piece get_board(Square sq) { return board[sq]; }
                Color get_side_to_move() { return side_to_move; }
                int get_castling_rights() { return castling_rights; }
                Square get_en_passant_sq() { return en_passant_sq; }
                int get_halfmove_clock() { return halfmove_clock; }
                int get_fullmove_number() { return fullmove_number; }
};
#endif
