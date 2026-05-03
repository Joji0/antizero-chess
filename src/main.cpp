#include "bitboard.h"
#include "magics.h"
#include "uci.h"

int main()
{
        init_bitboards();
        init_magics();
        uci_loop();
        return 0;
}
