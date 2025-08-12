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
    return (bitboard >> -shift->delta) & shift->mask;
}

// adds all of a move type to the movelist object moves
void add_pawn_moves(chessboard* board, movelist* moves, bool white_to_move);
void add_nonslider_moves(chessboard* board, movelist* moves, bool white_to_move, chesspiece piece, const shift* shiftset, uint8_t total_shifts);
void add_slider_moves(chessboard* board, movelist* moves, bool white_to_move, chesspiece piece, const shift* shiftset, uint8_t total_shifts);

void add_castle_moves(chessboard* board, movelist* moves, bool white_to_move);
// pieces are the correct color's rooks and king
void add_queenside_castle(chessboard* board, movelist* moves, uint64_t pieces, bool white_to_move);
void add_kingside_castle(chessboard* board, movelist* moves, uint64_t pieces, bool white_to_move);

// calls all the above functions to add all legal moves to a movelist
void add_legal_moves(chessboard* board, movelist* moves, bool white_to_move);

// returns bitboard of attacked currently attacked squares, used to check if board is in check
uint64_t get_pawn_attacked_bitboard(const chessboard* board, bool white_to_move);
uint64_t get_nonslider_attacked_bitboard(const chessboard* board, bool white_to_move, chesspiece piece, const shift* shiftset, uint8_t total_shifts);
uint64_t get_slider_attacked_bitboard(const chessboard* board, bool white_to_move, chesspiece piece, const shift* shiftset, uint8_t total_shifts);


// moves/shifts for the knight that are iterable
static const shift KNIGHT_SHIFTSET[8] = {
    { 17, ~(FILE_A) }, // nne
    { 15, ~(FILE_H) }, // nnw
    { 10, ~(FILE_A|FILE_B) }, // nee
    { 6, ~(FILE_G|FILE_H) }, // nww
    { -15, ~(FILE_A) }, // sse
    { -17, ~(FILE_H) }, // ssw
    { -6, ~(FILE_A|FILE_B) }, // see
    { -10, ~(FILE_G|FILE_H) } // sww
};
static const uint8_t TOTAL_KNIGHT_SHIFTS = 8;

// moves/shifts for the king that are iterable, simply goes in all 8 nearby directions
// same for queen because the queen moves the same way, just can repeatedly shift so it will
// be generated using slider logic
static const shift KING_QUEEN_SHIFTSET[8] = {
    { 8, ~(0) }, // n
    { 9, ~(FILE_A) }, // ne
    { 1, ~(FILE_A) }, // e
    { -7, ~(FILE_A) }, // se
    { -8, ~(0) }, // s
    { -9, ~(FILE_H) }, // sw
    { -1, ~(FILE_H) }, // w
    { 7, ~(FILE_H) } // nw
};
static const uint8_t TOTAL_KING_QUEEN_SHIFTS = 8;

static const shift ROOK_SINGLE_SHIFTS[4] = {
    { 8, ~(0) }, // n
    { 1, ~(FILE_A) }, // e
    { -8, ~(0) }, // s
    { -1, ~(FILE_H) } // w
};
static const uint8_t TOTAL_ROOK_SHIFTS = 4;

static const shift BISHOP_SINGLE_SHIFTS[4] = {
    { 9, ~(FILE_A) }, // ne
    { -7, ~(FILE_A) }, // se
    { -9, ~(FILE_H) }, // sw
    { 7, ~(FILE_H) } // nw
};
static const uint8_t TOTAL_BISHOP_SHIFTS = 4;

static const shift SHIFT_N = { 8, ~(0) }; // n
static const shift SHIFT_NE = { 9, ~(FILE_A) }; // ne
static const shift SHIFT_E = { 1, ~(FILE_A) };// e
static const shift SHIFT_SE = { -7, ~(FILE_A) }; // se
static const shift SHIFT_S = { -8, ~(0) }; // s
static const shift SHIFT_SW = { -9, ~(FILE_H) }; // sw
static const shift SHIFT_W = { -1, ~(FILE_H) }; // w
static const shift SHIFT_NW = { 7, ~(FILE_H) }; // nw

#endif /* MOVEGEN_H */