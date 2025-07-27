#include <stdbool.h>

#include "movegen.h"
#include "movelist.h"
#include "chessboard.h"
#include "macros.h"

void add_pawn_moves(chessboard* board, movelist* ml, bool white_to_move) {
    if (white_to_move) {
        uint64_t w_pawns = board->pawns & board->white_pieces;
        uint64_t all_pieces = board->white_pieces | board->black_pieces;

        // calculate single pushes
        uint64_t single_push = SHIFT_N(w_pawns) & ~all_pieces; 
        if (single_push)
            add_moves(board, ml, single_push, SHIFT_N_DELTA, CHESSMOVE_TYPE_NORMAL);

        // calculate double pushes
        // pawns that after the first push are on rank 3 meaning 
        // they were previously in their staarting position
        uint64_t double_push = single_push & RANK_3;
        double_push = SHIFT_N(double_push) & ~all_pieces;
        if (double_push)
            add_moves(board, ml, double_push, SHIFT_N_DELTA*2, CHESSMOVE_TYPE_DOUBLEPAWN);

        // calculate left take
        uint64_t left_take = SHIFT_NW(w_pawns) & board->black_pieces;
        add_moves(board, ml, left_take, SHIFT_NW_DELTA, CHESSMOVE_TYPE_NORMAL);

        // calculate right take
        uint64_t right_take = SHIFT_NE(w_pawns) & board->black_pieces;
        add_moves(board, ml, right_take, SHIFT_NE_DELTA, CHESSMOVE_TYPE_NORMAL);
    }
    else {
        // mirror white
        uint64_t b_pawns = board->pawns & board->black_pieces;
        uint64_t all_pieces = board->white_pieces | board->black_pieces;

        // calculate single pushes
        uint64_t single_push = SHIFT_S(b_pawns) & ~all_pieces; 
        add_moves(board, ml, single_push, SHIFT_S_DELTA, CHESSMOVE_TYPE_NORMAL);

        // calculate double pushes
        uint64_t double_push = b_pawns & RANK_6;
        double_push = SHIFT_S(double_push) & ~all_pieces;
        add_moves(board, ml, double_push, SHIFT_S_DELTA*2, CHESSMOVE_TYPE_DOUBLEPAWN);

        // calculate left take
        uint64_t left_take = SHIFT_SW(b_pawns) & board->white_pieces;
        add_moves(board, ml, left_take, SHIFT_SW_DELTA, CHESSMOVE_TYPE_NORMAL);

        // calculate right take
        uint64_t right_take = SHIFT_SE(b_pawns) & board->white_pieces;
        add_moves(board, ml, right_take, SHIFT_SE_DELTA, CHESSMOVE_TYPE_NORMAL);
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

    knight_move = SHIFT_NNE(knights) & ~same_color; // nne
    if (knight_move)
        add_moves(board, ml, knight_move, SHIFT_NNE_DELTA, CHESSMOVE_TYPE_NORMAL);

    knight_move = SHIFT_NEE(knights) & ~same_color; // nee
    if (knight_move)
        add_moves(board, ml, knight_move, SHIFT_NEE_DELTA, CHESSMOVE_TYPE_NORMAL);

    knight_move = SHIFT_SEE(knights) &  ~same_color; // see
    if (knight_move)
        add_moves(board, ml, knight_move, SHIFT_SEE_DELTA, CHESSMOVE_TYPE_NORMAL);

    knight_move = SHIFT_SSE(knights) &  ~same_color; // sse
    if (knight_move)
        add_moves(board, ml, knight_move, SHIFT_SSE_DELTA, CHESSMOVE_TYPE_NORMAL);

    knight_move = SHIFT_NNW(knights) &  ~same_color; // nnw
    if (knight_move)
        add_moves(board, ml, knight_move, SHIFT_NNW_DELTA, CHESSMOVE_TYPE_NORMAL);

    knight_move = SHIFT_NWW(knights) &  ~same_color; // nww
    if (knight_move)
        add_moves(board, ml, knight_move, SHIFT_NWW_DELTA, CHESSMOVE_TYPE_NORMAL);

    knight_move = SHIFT_SWW(knights) &  ~same_color; // sww
    if (knight_move)
        add_moves(board, ml, knight_move, SHIFT_SWW_DELTA, CHESSMOVE_TYPE_NORMAL);

    knight_move = SHIFT_SSW(knights) &  ~same_color; // ssw
    if (knight_move)
        add_moves(board, ml, knight_move, SHIFT_SSW_DELTA, CHESSMOVE_TYPE_NORMAL);
}

void add_king_moves(chessboard* board, movelist* ml, bool white_to_move) {
    uint64_t kings = board->kings;
    uint64_t same_color;

    if (white_to_move) {
        kings &= board->white_pieces;
        same_color = board->white_pieces;
    }
    else {
        kings &= board->black_pieces;
        same_color = board->black_pieces;
    }

    // now try to move the king in all 8 directions
    uint64_t king_move;

    king_move = SHIFT_N(kings) & ~same_color;
    if (king_move)
        add_moves(board, ml, king_move, SHIFT_N_DELTA, CHESSMOVE_TYPE_NORMAL);

    king_move = SHIFT_NE(kings) & ~same_color;
    if (king_move)
        add_moves(board, ml, king_move, SHIFT_NE_DELTA, CHESSMOVE_TYPE_NORMAL);

    king_move = SHIFT_E(kings) & ~same_color;
    if (king_move)
        add_moves(board, ml, king_move, SHIFT_E_DELTA, CHESSMOVE_TYPE_NORMAL);

    king_move = SHIFT_SE(kings) & ~same_color;
    if (king_move)
        add_moves(board, ml, king_move, SHIFT_SE_DELTA, CHESSMOVE_TYPE_NORMAL);

    king_move = SHIFT_S(kings) & ~same_color;
    if (king_move)
        add_moves(board, ml, king_move, SHIFT_S_DELTA, CHESSMOVE_TYPE_NORMAL);

    king_move = SHIFT_SW(kings) & ~same_color;
    if (king_move)
        add_moves(board, ml, king_move, SHIFT_SW_DELTA, CHESSMOVE_TYPE_NORMAL);

    king_move = SHIFT_W(kings) & ~same_color;
    if (king_move)
        add_moves(board, ml, king_move, SHIFT_W_DELTA, CHESSMOVE_TYPE_NORMAL);

    king_move = SHIFT_NW(kings) & ~same_color;
    if (king_move)
        add_moves(board, ml, king_move, SHIFT_NW_DELTA, CHESSMOVE_TYPE_NORMAL);
        
}
