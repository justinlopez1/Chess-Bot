#ifndef MACROS_H
#define MACROS_H

#define GET_BIT(bitboard, index) (( (bitboard) >> (index) ) & 1ULL)
#define SET_BIT(bitboard, index) ( (bitboard) |= (1ULL << (index) ))
#define CLEAR_BIT(bitboard, index) ( (bitboard) &= ~(1ULL << (index) ))

// shifts that move pieces one square in the 8 directions, protected by wrap around
#define SHIFT_N(bitboard) ( (bitboard) << 8)
#define SHIFT_NE(bitboard) (( (bitboard) << 9) & ~FILE_A)
#define SHIFT_E(bitboard) (( (bitboard) << 1) & ~FILE_A)
#define SHIFT_SE(bitboard) (( (bitboard) >> 7) & ~FILE_A)
#define SHIFT_S(bitboard) ( (bitboard) >> 8)
#define SHIFT_SW(bitboard) (( (bitboard) >> 9) & ~FILE_H)
#define SHIFT_W(bitboard) (( (bitboard) >> 1) & ~FILE_H)
#define SHIFT_NW(bitboard) (( (bitboard) << 7) & ~FILE_H)
// delta amounts of the shifts
#define SHIFT_N_DELTA  (8)
#define SHIFT_NE_DELTA (9)
#define SHIFT_E_DELTA  (1)
#define SHIFT_SE_DELTA (-7)
#define SHIFT_S_DELTA  (-8)
#define SHIFT_SW_DELTA (-9)
#define SHIFT_W_DELTA  (-1)
#define SHIFT_NW_DELTA (7)


#endif /* MACROS_H */