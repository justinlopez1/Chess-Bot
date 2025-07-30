#ifndef CHESSMOVE_H
#define CHESSMOVE_H

#include <stdint.h>
#include <stdbool.h>

#include "chesspiece.h"
#include "chessboard.h"

// struct represents a chess move
typedef struct {
    uint8_t from;
    uint8_t to;
    uint8_t type; // this represents a ton of things shown below
} chessmove;

enum {
    CHESSMOVE_TYPE_NORMAL = 0,
    CHESSMOVE_TYPE_Q_PROMOTION,
    CHESSMOVE_TYPE_R_PROMOTION,
    CHESSMOVE_TYPE_B_PROMOTION,
    CHESSMOVE_TYPE_N_PROMOTION,
    CHESSMOVE_TYPE_CASTLE,
    CHESSMOVE_TYPE_DOUBLEPAWN,
    CHESSMOVE_TYPE_ENPESSANT
};

static inline bool is_legal(chessboard* board, chessmove move) {
    // make move on baord
    // check if board is in check
    // undo the move
    // return true or false
   return true; 
}

#endif /* CHESSMOVE_H */