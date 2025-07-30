#ifndef MACROS_H
#define MACROS_H

#define GET_BIT(bitboard, index) (( (bitboard) >> (index) ) & 1ULL)
#define SET_BIT(bitboard, index) ( (bitboard) |= (1ULL << (index) ))
#define CLEAR_BIT(bitboard, index) ( (bitboard) &= ~(1ULL << (index) ))

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
enum {
    SHIFT_N_DELTA = 8,
    SHIFT_NE_DELTA = 9,
    SHIFT_E_DELTA = 1,
    SHIFT_SE_DELTA = -7,
    SHIFT_S_DELTA = -8,
    SHIFT_SW_DELTA = -9,
    SHIFT_W_DELTA = -1,
    SHIFT_NW_DELTA = 7
};


#endif /* MACROS_H */