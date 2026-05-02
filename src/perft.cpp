#include <iostream>
#include <cassert>
#include <string>
#include "movegen.h"

uint64_t perft(Position& pos, int depth)
{
        if (depth == 0) return 1;
        MoveList moves;
        generate_moves(pos, moves);
        uint64_t nodes = 0;
        for (auto& move : moves)
        {
                pos.make_move(move);
                nodes += perft(pos, depth - 1);
                pos.unmake_move(move);
        }
        return nodes;
}

void divide(Position& pos, int depth)
{
        MoveList moves;
        generate_moves(pos, moves);
        uint64_t total = 0;
        for (auto& move : moves)
        {
                pos.make_move(move);
                uint64_t nodes = perft(pos, depth - 1);
                total += nodes;
                Square from = move_from(move);
                Square to = move_to(move);
                char from_file = 'a' + file_of(from);
                char from_rank = '1' + rank_of(from);
                char to_file = 'a' + file_of(to);
                char to_rank = '1' + rank_of(to);
                std::cout << from_file << from_rank << to_file << to_rank << ": " << nodes << std::endl;
                pos.unmake_move(move);
        }
        std::cout << "Total: " << total << std::endl;
}

struct PerftTest
{
        std::string fen;
        std::string name;
        int max_depth;
        uint64_t expected[7];
};

int main()
{
        init_bitboards();
        init_magics();
        // all test results are taken from https://www.chessprogramming.org/Perft_Results
        PerftTest tests[] = {
                {
                        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
                        "Starting Position",
                        5,
                        {1, 20, 400, 8902, 197281, 4865609, 0}
                },
                {
                        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
                        "Kiwipete",
                        4,
                        {1, 48, 2039, 97862, 4085603, 0, 0}
                },
                {
                        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1",
                        "Position 3 (en passant + pins)",
                        5,
                        {1, 14, 191, 2812, 43238, 674624, 0}
                },
                {
                        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",
                        "Position 4 (promotions)",
                        5,
                        {1, 6, 264, 9467, 422333, 15833292, 0}
                },
                {
                        "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8",
                        "Position 5 (discovered checks)",
                        4,
                        {1, 44, 1486, 62379, 2103487, 0, 0}
                },
                {
                        "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10",
                        "Position 6 (complex middlegame)",
                        4,
                        {1, 46, 2079, 89890, 3894594, 0, 0}
                },
        };
        int total_tests = 0;
        int passed = 0;
        for (auto& test : tests)
        {
                std::cout << "\n=== " << test.name << " ===" << std::endl;
                std::cout << "FEN: " << test.fen << std::endl;
                Position pos(test.fen);
                bool all_ok = true;
                for (int d = 0; d <= test.max_depth; d++)
                {
                        uint64_t nodes = perft(pos, d);
                        bool ok = (nodes == test.expected[d]);
                        total_tests++;
                        if (ok) passed++;

                        std::cout << "  perft(" << d << ") = " << nodes;
                        if (ok) std::cout << " OK" << std::endl;
                        else
                        {
                                std::cout << " FAIL (expected " << test.expected[d] << ")" << std::endl;
                                all_ok = false;
                                if (d >= 1)
                                {
                                        std::cout << "  Divide perft(" << d << "):" << std::endl;
                                        divide(pos, d);
                                }
                        }
                }
                if (all_ok) std::cout << "  All passed!" << std::endl;
        }
        std::cout << "\n=== Results: " << passed << "/" << total_tests << " tests passed ===" << std::endl;
        if (passed == total_tests) std::cout << "Move generation is correct!" << std::endl;
        else std::cout << "Some tests failed. Use divide output to debug." << std::endl;
        return (passed == total_tests) ? 0 : 1;
}
