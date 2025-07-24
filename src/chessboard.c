#include <stdio.h>
#include <stdint.h>
#include <string.h>

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
    char buf[(8+8+3)*(8+1)+3+1];
    uint8_t idx = 0;

    char ranks[8] = {
        '1', '2', '3', '4', '5', '6', '7', '8'
    };
    char files[15] = {
        'a', ' ', 'b', ' ', 'c', ' ', 'd', 
        ' ', 'e', ' ', 'f', ' ', 'g', ' ', 'h'
    };
    uint8_t rank_idx = 0;
    uint8_t file_idx = 0;

    for (int rank = 7; rank >= 0; rank--) {
        // put in rank label then two spaces
        buf[idx++] = ranks[rank_idx++];
        buf[idx++] = ' ';
        buf[idx++] = ' ';
        for (int file = 0; file < 8; file++) {

            chesspiece piece = get_chesspiece_at(board, rank*8 + file);
            buf[idx++] = chesspiece_to_char(piece); 

            if (file < 7) buf[idx++] = ' ';
        }
        buf[idx++] = '\n';
    }
    buf[idx++] = '\n';
    buf[idx++] = ' ';
    buf[idx++] = ' ';
    buf[idx++] = ' ';

    memcpy(buf+idx, files, 15);
    idx += 15;

    // null terminate the string
    buf[idx] = '\0';
    printf("%s", buf);
}
