#include <stdio.h>
#include <stdbool.h>

#include "chessboard.h"
#include "movegen.h"

int main() {

    chessboard pawn_board = {
        // no other pieces
        .kings        = 0ULL,
        .queens       = 0ULL,
        .bishops      = 0ULL,
        .knights      = 0ULL,
        .rooks        = 0ULL,
        // pawns on c4, e4 (white) and b5, d5, f5 (black)
        .pawns        = (1ULL<<26)|(1ULL<<28)   // white c4,e4
                      | (1ULL<<33)|(1ULL<<35)|(1ULL<<37), // black b5,d5,f5
        .white_pieces = (1ULL<<26)|(1ULL<<28),
        .black_pieces = (1ULL<<33)|(1ULL<<35)|(1ULL<<37)
    };

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

    movelist ml;
    movelist_init(&ml);
    add_pawn_moves(&pawn_board, &ml, true); 
    

    return 0;
}