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
// static inline cause its a quick switch case
static inline char chesspiece_to_char(chesspiece piece) {
    switch (piece) {
        case PIECE_EMPTY:
            return '.';
            break;
        case W_PAWN:
            return 'p';
            break;
        case B_PAWN:
            return 'P';
            break;
        case W_ROOK:
            return 'r';
            break;
        case W_KNIGHT:
            return 'n';
            break;
        case W_BISHOP:
            return 'b';
            break;
        case B_ROOK:
            return 'R';
            break;
        case B_KNIGHT:
            return 'N';
            break;
        case B_BISHOP:
            return 'B';
            break;
        // queen king at bottom cause lower likely hood
        case W_QUEEN:
            return 'q';
            break;
        case W_KING:
            return 'k';
            break;
        case B_QUEEN:
            return 'Q';
            break;
        case B_KING:
            return 'K';
            break;
    }
}

#endif /* CHESSPIECE_H */