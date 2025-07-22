#ifndef CHESSMOVE_LIST_H
#define CHESSMOVE_LIST_H

#include <stdint.h>

#include "chessmove.h"

// used to store list of moves
typedef struct {
    chessmove moves[256] = { 0 };
    uint8_t curr_size = 0;
} chessmove_list;

#endif /* CHESSMOVE_LIST_H */