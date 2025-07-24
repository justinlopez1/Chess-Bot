#include <stdbool.h>

#include "movegen.h"
#include "movelist.h"

void add_psuedo_pawn_moves(chessboard * board, movelist ml, bool white_to_move) {
    if (white_to_move) {
        uint64_t w_pawns = board->pawns & board->white_pieces;

        // calculate single pushes
        

    }
    else {
        uint64_t b_pawns = board->pawns & board->black_pieces;

    }



}

void add_pawn_moves(chessboard * board, movelist ml, bool white_to_move) {
    movelist psuedo_ml;

    // generate psuedo moves
    // validate them and put them in ml
}