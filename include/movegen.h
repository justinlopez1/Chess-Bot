#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <stdbool.h>

#include "movelist.h"
#include "chessmove.h"
#include "chessboard.h"
#include "macros.h"


// adds all of a move type to the movelist object moves
void add_pawn_moves(chessboard* board, movelist* moves, bool white_to_move);
void add_knight_moves(chessboard* board, movelist* moves, bool white_to_move);
void add_king_moves(chessboard* board, movelist* moves, bool white_to_move);

#endif /* MOVEGEN_H */