#ifndef CHESSMOVE_H
#define CHESSMOVE_H

#include <stdint.h>
#include <stdbool.h>

#include "chesspiece.h"

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

static inline chesspiece chessmove_get_promotion(chessmove move) {
    
}

#endif /* CHESSMOVE_H */