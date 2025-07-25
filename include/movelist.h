#ifndef MOVELIST_H
#define MOVELIST_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "chessmove.h"
#include "chessboard.h"

static const char* square_name[64] = {
    "a1","b1","c1","d1","e1","f1","g1","h1",
    "a2","b2","c2","d2","e2","f2","g2","h2",
    "a3","b3","c3","d3","e3","f3","g3","h3",
    "a4","b4","c4","d4","e4","f4","g4","h4",
    "a5","b5","c5","d5","e5","f5","g5","h5",
    "a6","b6","c6","d6","e6","f6","g6","h6",
    "a7","b7","c7","d7","e7","f7","g7","h7",
    "a8","b8","c8","d8","e8","f8","g8","h8"
};

#define MOVELIST_MAX_SIZE 256

// used to store list of moves
typedef struct {
    chessmove moves[MOVELIST_MAX_SIZE]; 
    uint8_t curr_size;
} movelist;

static inline void movelist_init(movelist* ml) {
    ml->curr_size = 0;
    memset(ml->moves, 0, MOVELIST_MAX_SIZE); 
}

static inline uint8_t ctz(uint64_t num) {
    return __builtin_ctzll(num);
}

// adds moves to a movelist
// bitboard is bitboard filled with final destination pieces
// delta is how pieces were shifted, so if pieces were pushed 
// one rank forward it would be positive 8
// type is move type, ie CHESSMOVE_TYPE_NORMAL, CHESSMOVE_TYPE_ENPESSANT
static inline void add_moves(chessboard* board, movelist* ml, uint64_t bitboard, int8_t delta, uint8_t type) {
    // loop through bits that are 1's
    while (bitboard) {
        uint8_t idx = ctz(bitboard);

        // add move
        chessmove* move_ptr = &(ml->moves[ml->curr_size]);
        move_ptr->from = idx-delta;
        move_ptr->to = idx;
        move_ptr->type = type;
        
        // make sure the move was legal
        if (is_legal(board, *move_ptr)) {
            ml->curr_size++;
        }

        // pop bit
        bitboard &= bitboard - 1; 
    }
}

static inline void movelist_print(movelist* ml) {
    for (int i = 0; i < ml->curr_size; i++) {
        char str[8];
        memcpy(str, square_name[ml->moves[i].from], 2);
        str[2] = '-';
        str[3] = '>';
        memcpy(str+4, square_name[ml->moves[i].to], 2);
        str[6] = '\n';
        str[7] = '\0';
        printf("%s", str);
    }
}

#endif /* MOVELIST_H */