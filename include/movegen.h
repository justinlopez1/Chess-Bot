#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <stdbool.h>

#include "movelist.h"
#include "chessmove.h"
#include "chessboard.h"

// Rank masks
static const uint64_t RANK_1 = 0x00000000000000FFULL;
static const uint64_t RANK_2 = 0x000000000000FF00ULL;
static const uint64_t RANK_3 = 0x0000000000FF0000ULL;
static const uint64_t RANK_4 = 0x00000000FF000000ULL;
static const uint64_t RANK_5 = 0x000000FF00000000ULL;
static const uint64_t RANK_6 = 0x0000FF0000000000ULL;
static const uint64_t RANK_7 = 0x00FF000000000000ULL;
static const uint64_t RANK_8 = 0xFF00000000000000ULL;

// File masks
static const uint64_t FILE_A = 0x0101010101010101ULL;
static const uint64_t FILE_B = 0x0202020202020202ULL;
static const uint64_t FILE_C = 0x0404040404040404ULL;
static const uint64_t FILE_D = 0x0808080808080808ULL;
static const uint64_t FILE_E = 0x1010101010101010ULL;
static const uint64_t FILE_F = 0x2020202020202020ULL;
static const uint64_t FILE_G = 0x4040404040404040ULL;
static const uint64_t FILE_H = 0x8080808080808080ULL;

// adds all legal pawn moves to ml
void add_pawn_moves(chessboard* board, movelist* ml, bool white_to_move);

// adds all legal knight moves to ml
void add_knight_moves(chessboard* board, movelist* ml, bool white_to_move);

#endif /* MOVEGEN_H */