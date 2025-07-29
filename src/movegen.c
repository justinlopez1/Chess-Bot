#include <stdbool.h>
#include <stdlib.h>

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
    shift knight_shift;

    addmove_info move_info;
    move_info.move_type = CHESSMOVE_TYPE_NORMAL;

    for (uint8_t i = 0; i < TOTAL_KNIGHT_SHIFTS; i++) {
        knight_shift = KNIGHT_SHIFTSET[i];
        knight_shift.mask &= not_same_color;

        knight_move = shift_bitboard(knights, &knight_shift);
        if (knight_move) {
            move_info.bitboard = knight_move;
            move_info.delta = KNIGHT_SHIFTSET[i].delta;

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
    shift king_shift;

    addmove_info move_info;
    move_info.move_type = CHESSMOVE_TYPE_NORMAL;

    for (uint8_t i = 0; i < TOTAL_KING_SHIFTS; i++) {
        king_shift = KING_SHIFTSET[i];
        king_shift.mask &= not_same_color;

        king_move = shift_bitboard(kings, &king_shift);
        if (king_move) {
            move_info.bitboard = king_move;
            move_info.delta = KING_SHIFTSET[i].delta;
            add_moves(board, moves, &move_info);
        }
    }
}

void add_slider_moves(chessboard* board, movelist* moves, const bool white_to_move, slider_type slider, const shift* shiftset, uint8_t total_shifts) {
    uint64_t pieces = 0;
    switch (slider) {
        case (SLIDER_BISHOP):
            pieces = board->bishops;
            break;
        case (SLIDER_QUEEN):
            pieces = board->queens;
            break;
        case (SLIDER_ROOK):
            pieces = board->rooks;
            break;
        default:
            exit(1);
    }

    uint64_t not_same_color = 0;
    uint64_t not_other_color = 0;

    if (white_to_move) {
        pieces &= board->white_pieces;
        not_same_color = ~board->white_pieces;
        not_other_color = ~board->black_pieces;
    }
    else {
        pieces &= board->black_pieces;
        not_same_color = ~board->black_pieces;
        not_other_color = ~board->white_pieces;
    }

    if (!pieces) {
        return;
    }

    uint64_t current_pieces = 0;
    shift piece_shift;
    int8_t shift_count = 0;

    addmove_info move_info;
    move_info.move_type = CHESSMOVE_TYPE_NORMAL;

    for (uint8_t i = 0; i < total_shifts; i++) {
        current_pieces = pieces;
        piece_shift = shiftset[i];
        piece_shift.mask &= not_same_color;
        shift_count = 1;

        // try first shift then add in while loop to use loop as if statement condition
        current_pieces = shift_bitboard(current_pieces, &piece_shift);
        while (current_pieces) {

            // add current pieces moves
            move_info.bitboard = current_pieces;
            move_info.delta = (int8_t)(piece_shift.delta * (shift_count++));
            add_moves(board, moves, &move_info);

            current_pieces &= not_other_color;

            current_pieces = shift_bitboard(current_pieces, &piece_shift);
        }
    }
}

void add_legal_moves(chessboard * board, movelist * moves, bool white_to_move) {
    add_pawn_moves(board, moves, white_to_move);
    add_knight_moves(board, moves, white_to_move);
    add_king_moves(board, moves, white_to_move);

    add_slider_moves(board, moves, white_to_move, SLIDER_ROOK, ROOK_SINGLE_SHIFTS, TOTAL_ROOK_SHIFTS);
    // add rest 

}
