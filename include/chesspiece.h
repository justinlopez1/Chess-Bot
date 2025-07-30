#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <stdint.h>

// represents a chesspiece in one byte
// only used in the printing functions
typedef uint8_t chesspiece;
enum {
    PIECE_EMPTY = 0,
    W_PAWN, W_KNIGHT, W_BISHOP, W_ROOK, W_QUEEN, W_KING,
    B_PAWN, B_KNIGHT, B_BISHOP, B_ROOK, B_QUEEN, B_KING
};

// representation of chesspiece
enum {
    CHESSPIECE_EMPTY = 0,
    CHESSPIECE_PAWN, CHESSPIECE_KNIGHT, CHESSPIECE_BISHOP, CHESSPIECE_ROOK, CHESSPIECE_QUEEN, CHESSPIECES_KING
};

// represents slider types
typedef uint8_t slider_type;
enum {
    SLIDER_ROOK,
    SLIDER_BISHOP,
    SLIDER_QUEEN
};

// converts chesspiece to a char that can be printed to the terminal
// static inline cause its a quick switch case
static inline char chesspiece_to_char(const chesspiece piece) {
    switch (piece) {
        case PIECE_EMPTY:
            return '.';
        case W_PAWN:
            return 'p';
        case B_PAWN:
            return 'P';
        case W_ROOK:
            return 'r';
        case W_KNIGHT:
            return 'n';
        case W_BISHOP:
            return 'b';
        case B_ROOK:
            return 'R';
        case B_KNIGHT:
            return 'N';
        case B_BISHOP:
            return 'B';
        // queen king at bottom cause lower likely hood
        case W_QUEEN:
            return 'q';
        case W_KING:
            return 'k';
        case B_QUEEN:
            return 'Q';
        case B_KING:
            return 'K';
        default:
            return ' ';
    }
}

#endif /* CHESSPIECE_H */