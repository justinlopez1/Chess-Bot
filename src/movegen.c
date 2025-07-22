#include <stdbool.h>

#include "movegen.h"
#include "chessmove_list.h"

void add_psuedo_pawn_moves(chessboard * board, chessmove_list ml, bool white_to_move) {
    uint64_t w_pawns = board->pawns & board->white_pieces;
}

void add_pawn_moves(chessboard * board, chessmove_list ml, bool white_to_move) {
    chessmove_list psuedo_ml;

    // generate psuedo moves
    // validate them and put them in ml
}