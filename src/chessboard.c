#include <stdio.h>
#include <stdint.h>

#include "chessboard.h"

// the 16*8 comes from a single line being printed = 16 charaacters,
// 1 1 1 1 1 1 1 1\n = 16 characters including spaces and newline
// and we need 8 lines printed
void bitboard_print(uint64_t* bitboard) {
    char buf[16*8+1];
    uint8_t idx = 0;

    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {
            buf[idx++] = GET_BIT(*bitboard, rank*8 + file) ? '1' : '0';
            if (file < 7) buf[idx++] = ' ';
        }
        buf[idx++] = '\n';
    }

    buf[16*8] = '\0';
    printf("%s", buf);
}

chesspiece get_chesspiece_at(chessboard* board, uint8_t index) {
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


void chessboard_print(chessboard* board) {
    char buf[16*8+1];
    uint8_t idx = 0;

    for (int rank = 7; rank >= 0; rank--) {
        for (int file = 0; file < 8; file++) {

            // buf[idx++] = GET_BIT(bitboard, rank*8 + file) ? '1' : '0';
            chesspiece piece = get_chesspiece_at(board, rank*8 + file);
            buf[idx++] = chesspiece_to_char(piece); 

            if (file < 7) buf[idx++] = ' ';
        }
        buf[idx++] = '\n';
    }

    buf[idx] = '\0'; // null terminate the string
    printf("%s", buf);
}
