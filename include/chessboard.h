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
void bitboard_print(uint64_t* bitboard);

// returns chesspiece enum
// bottom left square is index 0
chesspiece get_chesspiece_at(chessboard* board, uint8_t index);

// prints a single chessboard to the terminal
// lowercase = white
void chessboard_print(chessboard* board);

#endif /* CHESSBOARD_H */ 