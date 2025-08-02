#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "chesspiece.h"
#include "chessboard.h"
#include "macros.h"
#include "chessmove.h"


undo_chessmove make_move(chessboard* board, const chessmove move) {
    undo_chessmove undo_info;
    undo_info.move_to_undo = move;
    undo_info.piece_taken = CHESSPIECE_EMPTY;
    undo_info.last_en_pessant_index = board->en_pessant_index;
    board->en_pessant_index = NO_EN_PESSANT;

    // determine piece
    uint64_t* pieces_moved = NULL;
    if (GET_BIT(board->pawns, move.from)) { pieces_moved = &(board->pawns); }
    else if (GET_BIT(board->bishops, move.from)) { pieces_moved = &(board->bishops); }
    else if (GET_BIT(board->rooks, move.from)) { pieces_moved = &(board->rooks); }
    else if (GET_BIT(board->knights, move.from)) { pieces_moved = &(board->knights); }
    else if (GET_BIT(board->queens, move.from)) { pieces_moved = &(board->queens); }
    else if (GET_BIT(board->kings, move.from)) { pieces_moved = &(board->kings); }
    else { exit(1); }

    // determine color
    bool white_to_move = 0;
    if (GET_BIT(board->white_pieces, move.from)) { white_to_move = true; }

    // check if piece is being taken
    uint64_t* other_color_ptr = NULL;
    uint64_t* same_color_ptr = NULL;
    if (white_to_move) { other_color_ptr = &board->black_pieces; same_color_ptr = &board->white_pieces; }
    else { other_color_ptr = &board->white_pieces; same_color_ptr = &board->black_pieces; }
    if (GET_BIT(*other_color_ptr, move.to)) { // this move does involve the taking of an opposing piece
        // remove the piece being taken and remeber the piece type 
        if (GET_BIT(board->pawns, move.to)) { CLEAR_BIT(board->pawns, move.to); undo_info.piece_taken = CHESSPIECE_PAWN; }
        else if (GET_BIT(board->bishops, move.to)) { CLEAR_BIT(board->bishops, move.to); undo_info.piece_taken = CHESSPIECE_BISHOP; }
        else if (GET_BIT(board->rooks, move.to)) { CLEAR_BIT(board->rooks, move.to); undo_info.piece_taken = CHESSPIECE_ROOK; }
        else if (GET_BIT(board->knights, move.to)) { CLEAR_BIT(board->knights, move.to); undo_info.piece_taken = CHESSPIECE_KNIGHT; }
        else if (GET_BIT(board->queens, move.to)) { CLEAR_BIT(board->queens, move.to); undo_info.piece_taken = CHESSPIECE_QUEEN; }
        else { CLEAR_BIT(board->kings, move.to); undo_info.piece_taken = CHESSPIECE_KING; }
    }

    // apply the move
    switch (move.type) {
        case CHESSMOVE_TYPE_NORMAL :
            CLEAR_BIT(*pieces_moved, move.from);
            SET_BIT(*pieces_moved, move.to);
            break;
        case CHESSMOVE_TYPE_DOUBLEPAWN :
            CLEAR_BIT(*pieces_moved, move.from);
            SET_BIT(*pieces_moved, move.to);
            board->en_pessant_index = (int8_t)move.to; // if en pessant move we need to remember the index that can be taken by en pessant
            break;
        case CHESSMOVE_TYPE_B_PROMOTION :
            CLEAR_BIT(*pieces_moved, move.from);
            SET_BIT(board->bishops, move.to);
            break;
        case CHESSMOVE_TYPE_Q_PROMOTION :
            CLEAR_BIT(*pieces_moved, move.from);
            SET_BIT(board->queens, move.to);
            break;
        case CHESSMOVE_TYPE_N_PROMOTION :
            CLEAR_BIT(*pieces_moved, move.from);
            SET_BIT(board->knights, move.to);
            break;
        case CHESSMOVE_TYPE_R_PROMOTION :
            CLEAR_BIT(*pieces_moved, move.from);
            SET_BIT(board->rooks, move.to);
            break;
        case CHESSMOVE_TYPE_ENPESSANT :
            CLEAR_BIT(*pieces_moved, move.from);
            SET_BIT(*pieces_moved, move.to);
            // also remove the piece being taken
            CLEAR_BIT(*pieces_moved, undo_info.last_en_pessant_index);
            CLEAR_BIT(*other_color_ptr, undo_info.last_en_pessant_index);

            break;
        default :
            exit(1);
    }

    // reset this since we can only en pessant one move after the double push

    CLEAR_BIT(*same_color_ptr, move.from);
    SET_BIT(*same_color_ptr, move.to);


    return undo_info;
}

void unmake_move(chessboard* board, undo_chessmove undo_info) {

    // remember old en pessant index
    board->en_pessant_index = undo_info.last_en_pessant_index;

    // determine piece
    uint64_t* pieces_moved = NULL;
    if (GET_BIT(board->pawns, undo_info.move_to_undo.to)) { pieces_moved = &(board->pawns); }
    else if (GET_BIT(board->bishops, undo_info.move_to_undo.to)) { pieces_moved = &(board->bishops); }
    else if (GET_BIT(board->rooks, undo_info.move_to_undo.to)) { pieces_moved = &(board->rooks); }
    else if (GET_BIT(board->knights, undo_info.move_to_undo.to)) { pieces_moved = &(board->knights); }
    else if (GET_BIT(board->queens, undo_info.move_to_undo.to)) { pieces_moved = &(board->queens); }
    else { pieces_moved = &(board->kings); }

    // determine color
    bool white_to_move = 0;
    if (GET_BIT(board->white_pieces, undo_info.move_to_undo.to)) { white_to_move = true; }

    // get colors pointers
    uint64_t* other_color_ptr = NULL;
    uint64_t* same_color_ptr = NULL;
    if (white_to_move) { other_color_ptr = &board->black_pieces; same_color_ptr = &board->white_pieces; }
    else { other_color_ptr = &board->white_pieces; same_color_ptr = &board->black_pieces; }

    // undo the move
    if (undo_info.move_to_undo.type == CHESSMOVE_TYPE_NORMAL || undo_info.move_to_undo.type == CHESSMOVE_TYPE_DOUBLEPAWN) {
        CLEAR_BIT(*pieces_moved, undo_info.move_to_undo.to);
        SET_BIT(*pieces_moved, undo_info.move_to_undo.from);
    }
    // else if any type of promotion move
    else if (
        undo_info.move_to_undo.type == CHESSMOVE_TYPE_B_PROMOTION || 
        undo_info.move_to_undo.type == CHESSMOVE_TYPE_R_PROMOTION || 
        undo_info.move_to_undo.type == CHESSMOVE_TYPE_Q_PROMOTION || 
        undo_info.move_to_undo.type == CHESSMOVE_TYPE_N_PROMOTION ) {
        CLEAR_BIT(*pieces_moved, undo_info.move_to_undo.to);
        SET_BIT(board->pawns, undo_info.move_to_undo.from);
    }
    else if (undo_info.move_to_undo.type == CHESSMOVE_TYPE_ENPESSANT) {
        CLEAR_BIT(*pieces_moved, undo_info.move_to_undo.to);
        SET_BIT(*pieces_moved, undo_info.move_to_undo.from);

        SET_BIT(board->pawns, undo_info.last_en_pessant_index);
        SET_BIT(*other_color_ptr, undo_info.last_en_pessant_index);
    }
    else {
        exit(1);
    }

    CLEAR_BIT(*same_color_ptr, undo_info.move_to_undo.to);
    SET_BIT(*same_color_ptr, undo_info.move_to_undo.from);

    // replace potential piece     
    uint64_t* taken_pieces = get_pieces_pointer(board, undo_info.piece_taken);
    if (taken_pieces != NULL) {
        SET_BIT(*taken_pieces, undo_info.move_to_undo.to);
        SET_BIT(*other_color_ptr, undo_info.move_to_undo.to);
    }
}