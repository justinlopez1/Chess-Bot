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
        // mirror white
        uint64_t b_pawns = board->pawns & board->black_pieces;
        uint64_t all_pieces = board->white_pieces | board->black_pieces;

        // calculate single pushes
        uint64_t single_push = (b_pawns >> 8) & ~all_pieces; 
        add_moves(board, ml, single_push, -8, CHESSMOVE_TYPE_NORMAL);

        // calculate double pushes
        uint64_t double_push = b_pawns & RANK_7;
        double_push = (double_push >> 8) & ~all_pieces;
        double_push = (double_push >> 8) & ~all_pieces;
        add_moves(board, ml, double_push, -16, CHESSMOVE_TYPE_DOUBLEPAWN);

        // calculate left take
        uint64_t left_take = (b_pawns >> (8+1) & ~FILE_H) & board->white_pieces;
        add_moves(board, ml, left_take, -(8+1), CHESSMOVE_TYPE_NORMAL);

        // calculate right take
        uint64_t right_take = (b_pawns >> (8-1) & ~FILE_A) & board->white_pieces;
        add_moves(board, ml, right_take, -(8-1), CHESSMOVE_TYPE_NORMAL);
    }
}

void add_knight_moves(chessboard* board, movelist* ml, bool white_to_move) {
    uint64_t knights = 0;
    uint64_t same_color = 0;
    if (white_to_move) {
        knights = board->knights & board->white_pieces;
        same_color = board->white_pieces;
    }
    else {
        knights = board->knights & board->black_pieces;
        same_color = board->black_pieces;
    }

    uint64_t knight_move = 0;
    knight_move = (knights << 17) & ~(FILE_A|same_color); // nne
    if (knight_move)
        add_moves(board, ml, knight_move, 17, CHESSMOVE_TYPE_NORMAL);

    knight_move = (knights << 10) & ~(FILE_A|FILE_B|same_color); // nee
    if (knight_move)
        add_moves(board, ml, knight_move, 10, CHESSMOVE_TYPE_NORMAL);

    knight_move = (knights >> 6) &  ~(FILE_A|FILE_B|same_color); // see
    if (knight_move)
        add_moves(board, ml, knight_move, -6, CHESSMOVE_TYPE_NORMAL);

    knight_move = (knights >> 15) &  ~(FILE_A|same_color); // sse
    if (knight_move)
        add_moves(board, ml, knight_move, -15, CHESSMOVE_TYPE_NORMAL);

    knight_move = (knights << 15) &  ~(FILE_H|same_color); // nnw
    if (knight_move)
        add_moves(board, ml, knight_move, 15, CHESSMOVE_TYPE_NORMAL);

    knight_move = (knights << 6) &  ~(FILE_G|FILE_H|same_color); // nww
    if (knight_move)
        add_moves(board, ml, knight_move, 6, CHESSMOVE_TYPE_NORMAL);

    knight_move = (knights >> 10) &  ~(FILE_G|FILE_H|same_color); // sww
    if (knight_move)
        add_moves(board, ml, knight_move, -10, CHESSMOVE_TYPE_NORMAL);

    knight_move = (knights >> 17) &  ~(FILE_H|same_color); // ssw
    if (knight_move)
        add_moves(board, ml, knight_move, -17, CHESSMOVE_TYPE_NORMAL);
}
