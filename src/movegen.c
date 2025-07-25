#include <stdbool.h>

#include "movegen.h"
#include "movelist.h"
#include "chessboard.h"

void add_pawn_moves(chessboard* board, movelist* ml, bool white_to_move) {
    if (white_to_move) {
        uint64_t w_pawns = board->pawns & board->white_pieces;
        uint64_t all_pieces = board->white_pieces | board->black_pieces;

        // calculate single pushes
        uint64_t single_push = (w_pawns << 8) & ~all_pieces; 
        add_moves(board, ml, single_push, 8, CHESSMOVE_TYPE_NORMAL);

        // calculate double pushes
        uint64_t double_push = w_pawns & RANK_2;
        double_push = (double_push << 8) & ~all_pieces;
        double_push = (double_push << 8) & ~all_pieces;
        add_moves(board, ml, double_push, 16, CHESSMOVE_TYPE_DOUBLEPAWN);

        // calculate left take
        uint64_t left_take = (w_pawns << (8-1) & ~FILE_H) & board->black_pieces;
        add_moves(board, ml, left_take, 8-1, CHESSMOVE_TYPE_NORMAL);

        // calculate right take
        uint64_t right_take = (w_pawns << (8+1) & ~FILE_A) & board->black_pieces;
        add_moves(board, ml, right_take, 8+1, CHESSMOVE_TYPE_NORMAL);
    }
    else {
        uint64_t b_pawns = board->pawns & board->black_pieces;
    }

    chessboard_print(board);
    movelist_print(ml);

}
