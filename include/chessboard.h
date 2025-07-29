#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <stdint.h>

#include "macros.h"
#include "chesspiece.h"

// efficient way to represent a chess board
// 64 squares on board, 1's in the uint64_t represent a piece is there
typedef struct {
    uint64_t kings;
    uint64_t queens;
    uint64_t bishops;
    uint64_t knights;
    uint64_t rooks;
    uint64_t pawns;

    uint64_t white_pieces;
    uint64_t black_pieces;
} chessboard;

// prints a single bitboard to the terminal
void bitboard_print(uint64_t bitboard, const char* title);

// prints a single chessboard to the terminal
// lowercase = white
void chessboard_print(const chessboard* board);

// returns chesspiece enum
// bottom left square is index 0
// static inline cause its fast and used repeatedly elsewhere
static inline chesspiece get_chesspiece_at(const chessboard* board, uint8_t index) {
    uint64_t get_bit_mask = 1ULL << index;

    if (get_bit_mask & board->white_pieces) {
        if (get_bit_mask & board->pawns) return W_PAWN;
        if (get_bit_mask & board->rooks) return W_ROOK;
        if (get_bit_mask & board->knights) return W_KNIGHT;
        if (get_bit_mask & board->bishops) return W_BISHOP;
        if (get_bit_mask & board->queens) return W_QUEEN;
        if (get_bit_mask & board->kings) return W_KING;
    }
    else if (get_bit_mask & board->black_pieces) {
        if (get_bit_mask & board->pawns) return B_PAWN;
        if (get_bit_mask & board->rooks) return B_ROOK;
        if (get_bit_mask & board->knights) return B_KNIGHT;
        if (get_bit_mask & board->bishops) return B_BISHOP;
        if (get_bit_mask & board->queens) return B_QUEEN;
        if (get_bit_mask & board->kings) return B_KING;
    }

    // no white or black piece was found at square
    return PIECE_EMPTY;
}


#endif /* CHESSBOARD_H */ 