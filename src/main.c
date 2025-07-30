#include <stdio.h>
#include <stdbool.h>

#include "chessboard.h"
#include "movegen.h"
#include "chessmove.h"

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

    chessboard knight_board = {
        // no kings, queens, bishops, rooks or pawns
        .kings        = 0ULL,
        .queens       = 0ULL,
        .bishops      = 0ULL,
        .rooks        = 0ULL,
        .pawns        = 0ULL,

        // one white knight on e4 (index 4 + 3*8 = 28)
        // .knights      = 1ULL << 28,
        .knights = 1ULL,

        // white_pieces includes that knight, black_pieces is empty
        .white_pieces = 0ULL,
        // .black_pieces = 1ULL << 28
        .black_pieces = 1ULL
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
    
    chessboard rook_board = {
        // no kings, queens, bishops, rooks or pawns
        .kings        = 0ULL,
        .queens       = 0ULL,
        .bishops      = 0ULL,
        .rooks        = 1ULL << 32 | 1ULL << 36,
        .pawns        = 0ULL,

        // one white knight on e4 (index 4 + 3*8 = 28)
        // .knights      = 1ULL << 28,
        .knights = 0ULL,

        // white_pieces includes that knight, black_pieces is empty
        .white_pieces = 1ULL << 36,
        // .black_pieces = 1ULL << 28
        .black_pieces = 1ULL << 32
    };

    movelist ml;
    movelist_init(&ml);
    // add_pawn_moves(&board, &ml, false); 
    // add_knight_moves(&knight_board, &ml, false);
    // add_slider_moves(&rook_board, &ml, false, SLIDER_ROOK, ROOK_SINGLE_SHIFTS, TOTAL_ROOK_SHIFTS);

    chessmove move;
    move.from = 8;
    move.to = 16;
    move.type = CHESSMOVE_TYPE_NORMAL;

    make_move(&board, move);
    
    add_legal_moves(&board, &ml, true);
   
    chessboard_print(&board);
    movelist_print(&ml);

    return 0;
}