#include <iostream>
#include <sstream>
#include <string>
#include "uci.h"
#include "movegen.h"
#include "search.h"

std::string move_to_str(Move m)
{
        Square from = move_from(m);
        Square to = move_to(m);
        std::string s;
        s += ('a' + file_of(from));
        s += ('1' + rank_of(from));
        s += ('a' + file_of(to));
        s += ('1' + rank_of(to));
        MoveFlags flag = move_flag(m);
        if (flag == PROMO_QUEEN) s += 'q';
        else if (flag == PROMO_ROOK) s += 'r';
        else if (flag == PROMO_BISHOP) s += 'b';
        else if (flag == PROMO_KNIGHT) s += 'n';
        return s;
}

Move parse_move(Position& pos, const std::string& s)
{
        File ff = static_cast<File>(s[0] - 'a');
        Rank fr = static_cast<Rank>(s[1] - '1');
        File tf = static_cast<File>(s[2] - 'a');
        Rank tr = static_cast<Rank>(s[3] - '1');
        Square from = make_square(ff, fr);
        Square to = make_square(tf, tr);
        MoveList moves;
        generate_moves(pos, moves);
        for (auto& move : moves)
        {
                if (move_from(move) == from && move_to(move) == to)
                {
                        if (s.size() == 5)
                        {
                                if (s[4] == 'q' && move_flag(move) == PROMO_QUEEN) return move;
                                if (s[4] == 'r' && move_flag(move) == PROMO_ROOK) return move;
                                if (s[4] == 'b' && move_flag(move) == PROMO_BISHOP) return move;
                                if (s[4] == 'n' && move_flag(move) == PROMO_KNIGHT) return move;
                        }
                        else return move;
                }
        }
        return 0;
}

void handle_position(Position& pos, std::istringstream& iss)
{
        std::string token;
        iss >> token;
        if (token == "startpos")
        {
                pos.set_fen("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
                iss >> token;
        }
        else if (token == "fen")
        {
                std::string fen;
                for (int i = 0; i < 6 && iss >> token; i++)
                {
                        if (i > 0) fen += ' ';
                        if (token == "moves") { break; }
                        fen += token;
                }
                pos.set_fen(fen);
                if (token != "moves") iss >> token;
        }
        if (token == "moves")
        {
                while (iss >> token)
                {
                        Move m = parse_move(pos, token);
                        if (m != 0) pos.make_move(m);
                }
        }
}

static int depth_for_budget(int budget_ms)
{
        if (budget_ms < 30) return 2;
        if (budget_ms < 150) return 3;
        if (budget_ms < 600) return 4;
        if (budget_ms < 2500) return 5;
        if (budget_ms < 10000) return 6;
        if (budget_ms < 40000) return 7;
        return 8;
}

void handle_go(Position& pos, std::istringstream& iss)
{
        std::string token;
        int depth = 0;
        int wtime = 0, btime = 0, winc = 0, binc = 0;
        int movetime = 0;
        int movestogo = 0;
        bool infinite = false;
        bool has_clock = false;
        bool has_depth = false;
        while (iss >> token)
        {
                if (token == "depth") { iss >> depth; has_depth = true; }
                else if (token == "wtime") { iss >> wtime; has_clock = true; }
                else if (token == "btime") { iss >> btime; has_clock = true; }
                else if (token == "winc") { iss >> winc; }
                else if (token == "binc") { iss >> binc; }
                else if (token == "movetime") { iss >> movetime; }
                else if (token == "movestogo") { iss >> movestogo; }
                else if (token == "infinite") { infinite = true; }
        }
        int search_depth;
        if (has_depth) search_depth = depth;
        else if (infinite) search_depth = 8;
        else if (movetime > 0)
        {
                int budget = movetime - 30;
                if (budget < 1) budget = 1;
                search_depth = depth_for_budget(budget);
        }
        else if (has_clock)
        {
                int time_left = (pos.get_side_to_move() == WHITE) ? wtime : btime;
                int inc = (pos.get_side_to_move() == WHITE) ? winc  : binc;
                const int overhead = 50;
                int usable = time_left - overhead;
                if (usable < 1) usable = 1;
                int divisor = (movestogo > 0) ? (movestogo + 2) : 30;
                int budget = usable / divisor + (inc * 3) / 4;
                int cap = usable / 3;
                if (budget > cap) budget = cap;
                if (budget > usable) budget = usable;
                if (budget < 1) budget = 1;
                search_depth = depth_for_budget(budget);
        }
        else search_depth = 6;
        Move best = search(pos, search_depth);
        std::cout << "bestmove " << move_to_str(best) << std::endl;
}

void uci_loop()
{
        Position pos;
        std::string line;
        while (std::getline(std::cin, line))
        {
                std::istringstream iss(line);
                std::string command;
                iss >> command;
                if (command == "uci")
                {
                        std::cout << "id name antizero-chess" << std::endl;
                        std::cout << "id author Joji0" << std::endl;
                        std::cout << "uciok" << std::endl;
                }
                else if (command == "isready") std::cout << "readyok" << std::endl;
                else if (command == "ucinewgame") pos = Position();
                else if (command == "position") handle_position(pos, iss);
                else if (command == "go") handle_go(pos, iss);
                else if (command == "quit") break;
        }
}
