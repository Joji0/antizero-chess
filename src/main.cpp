#include <iostream>
#include <cassert>
#include "types.h"
int main()
{
        // Test make_square and extraction
        assert(make_square(FILE_E, RANK_4) == E4);
        assert(file_of(E4) == FILE_E);
        assert(rank_of(E4) == RANK_4);

        // Test corners
        assert(make_square(FILE_A, RANK_1) == A1);
        assert(make_square(FILE_H, RANK_8) == H8);

        // Test make_piece
        assert(make_piece(WHITE, QUEEN) == W_QUEEN);
        assert(make_piece(BLACK, KNIGHT) == B_KNIGHT);

        // Test move encoding
        Move m = make_move(E2, E4, NORMAL);
        assert(move_from(m) == E2);
        assert(move_to(m) == E4);
        assert(move_flags(m) == NORMAL);

        // Test with flags
        Move promo = make_move(A7, A8, PROMO_QUEEN);
        assert(move_from(promo) == A7);
        assert(move_to(promo) == A8);
        assert(move_flags(promo) == PROMO_QUEEN);

        std::cout << "All types.h tests passed!" << std::endl;
        return 0;
}
