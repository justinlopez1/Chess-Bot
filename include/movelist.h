#ifndef MOVELIST_H
#define MOVELIST_H

#include <stdint.h>

#include "chessmove.h"

// used to store list of moves
typedef struct {
    chessmove moves[256]; 
    uint8_t curr_size;
} movelist;

#endif /* MOVELIST_H */