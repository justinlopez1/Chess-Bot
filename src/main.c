#include <stdio.h>
#include "board.h"

static const uint64_t PAWN_START = 0x00FF00000000FF00ULL;

int main() {

    bitboard_print(PAWN_START);

    return 0;
}