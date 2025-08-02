#ifndef CHESSMOVE_H
#define CHESSMOVE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "chesspiece.h"
#include "chessboard.h"
#include "macros.h"

// struct represents a chess move
// from and to are index's in the biboard
typedef struct {
    uint8_t from;
    uint8_t to;
    uint8_t type; // this represents a ton of things shown below
} chessmove;

// information needed to undo a chessmove
typedef struct {
    chessmove move_to_undo; // the move that was previously made, so to undo it you would perform this move backwards
    chesspiece piece_taken; // indicates the piece that was taken, if applicable
    int8_t last_en_pessant_index;
} undo_chessmove;

enum {
    CHESSMOVE_TYPE_NORMAL = 0,
    CHESSMOVE_TYPE_Q_PROMOTION,
    CHESSMOVE_TYPE_R_PROMOTION,
    CHESSMOVE_TYPE_B_PROMOTION,
    CHESSMOVE_TYPE_N_PROMOTION,
    CHESSMOVE_TYPE_CASTLE,
    CHESSMOVE_TYPE_DOUBLEPAWN,
    CHESSMOVE_TYPE_ENPESSANT
};

// performs a chessmove on a board, modifying the data that the pointer board is pointing to
undo_chessmove make_move(chessboard* board, chessmove move); 

// performs the reverse of a chessmove on a board, modifying the data that the pointer board is pointing to
void unmake_move(chessboard* board, undo_chessmove undo_info);

// return stdbool indicating wether or not the provided move is legal on the provided board
// a move is illegal if it tries to move a pinned piece, so this is all this function checks for
// the provided move should be a valid psuedo move
static inline bool is_legal(chessboard* board, chessmove move, bool white_to_move) {
    uint64_t queens = board->queens;
    uint64_t white = board->white_pieces;
    uint64_t black = board->black_pieces;
    undo_chessmove undo_info = make_move(board, move);
    bool ret = in_check(board, white_to_move);
    unmake_move(board, undo_info);
    return !ret;
} 

// returns pointer to the bitboard that this piece is associated to
static inline uint64_t* get_pieces_pointer(chessboard* board, chesspiece piece) {
    switch (piece) {
        case (CHESSPIECE_EMPTY) :
            return NULL;
        case (CHESSPIECE_PAWN) :
            return &board->pawns;
        case (CHESSPIECE_KNIGHT) :
            return &board->knights;
        case (CHESSPIECE_BISHOP) :
            return &board->bishops;
        case (CHESSPIECE_ROOK) :
            return &board->rooks;
        case (CHESSPIECE_QUEEN) :
            return &board->queens;
        case (CHESSPIECE_KING) :
            return &board->kings;
        default :
            exit(1);
    }
}


#endif /* CHESSMOVE_H */