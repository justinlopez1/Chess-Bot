#ifndef MACROS_H
#define MACROS_H

#define GET_BIT(bitboard, index) (bitboard & 1ULL << index)
#define SET_BIT(bitboard, index) (bitboard |= 1ULL << index)
#define CLEAR_BIT(bitboard, index) (bitboard &= !(!1ULL << index))

#endif /* MACROS_H */