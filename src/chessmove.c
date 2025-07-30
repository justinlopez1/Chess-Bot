#include <stdint.h>
#include <stdbool.h>

#include "chesspiece.h"
#include "chessboard.h"
#include "macros.h"
#include "chessmove.h"


undo_chessmove make_move(chessboard* board, const chessmove move) {
    undo_chessmove undo_info;
    undo_info.move_to_undo = move;
    undo_info.piece_taken = CHESSPIECE_EMPTY;


    // determine piece
    uint64_t* pieces_moved = NULL;
    if (GET_BIT(board->pawns, move.from)) { pieces_moved = &(board->pawns); }
    else if (GET_BIT(board->bishops, move.from)) { pieces_moved = &(board->bishops); }
    else if (GET_BIT(board->rooks, move.from)) { pieces_moved = &(board->rooks); }
    else if (GET_BIT(board->knights, move.from)) { pieces_moved = &(board->knights); }
    else if (GET_BIT(board->queens, move.from)) { pieces_moved = &(board->queens); }
    else { pieces_moved = &(board->kings); }

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
        if (GET_BIT(board->pawns, move.from)) { CLEAR_BIT(board->pawns, move.to); undo_info.piece_taken = CHESSPIECE_PAWN; }
        else if (GET_BIT(board->bishops, move.from)) { CLEAR_BIT(board->bishops, move.to); undo_info.piece_taken = CHESSPIECE_BISHOP; }
        else if (GET_BIT(board->rooks, move.from)) { CLEAR_BIT(board->rooks, move.to); undo_info.piece_taken = CHESSPIECE_ROOK; }
        else if (GET_BIT(board->knights, move.from)) { CLEAR_BIT(board->knights, move.to); undo_info.piece_taken = CHESSPIECE_KNIGHT; }
        else if (GET_BIT(board->queens, move.from)) { CLEAR_BIT(board->queens, move.to); undo_info.piece_taken = CHESSPIECE_QUEEN; }
        else { CLEAR_BIT(board->kings, move.to); undo_info.piece_taken = CHESSPIECES_KING; }
    }

    // apply the move
    CLEAR_BIT(*pieces_moved, move.from);
    SET_BIT(*pieces_moved, move.to);

    CLEAR_BIT(*same_color_ptr, move.from);
    SET_BIT(*same_color_ptr, move.to);

    return undo_info;
}

void unmake_move(chessboard* board, undo_chessmove undo_info) {

}