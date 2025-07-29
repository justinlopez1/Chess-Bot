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
    uint8_t delta;
    bool left_shift;
    uint64_t mask;
} shift;

static inline uint64_t shift_bitboard(const uint64_t bitboard, shift* shift) {
    if (shift->left_shift) {
        return (bitboard << shift->delta) & shift->mask;
    }
    return (bitboard >> shift->delta) & shift->mask;
}

// adds all of a move type to the movelist object moves
void add_pawn_moves(chessboard* board, movelist* moves, bool white_to_move);
void add_knight_moves(chessboard* board, movelist* moves, bool white_to_move);
void add_king_moves(chessboard* board, movelist* moves, bool white_to_move);

// moves/shifts for the knight that are iterable
static const shift knight_moveset[8] = {
    { 17, ~(FILE_A) },
    { 15, ~(FILE_H) },
    { 10, ~(FILE_A|FILE_B) },
    { 6, ~(FILE_G|FILE_H) },
    { -15 , ~(FILE_A) },
    { -17, ~(FILE_H) },
    { -6, ~(FILE_A|FILE_B) },
    { -10, ~(FILE_G|FILE_H) }
};

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

#endif /* MOVEGEN_H */