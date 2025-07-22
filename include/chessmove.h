#ifndef CHESSMOVE_H
#define CHESSMOVE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t from;
    uint8_t to;
    uint8_t flags; // this represents a ton of things shown below
} chessmove;

enum {
    CHESSMOVE_FLAGS_NO_PROMOTION_MASK = (1 << 0), // if no promotion is needed
    CHESSMOVE_FLAGS_Q_PROMOTION_MASK = (1 << 1), // if a promotion need and one is selected
    CHESSMOVE_FLAGS_R_PROMOTION_MASK = (1 << 1),
    CHESSMOVE_FLAGS_B_PROMOTION_MASK = (1 << 1),
    CHESSMOVE_FLAGS_N_PROMOTION_MASK = (1 << 1),
    CHESSMOVE_FLAGS_CASTLE_MASK = (1 << 1), // if move is a castle
    CHESSMOVE_FLAGS_DOUBLEPAWN_MASK = (1 << 1), // if move is a double forward pawn move
    CHESSMOVE_FLAGS_ENPESSANT_MASK = (1 << 1), // if move is an en pessant move
};

bool chessmove_is_castle(chessmove move);

bool chessmove_is_doublepawn(chessmove move);

bool chessmove_is_enpessane(chessmove move);

// add promotion getters

#endif /* CHESSMOVE_H */