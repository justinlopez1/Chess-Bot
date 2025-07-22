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
