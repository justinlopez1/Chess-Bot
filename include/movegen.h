#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <stdbool.h>

#include "chessmove_list.h"
#include "chessmove.h"
#include "chessboard.h"

// adds all psuedo pawn moves to ml
void add_psuedo_pawn_moves(chessboard* board, chessmove_list ml, bool white_to_move);

// adds all legal pawn moves to ml
void add_pawn_moves(chessboard* board, chessmove_list ml, bool white_to_move);

#endif MOVEGEN_H