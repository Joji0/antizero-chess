#include <iostream>
#include <iterator>
#include <cctype>
#include "position.h"

// parse helper
Piece char_to_piece(char c)
{
        switch(c)
        {
                case 'P': return W_PAWN;
                case 'N': return W_KNIGHT;
                case 'B': return W_BISHOP;
                case 'R': return W_ROOK;
                case 'Q': return W_QUEEN;
                case 'K': return W_KING;
                case 'p': return B_PAWN;
                case 'n': return B_KNIGHT;
                case 'b': return B_BISHOP;
                case 'r': return B_ROOK;
                case 'q': return B_QUEEN;
                case 'k': return B_KING;
                default: return NO_PIECE;
        }
}

char piece_to_char(Piece p)
{
        const char table[] = "PNBRQKpnbrqk.";
        return table[p];
}

std::string::const_iterator Position::parse_board(const std::string &s, std::string::const_iterator it)
{
        std::string::const_iterator ret = it;
        int file = 0;
        int rank = 7;
        for (; ret != s.end() && *ret != ' '; ret++)
        {
                if (*ret == '/')
                {
                        assert(file == 8 && rank > 0);
                        file = 0;
                        rank--;
                }
                else
                {
                        if (isdigit(*ret)) { file += *ret - '0'; }
                        else if (isupper(*ret) || islower(*ret))
                        {
                                place_piece(char_to_piece(*ret), make_square(static_cast<File>(file), static_cast<Rank>(rank)));
                                file++;
                        }
                        else assert(false);
                }
        }
        return ret;
}

std::string::const_iterator Position::parse_active_color(const std::string &s, std::string::const_iterator it)
{
        std::string::const_iterator ret = it;
        while (ret != s.end() && *ret == ' ') ret++;
        if (*ret == 'w') side_to_move = WHITE;
        else if (*ret == 'b') side_to_move = BLACK;
        else assert(false);
        ret++;
        return ret;
}

std::string::const_iterator Position::parse_castling_rights(const std::string &s, std::string::const_iterator it)
{
        std::string::const_iterator ret = it;
        while (ret != s.end() && *ret == ' ') ret++;
        if (*ret == '-') return ++ret;
        for (; ret != s.end() && *ret != ' '; ret++)
        {
                if (*ret == 'K') castling_rights |= WHITE_OO;
                else if (*ret == 'Q') castling_rights |= WHITE_OOO;
                else if (*ret == 'k') castling_rights |= BLACK_OO;
                else if (*ret == 'q') castling_rights |= BLACK_OOO;
        }
        return ret;
}

std::string::const_iterator Position::parse_en_passant(const std::string &s, std::string::const_iterator it)
{
        std::string::const_iterator ret = it;
        while (ret != s.end() && *ret == ' ') ret++;
        if (*ret == '-') return ++ret;
        File file = NO_FILE;
        Rank rank = NO_RANK;
        for (; ret != s.end() && *ret != ' '; ret++)
        {
                if (islower(*ret))
                {
                        switch(*ret)
                        {
                                case 'a': file = FILE_A; break;
                                case 'b': file = FILE_B; break;
                                case 'c': file = FILE_C; break;
                                case 'd': file = FILE_D; break;
                                case 'e': file = FILE_E; break;
                                case 'f': file = FILE_F; break;
                                case 'g': file = FILE_G; break;
                                case 'h': file = FILE_H; break;
                        }
                }
                else if (isdigit(*ret)) rank = static_cast<Rank>(*ret - '1');
                else assert(false);
        }
        assert(file != NO_FILE && rank != NO_RANK);
        en_passant_sq = make_square(file, rank);
        return ret;
}

std::string::const_iterator Position::parse_halfmove(const std::string &s, std::string::const_iterator it)
{
        std::string::const_iterator ret = it;
        while (ret != s.end() && *ret == ' ') ret++;
        std::string got = "";
        for (; ret != s.end() && *ret != ' '; ret++) got += *ret;
        halfmove_clock = stoi(got);
        return ret;
}

std::string::const_iterator Position::parse_fullmove(const std::string &s, std::string::const_iterator it)
{
        std::string::const_iterator ret = it;
        while (ret != s.end() && *ret == ' ') ret++;
        std::string got = "";
        for (; ret != s.end() && *ret != ' '; ret++) got += *ret;
        fullmove_number = stoi(got);
        return ret;
}

// set board to current FEN string
void Position::set_fen(const std::string& s)
{
        clear();
        std::string::const_iterator it = s.begin();
        it = parse_board(s, it);
        it = parse_active_color(s, it);
        it = parse_castling_rights(s, it);
        it = parse_en_passant(s, it);
        it = parse_halfmove(s, it);
        it = parse_fullmove(s, it);
}

// export position to FEN
std::string Position::to_fen() const
{
        std::string fen = "";
        int cnt = 0;
        for (int rank = 7; rank >= 0; rank--)
        {
                for (int file = 0; file < 8; file++)
                {
                        Square sq = make_square(static_cast<File>(file), static_cast<Rank>(rank));
                        if (board[sq] == NO_PIECE) cnt++;
                        else
                        {
                                if (cnt > 0)
                                {
                                        fen += std::to_string(cnt);
                                        cnt = 0;
                                }
                                fen += piece_to_char(board[sq]);
                        }
                }
                if (cnt > 0)
                {
                        fen += std::to_string(cnt);
                        cnt = 0;
                }
                if (rank > 0) fen += '/';
        }
        fen += ' ';
        if (side_to_move == WHITE) fen += 'w';
        else fen += 'b';
        fen += ' ';
        if (castling_rights == 0) fen += '-';
        else
        {
                if (castling_rights & 1) fen += 'K';
                if (castling_rights & 2) fen += 'Q';
                if (castling_rights & 4) fen += 'k';
                if (castling_rights & 8) fen += 'q';
        }
        fen += ' ';
        if (en_passant_sq == NO_SQUARE) fen += '-';
        else
        {
                fen += 'a' + file_of(en_passant_sq);
                fen += '1' + rank_of(en_passant_sq);
        }
        fen += ' ';
        fen += std::to_string(halfmove_clock);
        fen += ' ';
        fen += std::to_string(fullmove_number);
        return fen;
}

void Position::print() const
{
        for (int rank = 7; rank >= 0; rank--)
        {
                for(int file = 0; file < 8; file++)
                {
                        Square sq = make_square(static_cast<File>(file), static_cast<Rank>(rank));
                        if (board[sq] == NO_PIECE) std::cout << ". ";
                        else std::cout << piece_to_char(board[sq]) << ' ';
                }
                std::cout << "\n";
        }
}
