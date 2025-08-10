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
        uint64_t single_push = shift_bitboard(w_pawns, &SHIFT_N) & ~all_pieces; 
        if (single_push) {
            // add promoting moves
            move_info.bitboard = single_push & RANK_8;
            move_info.delta = SHIFT_N.delta;
            add_promotion_moves(board, moves, &move_info);
            // then take out the regular ones to the top row
            move_info.move_type = CHESSMOVE_TYPE_NORMAL;
            move_info.bitboard = single_push & ~RANK_8;
            add_moves(board, moves, &move_info);
        }

        // calculate double pushes
        // pawns that after the first push are on rank 3 meaning 
        // they were previously in their staarting position
        uint64_t double_push = single_push & RANK_3;
        double_push = shift_bitboard(double_push, &SHIFT_N) & ~all_pieces;
        if (double_push) {
            move_info.bitboard = double_push;
            move_info.delta = (int8_t)(SHIFT_N.delta*2);
            move_info.move_type = CHESSMOVE_TYPE_DOUBLEPAWN;
            add_moves(board, moves, &move_info);
        }

        // calculate left take
        uint64_t left_take = shift_bitboard(w_pawns, &SHIFT_NW) & board->black_pieces;
        if (left_take) {
            move_info.bitboard = left_take & RANK_8;
            move_info.delta = SHIFT_NW.delta;
            add_promotion_moves(board, moves, &move_info);

            move_info.move_type = CHESSMOVE_TYPE_NORMAL;
            move_info.bitboard = left_take & ~RANK_8;
            add_moves(board, moves, &move_info);
        }


        // calculate right take
        uint64_t right_take = shift_bitboard(w_pawns, &SHIFT_NE) & board->black_pieces;
        if (right_take) {
            move_info.bitboard = right_take & RANK_8;
            move_info.delta = SHIFT_NE.delta;
            add_promotion_moves(board, moves, &move_info);

            move_info.move_type = CHESSMOVE_TYPE_NORMAL;
            move_info.bitboard = right_take & ~RANK_8;
            add_moves(board, moves, &move_info);
        }

        // calculate potential en pessants
        if (board->en_pessant_index != NO_EN_PESSANT) {
            // our any of our pawns next to that index? if so we can take it with en pessant
            uint64_t en_pessantable_square = 0ULL;
            SET_BIT(en_pessantable_square, board->en_pessant_index);

            uint64_t right = shift_bitboard(w_pawns, &SHIFT_E); 
            uint64_t left = shift_bitboard(w_pawns, &SHIFT_W);
            if (right & en_pessantable_square) {
                // we can take with en pessant
                right &= en_pessantable_square;
                // calculate normal right take shifts
                uint64_t right_take = shift_bitboard(right, &SHIFT_N);
                move_info.move_type = CHESSMOVE_TYPE_ENPESSANT;
                move_info.bitboard = right_take;
                move_info.delta = SHIFT_NE.delta;
                add_moves(board, moves, &move_info);
            }
            else if (left & en_pessantable_square) {
                left &= en_pessantable_square;
                uint64_t left_take = shift_bitboard(left, &SHIFT_N);
                move_info.move_type = CHESSMOVE_TYPE_ENPESSANT;
                move_info.bitboard = left_take;
                move_info.delta = SHIFT_NW.delta;
                add_moves(board, moves, &move_info);
            }
        }

    }
    else {
        uint64_t b_pawns = board->pawns & board->black_pieces;
        uint64_t all_pieces = board->white_pieces | board->black_pieces;
        addmove_info move_info;
        move_info.white_to_move = white_to_move;

        // calculate single pushes
        uint64_t single_push = shift_bitboard(b_pawns, &SHIFT_S) & ~all_pieces; 
        if (single_push) {
            move_info.bitboard = single_push & RANK_1;
            move_info.delta = SHIFT_S.delta;
            add_promotion_moves(board, moves, &move_info);

            move_info.move_type = CHESSMOVE_TYPE_NORMAL;
            move_info.bitboard = single_push & ~RANK_1;
            add_moves(board, moves, &move_info);
        }

        // calculate double pushes
        uint64_t double_push = single_push & RANK_6;
        double_push = shift_bitboard(double_push, &SHIFT_S) & ~all_pieces;
        if (double_push) {
            move_info.bitboard = double_push;
            move_info.delta = (int8_t)(SHIFT_S.delta*2);
            move_info.move_type = CHESSMOVE_TYPE_DOUBLEPAWN;
            add_moves(board, moves, &move_info);
        }

        // calculate left take
        uint64_t left_take = shift_bitboard(b_pawns, &SHIFT_SW) & board->white_pieces;
        if (left_take) {
            move_info.bitboard = left_take & RANK_1;
            move_info.delta = SHIFT_SW.delta;
            add_promotion_moves(board, moves, &move_info);

            move_info.move_type = CHESSMOVE_TYPE_NORMAL;
            move_info.bitboard = left_take & ~RANK_1;
            add_moves(board, moves, &move_info);
        }

        // calculate right take
        uint64_t right_take = shift_bitboard(b_pawns, &SHIFT_SE) & board->white_pieces;
        if (right_take) {
            move_info.bitboard = right_take & RANK_1;
            move_info.delta = SHIFT_SE.delta;
            add_promotion_moves(board, moves, &move_info);

            move_info.move_type = CHESSMOVE_TYPE_NORMAL;
            move_info.bitboard = right_take & ~RANK_1;
            add_moves(board, moves, &move_info);
        }

        // calculate potential en pessants
        if (board->en_pessant_index != NO_EN_PESSANT) {
            // our any of our pawns next to that index? if so we can take it with en pessant
            uint64_t en_pessantable_square = 0ULL;
            SET_BIT(en_pessantable_square, board->en_pessant_index);

            uint64_t right = shift_bitboard(b_pawns, &SHIFT_E); 
            uint64_t left = shift_bitboard(b_pawns, &SHIFT_W);
            if (right & en_pessantable_square) {
                // we can take with en pessant
                right &= en_pessantable_square;
                // calculate normal right take shifts
                uint64_t right_take = shift_bitboard(right, &SHIFT_S);
                move_info.move_type = CHESSMOVE_TYPE_ENPESSANT;
                move_info.bitboard = right_take;
                move_info.delta = SHIFT_SE.delta;
                add_moves(board, moves, &move_info);
            }
            else if (left & en_pessantable_square) {
                left &= en_pessantable_square;
                uint64_t left_take = shift_bitboard(left, &SHIFT_S);
                move_info.move_type = CHESSMOVE_TYPE_ENPESSANT;
                move_info.bitboard = left_take;
                move_info.delta = SHIFT_SW.delta;
                add_moves(board, moves, &move_info);
            }
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

    if (piece == CHESSPIECE_KING) {
        add_castle_moves(board, moves, white_to_move);
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
        shift_count = 1;

        // try first shift then add in while loop to use loop as if statement condition
        current_pieces = shift_bitboard(current_pieces, &piece_shift);
        while (current_pieces) {

            // check if our shift has us on a teammates color, if so we should remove that bit
            current_pieces &= not_same_color;
            
            // add current pieces moves
            move_info.bitboard = current_pieces;
            move_info.delta = (int8_t)(piece_shift.delta * (shift_count++));
            add_moves(board, moves, &move_info);

            // check if our shift is on top of opponents color, meaning we are taking it meaning we should remove it after adding the move
            current_pieces &= not_other_color;

            current_pieces = shift_bitboard(current_pieces, &piece_shift);
        }
    }
}

void add_castle_moves(chessboard * board, movelist * moves, bool white_to_move) {
    uint64_t pieces = board->unmoved_pieces_castle;
    if (white_to_move) {
        pieces &= board->white_pieces;
    }
    else {
        pieces &= board->black_pieces;
    }

    if (!pieces) { return; }

    // check if king is in position to be castled
    if (!(pieces & board->kings)) { return; }
    if (in_check(board, white_to_move)) { return; }

    uint64_t all_pieces = board->white_pieces | board->black_pieces;

    // check if rook is in position to be castled
    // check that no pieces are in between
    // check that king is not in check ever when moving across
    // left
    if (pieces & FILE_A) {
        uint8_t rook_idx = ctz(pieces & FILE_A);
        uint64_t king_idx = ctz(pieces & FILE_E);

        undo_chessmove undo_arr[2];
        uint8_t undo_arr_size = 0;
        bool castle_failed = false;
        // check each of these squares to make sure no piece is there, then check them to make sure resulting board is not in check if king was there
        for (int i = king_idx-1; i >= king_idx-2; i--) {
            // make sure no piece at square
            if (GET_BIT(all_pieces, i)) {
                castle_failed = true;
                break;
            }

            chessmove move;
            move.from = i+1;
            move.to = i;
            move.type = CHESSMOVE_TYPE_NORMAL;
            undo_arr[undo_arr_size++] = make_move(board, move);
            if (in_check(board, !white_to_move)) {
                castle_failed = true;
                break;
            }
        }
        // go through undo list and undo the moves
        for (int8_t i = undo_arr_size-1; i >= 0; i--) {
            unmake_move(board, undo_arr[i]);
        }
        // also make sure that one square has no piece on it
        if (!GET_BIT(all_pieces, rook_idx+1) && !castle_failed) {
            // add the castle move
            chessmove move;
            move.from = king_idx;
            move.to = king_idx-2;
            move.type = CHESSMOVE_TYPE_CASTLE;
            
            moves->moves[moves->curr_size++] = move;
        }
    }

    // right
    if (pieces & FILE_H) {

    }
}

void add_legal_moves(chessboard * board, movelist * moves, bool white_to_move) {
    add_pawn_moves(board, moves, white_to_move);

    add_nonslider_moves(board, moves, white_to_move, CHESSPIECE_KNIGHT, KNIGHT_SHIFTSET, TOTAL_KNIGHT_SHIFTS);
    add_nonslider_moves(board, moves, white_to_move, CHESSPIECE_KING, KING_QUEEN_SHIFTSET, TOTAL_KING_QUEEN_SHIFTS);

    add_slider_moves(board, moves, white_to_move, CHESSPIECE_ROOK, ROOK_SINGLE_SHIFTS, TOTAL_ROOK_SHIFTS);
    add_slider_moves(board, moves, white_to_move, CHESSPIECE_BISHOP, BISHOP_SINGLE_SHIFTS, TOTAL_BISHOP_SHIFTS);
    add_slider_moves(board, moves, white_to_move, CHESSPIECE_QUEEN, KING_QUEEN_SHIFTSET, TOTAL_KING_QUEEN_SHIFTS);

    add_castle_moves(board, moves, white_to_move);
}

uint64_t get_pawn_attacked_bitboard(const chessboard * board, bool white_to_move) {
    uint64_t attacked_bitboard = 0;
    if (white_to_move) {
        uint64_t w_pawns = board->pawns & board->white_pieces;

        // calculate left take
        uint64_t left_take = shift_bitboard(w_pawns, &SHIFT_NW);
        if (left_take) { attacked_bitboard |= left_take; }

        // calculate right take
        uint64_t right_take = shift_bitboard(w_pawns, &SHIFT_NE);
        if (right_take) { attacked_bitboard |= right_take; }
    }
    else {
        uint64_t b_pawns = board->pawns & board->black_pieces;

        // calculate left take
        uint64_t left_take = shift_bitboard(b_pawns, &SHIFT_SW);
        if (left_take) { attacked_bitboard |= left_take; }

        // calculate right take
        uint64_t right_take = shift_bitboard(b_pawns, &SHIFT_SE);
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
    uint64_t attacked_bitbaord = 0;

    for (uint8_t i = 0; i < total_shifts; i++) {
        current_pieces = pieces;
        piece_shift = shiftset[i];

        // try first shift then add in while loop to use loop as if statement condition
        current_pieces = shift_bitboard(current_pieces, &piece_shift);
        while (current_pieces) {

            // check if our shift has us on a teammates color, if so we should remove that bit
            current_pieces &= not_same_color;
            
            // add current pieces moves
            attacked_bitbaord |= current_pieces;

            // check if our shift is on top of opponents color, meaning we are taking it meaning we should remove it after adding the move
            current_pieces &= not_other_color;

            current_pieces = shift_bitboard(current_pieces, &piece_shift);
        }
    }

    return attacked_bitbaord;
}
