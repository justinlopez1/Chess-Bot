#ifndef MOVELIST_H
#define MOVELIST_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "chessmove.h"
#include "chessboard.h"

static const char* const square_name[64] = {
    "a1","b1","c1","d1","e1","f1","g1","h1",
    "a2","b2","c2","d2","e2","f2","g2","h2",
    "a3","b3","c3","d3","e3","f3","g3","h3",
    "a4","b4","c4","d4","e4","f4","g4","h4",
    "a5","b5","c5","d5","e5","f5","g5","h5",
    "a6","b6","c6","d6","e6","f6","g6","h6",
    "a7","b7","c7","d7","e7","f7","g7","h7",
    "a8","b8","c8","d8","e8","f8","g8","h8"
};

enum { 
    MOVELIST_MAX_SIZE = 256,
    MOVELIST_SINGLE_PIECE_MAX_SIZE = 32
};


// used to store list of moves
typedef struct {
    chessmove moves[MOVELIST_MAX_SIZE]; 
    uint8_t curr_size;
} movelist;

// initialize the movelist as empty
static inline void movelist_init(movelist* moves) {
    moves->curr_size = 0;
    memset(moves->moves, 0, MOVELIST_MAX_SIZE); 
}

// count trailing zeros
static inline uint8_t ctz(uint64_t num) {
    return __builtin_ctzll(num);
}

// bitboard holds the bitboard after the shift
// struct that holds the info needed to add a move to a movelist
// delta is how pieces were shifted, so if pieces were pushed 
// type is move type, ie CHESSMOVE_TYPE_NORMAL, CHESSMOVE_TYPE_ENPESSANT
typedef struct {
    uint64_t bitboard;
    int8_t delta;
    uint8_t move_type;
    bool white_to_move;
} addmove_info;

// adds moves to a movelist, can be used when all moves of the same type. Board parameter is needed to pass into
// is_legal to check if board state is legal after move is attempted
static inline void add_moves(chessboard* board, movelist* moves, const addmove_info* info) {
    uint64_t bitboard = info->bitboard;
    int8_t delta = info->delta;
    uint8_t type = info->move_type;

    // loop through bits that are 1's
    while (bitboard) {
        uint8_t idx = ctz(bitboard);

        // add move
        chessmove* move_ptr = &(moves->moves[moves->curr_size]);
        move_ptr->from = idx-delta;
        move_ptr->to = idx;
        move_ptr->type = type;
        
        // make sure the move was legal
        if (is_legal(board, *move_ptr, info->white_to_move)) {
            moves->curr_size++;
        }

        // pop bit
        bitboard &= bitboard - 1; 
    }
}

static inline void add_promotion_moves(chessboard* board, movelist* moves, addmove_info* move_info) {
    // add each promotion
    move_info->move_type = CHESSMOVE_TYPE_Q_PROMOTION;
    add_moves(board, moves, move_info);
    move_info->move_type = CHESSMOVE_TYPE_B_PROMOTION;
    add_moves(board, moves, move_info);
    move_info->move_type = CHESSMOVE_TYPE_R_PROMOTION;
    add_moves(board, moves, move_info);
    move_info->move_type = CHESSMOVE_TYPE_N_PROMOTION;
    add_moves(board, moves, move_info);
}

enum { MOVE_STRING_LENGTH = 8 };

static inline void movelist_print(const movelist* moves) {
    for (int i = 0; i < moves->curr_size; i++) {
        char str[MOVE_STRING_LENGTH+1];
        uint8_t idx = 0;

        memcpy(str+idx, square_name[moves->moves[i].from], 2);
        idx+=2;

        str[idx++] = '-';
        str[idx++] = '>';

        memcpy(str+idx, square_name[moves->moves[i].to], 2);
        idx+=2;

        switch (moves->moves[i].type) {
            case CHESSMOVE_TYPE_NORMAL :
            str[idx++] = '-';
                break;
            case CHESSMOVE_TYPE_B_PROMOTION :
                str[idx++] = 'B';
                break;
            case CHESSMOVE_TYPE_R_PROMOTION :
                str[idx++] = 'R';
                break;
            case CHESSMOVE_TYPE_Q_PROMOTION :
                str[idx++] = 'Q';
                break;
            case CHESSMOVE_TYPE_N_PROMOTION :
                str[idx++] = 'N';
                break;
            case CHESSMOVE_TYPE_DOUBLEPAWN :
                str[idx++] = 'D';
                break;
            case CHESSMOVE_TYPE_ENPESSANT :
                str[idx++] = 'E';
                break;
        }

        str[idx++] = '\n';
        str[idx++] = '\0';
        printf("%s", str);
    }
}

#endif /* MOVELIST_H */