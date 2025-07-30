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
        move_info.white_to_move = white_to_move;

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
        move_info.white_to_move = white_to_move;

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

void add_nonslider_moves(chessboard* board, movelist* moves, bool white_to_move, chesspiece piece, const shift* shiftset, uint8_t total_shifts) {
    uint64_t pieces = 0;
    switch (piece) {
        case (CHESSPIECE_KNIGHT) :
            pieces = board->knights;
            break;
        case (CHESSPIECE_KING) :
            pieces = board->kings;
            break;
        default :
            exit(1);
    }
    uint64_t not_same_color = 0;

    if (white_to_move) {
        pieces &= board->white_pieces;
        not_same_color = ~board->white_pieces;
    }
    else {
        pieces &= board->black_pieces;
        not_same_color = ~board->black_pieces;
    }

    if (!pieces) { return; }

    // now compute all its moves
    uint64_t piece_move = 0;
    shift piece_shift;

    addmove_info move_info;
    move_info.move_type = CHESSMOVE_TYPE_NORMAL;
    move_info.white_to_move = white_to_move;

    for (uint8_t i = 0; i < total_shifts; i++) {
        piece_shift = shiftset[i];
        piece_shift.mask &= not_same_color;

        piece_move = shift_bitboard(pieces, &piece_shift);
        if (piece_move) {
            move_info.bitboard = piece_move;
            move_info.delta = shiftset[i].delta;
            add_moves(board, moves, &move_info);
        }
    }
}

void add_slider_moves(chessboard* board, movelist* moves, const bool white_to_move, chesspiece piece, const shift* shiftset, uint8_t total_shifts) {
    uint64_t pieces = 0;
    switch (piece) {
        case (CHESSPIECE_BISHOP):
            pieces = board->bishops;
            break;
        case (CHESSPIECE_QUEEN):
            pieces = board->queens;
            break;
        case (CHESSPIECE_ROOK):
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

    if (!pieces) { return; }

    uint64_t current_pieces = 0;
    shift piece_shift;
    int8_t shift_count = 0;

    addmove_info move_info;
    move_info.move_type = CHESSMOVE_TYPE_NORMAL;
    move_info.white_to_move = white_to_move;

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

    add_nonslider_moves(board, moves, white_to_move, CHESSPIECE_KNIGHT, KNIGHT_SHIFTSET, TOTAL_KNIGHT_SHIFTS);
    add_nonslider_moves(board, moves, white_to_move, CHESSPIECE_KING, KING_QUEEN_SHIFTSET, TOTAL_KING_QUEEN_SHIFTS);

    add_slider_moves(board, moves, white_to_move, CHESSPIECE_ROOK, ROOK_SINGLE_SHIFTS, TOTAL_ROOK_SHIFTS);
    add_slider_moves(board, moves, white_to_move, CHESSPIECE_BISHOP, BISHOP_SINGLE_SHIFTS, TOTAL_BISHOP_SHIFTS);
    add_slider_moves(board, moves, white_to_move, CHESSPIECE_QUEEN, KING_QUEEN_SHIFTSET, TOTAL_KING_QUEEN_SHIFTS);
}

uint64_t get_pawn_attacked_bitboard(const chessboard * board, bool white_to_move) {
    uint64_t attacked_bitboard = 0;
    if (white_to_move) {
        uint64_t w_pawns = board->pawns & board->white_pieces;
        uint64_t all_pieces = board->white_pieces | board->black_pieces;

        // calculate left take
        uint64_t left_take = SHIFT_NW(w_pawns);
        if (left_take) { attacked_bitboard |= left_take; }

        // calculate right take
        uint64_t right_take = SHIFT_NE(w_pawns);
        if (right_take) { attacked_bitboard |= left_take; }
    }
    else {
        uint64_t b_pawns = board->pawns & board->black_pieces;
        uint64_t all_pieces = board->white_pieces | board->black_pieces;

        // calculate left take
        uint64_t left_take = SHIFT_SW(b_pawns);
        if (left_take) { attacked_bitboard |= left_take; }

        // calculate right take
        uint64_t right_take = SHIFT_SE(b_pawns);
        if (right_take) { attacked_bitboard |= right_take; }
    }   
    return attacked_bitboard;;
}

uint64_t get_nonslider_attacked_bitboard(const chessboard * board, bool white_to_move, chesspiece piece, const shift * shiftset, uint8_t total_shifts) {
    uint64_t pieces = 0;
    switch (piece) {
        case (CHESSPIECE_KNIGHT) :
            pieces = board->knights;
            break;
        case (CHESSPIECE_KING) :
            pieces = board->kings;
            break;
        default :
            exit(1);
    }
    uint64_t not_same_color = 0;

    if (white_to_move) {
        pieces &= board->white_pieces;
        not_same_color = ~board->white_pieces;
    }
    else {
        pieces &= board->black_pieces;
        not_same_color = ~board->black_pieces;
    }

    if (!pieces) { return 0; }

    shift piece_shift;
    uint64_t attacked_bitboard = 0;

    for (uint8_t i = 0; i < total_shifts; i++) {
        piece_shift = shiftset[i];
        piece_shift.mask &= not_same_color;

        attacked_bitboard |= shift_bitboard(pieces, &piece_shift);
    }
    
    return attacked_bitboard;
}

uint64_t get_slider_attacked_bitboard(const chessboard * board, bool white_to_move, chesspiece piece, const shift * shiftset, uint8_t total_shifts) {
    uint64_t pieces = 0;
    switch (piece) {
        case (CHESSPIECE_BISHOP):
            pieces = board->bishops;
            break;
        case (CHESSPIECE_QUEEN):
            pieces = board->queens;
            break;
        case (CHESSPIECE_ROOK):
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

    if (!pieces) { return 0; }

    uint64_t current_pieces = 0;
    shift piece_shift;
    int8_t shift_count = 0;
    uint64_t attacked_bitbaord = 0;

    for (uint8_t i = 0; i < total_shifts; i++) {
        current_pieces = pieces;
        piece_shift = shiftset[i];
        piece_shift.mask &= not_same_color;
        shift_count = 1;

        while (current_pieces) {
            current_pieces = shift_bitboard(current_pieces, &piece_shift);
            attacked_bitbaord |= current_pieces;
        }
    }
    return attacked_bitbaord;
}
