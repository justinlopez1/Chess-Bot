#include <stdbool.h>

#include "movegen.h"
#include "movelist.h"
#include "chessboard.h"
#include "macros.h"

void add_pawn_moves(chessboard* board, movelist* moves, const bool white_to_move) {
    if (white_to_move) {
        uint64_t w_pawns = board->pawns & board->white_pieces;
        uint64_t all_pieces = board->white_pieces | board->black_pieces;
        addmove_info move_info;

        // calculate single pushes
        uint64_t single_push = SHIFT_N(w_pawns) & ~all_pieces; 
        if (single_push) {
            move_info.bitboard = single_push;
            move_info.delta = SHIFT_N_DELTA;
            move_info.move_type = CHESSMOVE_TYPE_NORMAL;
            add_moves(board, moves, &move_info);
        }

        // calculate double pushes
        // pawns that after the first push are on rank 3 meaning 
        // they were previously in their staarting position
        uint64_t double_push = single_push & RANK_3;
        double_push = SHIFT_N(double_push) & ~all_pieces;
        if (double_push) {
            move_info.bitboard = double_push;
            move_info.delta = SHIFT_N_DELTA*2;
            move_info.move_type = CHESSMOVE_TYPE_DOUBLEPAWN;
            add_moves(board, moves, &move_info);
        }

        // calculate left take
        uint64_t left_take = SHIFT_NW(w_pawns) & board->black_pieces;
        if (left_take) {
            move_info.bitboard = left_take;
            move_info.delta = SHIFT_NW_DELTA;
            move_info.move_type = CHESSMOVE_TYPE_NORMAL;
            add_moves(board, moves, &move_info);
        }

        // calculate right take
        uint64_t right_take = SHIFT_NE(w_pawns) & board->black_pieces;
        if (right_take) {
            move_info.bitboard = right_take;
            move_info.delta = SHIFT_NE_DELTA;
            move_info.move_type = CHESSMOVE_TYPE_NORMAL;
            add_moves(board, moves, &move_info);
        }
    }
    else {
        uint64_t b_pawns = board->pawns & board->black_pieces;
        uint64_t all_pieces = board->white_pieces | board->black_pieces;
        addmove_info move_info;

        // calculate single pushes
        uint64_t single_push = SHIFT_S(b_pawns) & ~all_pieces; 
        if (single_push) {
            move_info.bitboard = single_push;
            move_info.delta = SHIFT_S_DELTA;
            move_info.move_type = CHESSMOVE_TYPE_NORMAL;
            add_moves(board, moves, &move_info);
        }

        // calculate double pushes
        uint64_t double_push = single_push & RANK_6;
        double_push = SHIFT_S(double_push) & ~all_pieces;
        if (double_push) {
            move_info.bitboard = double_push;
            move_info.delta = SHIFT_S_DELTA*2;
            move_info.move_type = CHESSMOVE_TYPE_DOUBLEPAWN;
            add_moves(board, moves, &move_info);
        }

        // calculate left take
        uint64_t left_take = SHIFT_SW(b_pawns) & board->white_pieces;
        if (left_take) {
            move_info.bitboard = left_take;
            move_info.delta = SHIFT_SW_DELTA;
            move_info.move_type = CHESSMOVE_TYPE_NORMAL;
            add_moves(board, moves, &move_info);
        }

        // calculate right take
        uint64_t right_take = SHIFT_SE(b_pawns) & board->white_pieces;
        if (right_take) {
            move_info.bitboard = right_take;
            move_info.delta = SHIFT_SE_DELTA;
            move_info.move_type = CHESSMOVE_TYPE_NORMAL;
            add_moves(board, moves, &move_info);
        }
    }   
}

void add_knight_moves(chessboard* board, movelist* moves, const bool white_to_move) {
    uint64_t knights = 0;
    uint64_t not_same_color = 0;

    if (white_to_move) {
        knights = board->knights & board->white_pieces;
        not_same_color = ~board->white_pieces;
    }
    else {
        knights = board->knights & board->black_pieces;
        not_same_color = ~board->black_pieces;
    }

    uint64_t knight_move = 0;
    addmove_info move_info;

    for (uint8_t i = 0; i < TOTAL_KNIGHT_MOVES; i++) {
        shift knight_shift = KNIGHT_MOVESET[i];
        knight_shift.mask &= not_same_color;

        knight_move = shift_bitboard(knights, &knight_shift);
        if (knight_move) {
            move_info.bitboard = knight_move;
            move_info.delta = (int8_t)KNIGHT_MOVESET[i].delta;
            move_info.move_type = CHESSMOVE_TYPE_NORMAL;

            add_moves(board, moves, &move_info);
        }
    }
}

void add_king_moves(chessboard* board, movelist* moves, const bool white_to_move) {
    uint64_t kings = board->kings;
    uint64_t not_same_color = 0;

    if (white_to_move) {
        kings &= board->white_pieces;
        not_same_color = ~board->white_pieces;
    }
    else {
        kings &= board->black_pieces;
        not_same_color = ~board->black_pieces;
    }

    // now try to move the king in all 8 directions
    uint64_t king_move = 0;
    addmove_info move_info;

    for (uint8_t i = 0; i < TOTAL_KING_MOVES; i++) {
        shift king_shift = KING_MOVESET[i];
        king_shift.mask &= not_same_color;

        king_move = shift_bitboard(kings, &king_shift);
        if (king_move) {
            move_info.bitboard = king_move;
            move_info.delta = (int8_t)KING_MOVESET[i].delta;
            move_info.move_type = CHESSMOVE_TYPE_NORMAL;
            add_moves(board, moves, &move_info);
        }
    }
}
