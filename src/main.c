#include <stdio.h>
#include "chessboard.h"

int main() {

    chessboard board = {
        // piece‐type bitboards (both colors)
        .kings        = (1ULL<<4)  | (1ULL<<60),
        .queens       = (1ULL<<3)  | (1ULL<<59),
        .bishops      = (1ULL<<2)  | (1ULL<<5)  | (1ULL<<58) | (1ULL<<61),
        .knights      = (1ULL<<1)  | (1ULL<<6)  | (1ULL<<57) | (1ULL<<62),
        .rooks        = (1ULL<<0)  | (1ULL<<7)  | (1ULL<<56) | (1ULL<<63),
        .pawns        = 0x00FF00000000FF00ULL,
        // color bitboards
        .white_pieces = 0x000000000000FFFFULL,
        .black_pieces = 0xFFFF000000000000ULL
    };

    chessboard_print(&board);

    return 0;
}