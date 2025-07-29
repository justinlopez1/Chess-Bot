#ifndef MOVEGEN_H
#define MOVEGEN_H

#include <stdbool.h>

#include "movelist.h"
#include "chessmove.h"
#include "chessboard.h"
#include "macros.h"

// a struct that represents the info needed to shift pieces on a chessboard
// shifted by delta amount, and applied with mask after
// delta must be positive, l/r shift specificied by left_shift bool
typedef struct {
    int8_t delta;
    uint64_t mask;
} shift;

static inline uint64_t shift_bitboard(const uint64_t bitboard, const shift* shift) {
    if (shift->delta >= 0) {
        return (bitboard << shift->delta) & shift->mask;
    }
    return (bitboard >> shift->delta) & shift->mask;
}

// adds all of a move type to the movelist object moves
void add_pawn_moves(chessboard* board, movelist* moves, bool white_to_move);
void add_knight_moves(chessboard* board, movelist* moves, bool white_to_move);
void add_king_moves(chessboard* board, movelist* moves, bool white_to_move);

// moves/shifts for the knight that are iterable
static const shift KNIGHT_MOVESET[8] = {
    { 17, ~(FILE_A) },
    { 15, ~(FILE_H) },
    { 10, ~(FILE_A|FILE_B) },
    { 6, ~(FILE_G|FILE_H) },
    { -15, ~(FILE_A) },
    { -17, ~(FILE_H) },
    { -6, ~(FILE_A|FILE_B) },
    { -10, ~(FILE_G|FILE_H) }
};
static const uint8_t TOTAL_KNIGHT_MOVES = 8;

// moves/shifts for the king that are iterable, simply goes in all 8 nearby directions
static const shift KING_MOVESET[8] = {
    { 8, ~(0) },
    { 9, ~(FILE_A) },
    { 1, ~(FILE_A) },
    { -7, ~(FILE_A) },
    { -8, ~(0) },
    { -9, ~(FILE_H) },
    { -1, ~(FILE_H) },
    { 7, ~(FILE_H) }
};
static const uint8_t TOTAL_KING_MOVES = 8;

#endif /* MOVEGEN_H */