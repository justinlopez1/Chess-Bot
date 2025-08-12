#include <stdio.h>
#include <stdbool.h>

#include "chessboard.h"
#include "movegen.h"
#include "chessmove.h"

int main() {

    uint64_t unmoved_start = 0;
    unmoved_start |= WHITE_KING_START_POS | BLACK_KING_START_POS | BLACK_QUEENSIDE_ROOK_START_POS |
                        WHITE_QUENSIDE_ROOK_START_POS | BLACK_KINGSIDE_ROOK_START_POS | WHITE_KINGSIDE_ROOK__START_POS;

    chessboard pawn_board = {
        // no other pieces
        .kings        = 0ULL,
        .queens       = 0ULL,
        .bishops      = 0ULL,
        .knights      = 0ULL,
        .rooks        = 0ULL,
        // pawns on c4, e4 (white) and b5, d5, f5 (black)
        .pawns        = (1ULL<<24)|(1ULL<<9),
        .white_pieces = (1ULL<<9),
        .black_pieces = (1ULL<<24),
        .en_pessant_index = NO_EN_PESSANT,
        .unmoved_pieces_castle = 0
    };

    chessboard knight_board = {
        // no kings, queens, bishops, rooks or pawns
        .kings        = 0ULL,
        .queens       = 0ULL,
        .bishops      = 0ULL,
        .rooks        = 0ULL,
        .pawns        = 0ULL,

        // one white knight on e4 (index 4 + 3*8 = 28)
        // .knights      = 1ULL << 28,
        .knights = 1ULL,

        // white_pieces includes that knight, black_pieces is empty
        .white_pieces = 0ULL,
        // .black_pieces = 1ULL << 28
        .black_pieces = 1ULL
    };


    chessboard board = {
        // piece‐type bitboards (both colors)
        .kings        = (1ULL<<4)  | (1ULL<<60),
        .queens       = (1ULL<<3)  | (1ULL<<59),
        .bishops      = (1ULL<<2)  | (1ULL<<5)  | (1ULL<<58) | (1ULL<<61),
        .knights      = (1ULL<<1)  | (1ULL<<6)  | (1ULL<<57) | (1ULL<<62),
        .rooks        = (1ULL<<0)  | (1ULL<<7)  | (1ULL<<56) | (1ULL<<63),
        .pawns        = 0x00FF00000000FF00ULL,
        // color bitboards
        .white_pieces = 0x000000000000FFFFULL,
        .black_pieces = 0xFFFF000000000000ULL
    };
    
    chessboard rook_board = {
        // no kings, queens, bishops, rooks or pawns
        .kings        = 0ULL,
        .queens       = 0ULL,
        .bishops      = 0ULL,
        .rooks        = 1ULL << 32 | 1ULL << 36,
        .pawns        = 0ULL,

        // one white knight on e4 (index 4 + 3*8 = 28)
        // .knights      = 1ULL << 28,
        .knights = 0ULL,

        // white_pieces includes that knight, black_pieces is empty
        .white_pieces = 1ULL << 36,
        // .black_pieces = 1ULL << 28
        .black_pieces = 1ULL << 32
    };

chessboard pinned_board = {
    .kings        = (1ULL << 0)  | (1ULL << 63),      // a1 (bit 0) + h8 (bit 63)
    .queens       = (1ULL << 36),                    // e5 (bit 36)
    .bishops      = 0ULL,
    .rooks        = (1ULL << 5),                     // f1 (bit 5)
    .knights      = 0ULL,
    .pawns        = (1ULL << 27),                    // e4 (bit 28)
    .white_pieces = (1ULL << 0) | (1ULL << 5) | (1ULL << 27),  // king, rook, pawn
    .black_pieces = (1ULL << 63) | (1ULL << 36)               // king, queen
};

chessboard promotion = {
    .kings        = (1ULL << 0)  | (1ULL << 63),      // a1 (bit 0) + h8 (bit 63)
    .queens       = (1ULL << 36),                    // e5 (bit 36)
    .bishops      = 0ULL,
    .rooks        = (1ULL << 5),                     // f1 (bit 5)
    .knights      = 0ULL,
    .pawns        = (1ULL << 27) | (1ULL << 48),                    // e4 (bit 28)
    .white_pieces = (1ULL << 0) | (1ULL << 5) | (1ULL << 27) | (1ULL << 48),  // king, rook, pawn
    .black_pieces = (1ULL << 63) | (1ULL << 36),               // king, queen
    .en_pessant_index = NO_EN_PESSANT
};

chessboard castle_qs_white_ok = {
        .kings        = (1ULL << 4)  | (1ULL << 63),  // e1 (white), h8 (black)
        .queens       = 0ULL,
        .bishops      = 0ULL,
        .knights      = 0ULL,
        .rooks        = (1ULL << 0),                  // a1 rook
        .pawns        = 0ULL,

        .white_pieces = (1ULL << 4) | (1ULL << 0),    // e1, a1
        .black_pieces = (1ULL << 63),                 // h8

        // make sure your struct has this field; it must include unmoved K and R
        .unmoved_pieces_castle = ( 1<< WHITE_KING_START_POS) | (1<<WHITE_QUENSIDE_ROOK_START_POS),
        .en_pessant_index = NO_EN_PESSANT
    };

    movelist ml;
    movelist_init(&ml);
    // add_pawn_moves(&board, &ml, false); 
    // add_knight_moves(&knight_board, &ml, false);
    // add_slider_moves(&rook_board, &ml, false, SLIDER_ROOK, ROOK_SINGLE_SHIFTS, TOTAL_ROOK_SHIFTS);

    // undo_chessmove u =  make_move(&board, move);
    // unmake_move(&board, u);

    chessmove move;
    move.from = 4;
    move.to = 2;
    move.type = CHESSMOVE_TYPE_CASTLE_QUEENSIDE;
    // make_move(&pawn_board, move);

    chessboard_print(&castle_qs_white_ok);
    
    add_legal_moves(&castle_qs_white_ok, &ml, true);

    chessboard_print(&castle_qs_white_ok);
   
    movelist_print(&ml);

    make_move(&castle_qs_white_ok, move);
    chessboard_print(&castle_qs_white_ok);

    /*
    add_legal_moves(&knight_board, &ml, false);
    chessboard_print(&knight_board);
    movelist_print(&ml);
    */

    return 0;
}