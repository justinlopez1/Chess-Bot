#include <stdint.h>
#include "chesspiece.h"

char chesspiece_to_char(chesspiece piece) {
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