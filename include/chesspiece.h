#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <stdint.h>

// represents a chesspiece in one byte
typedef uint8_t chesspiece;
enum {
    PIECE_EMPTY = 0,
    W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
    B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING
};

// converts chesspiece to a char that can be printed to the terminal
char chesspiece_to_char(chesspiece piece);

#endif /* CHESSPIECE_H */