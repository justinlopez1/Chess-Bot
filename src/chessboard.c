#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "chessboard.h"
#include "movegen.h"

void chessboard_init(chessboard * board, uint64_t * data) {
    board->kings = data[0];
    board->queens = data[1];
    board->bishops = data[2];
    board->knights = data[3];
    board->rooks = data[4];
    board->pawns = data[5];
    board->white_pieces = data[6];
    board->black_pieces = data[7];
    board->unmoved_pieces_castle = board->kings | board->rooks;
    board->en_pessant_index = NO_EN_PESSANT;
}

// the 16*8 comes from a single line being printed = 16 charaacters,
// 1 1 1 1 1 1 1 1\n = 16 characters including spaces and newline
// and we need 8 lines printed
void bitboard_print(const uint64_t bitboard, const char* title) {

    printf("%s\n", title);

    char buf[(8+8+3)*(8+1)+3+3];
    uint8_t idx = 0;

    char ranks[8] = {
        '1', '2', '3', '4', '5', '6', '7', '8'
    };
    char files[15] = {
        'a', ' ', 'b', ' ', 'c', ' ', 'd', 
        ' ', 'e', ' ', 'f', ' ', 'g', ' ', 'h'
    };

    for (int rank = 7; rank >= 0; rank--) {
        // put in rank label then two spaces
        buf[idx++] = ranks[rank];
        buf[idx++] = ' ';
        buf[idx++] = ' ';
        for (int file = 0; file < 8; file++) {

            buf[idx++] = GET_BIT(bitboard, rank*8 + file) ? '1' : '0';

            if (file < 7) buf[idx++] = ' ';
        }
        buf[idx++] = '\n';
    }
    buf[idx++] = ' ';
    buf[idx++] = ' ';
    buf[idx++] = ' ';

    memcpy(buf+idx, files, 15);
    idx += 15;

    // null terminate the string
    buf[idx++] = '\n';
    buf[idx++] = '\n';
    buf[idx] = '\0';
    printf("%s", buf);
}

void chessboard_print(const chessboard* board) {
    char buf[(8+8+3)*(8+1)+3+3];
    uint8_t idx = 0;

    char ranks[8] = {
        '1', '2', '3', '4', '5', '6', '7', '8'
    };
    char files[15] = {
        'a', ' ', 'b', ' ', 'c', ' ', 'd', 
        ' ', 'e', ' ', 'f', ' ', 'g', ' ', 'h'
    };

    for (int rank = 7; rank >= 0; rank--) {
        // put in rank label then two spaces
        buf[idx++] = ranks[rank];
        buf[idx++] = ' ';
        buf[idx++] = ' ';
        for (int file = 0; file < 8; file++) {

            chesspiece piece = get_chesspiece_at(board, rank*8 + file);
            buf[idx++] = chesspiece_to_char(piece); 

            if (file < 7) buf[idx++] = ' ';
        }
        buf[idx++] = '\n';
    }
    buf[idx++] = ' ';
    buf[idx++] = ' ';
    buf[idx++] = ' ';

    memcpy(buf+idx, files, 15);
    idx += 15;

    // null terminate the string
    buf[idx++] = '\n';
    buf[idx++] = '\n';
    buf[idx] = '\0';
    printf("%s", buf);
}


bool in_check(const chessboard* board, bool white_to_move) {
    // first get position (bitboard) of king
    uint64_t our_king = board->kings;
    if (white_to_move) { our_king &= board->white_pieces; }
    else { our_king &= board->black_pieces; }

    // generate attacking moves and check if our king is on any of the attacking squares
    if (our_king & get_pawn_attacked_bitboard(board, !white_to_move)) { return true; }
    if (our_king & get_slider_attacked_bitboard(board, !white_to_move, CHESSPIECE_QUEEN, KING_QUEEN_SHIFTSET, TOTAL_KING_QUEEN_SHIFTS)) { return true; }
    if (our_king & get_slider_attacked_bitboard(board, !white_to_move, CHESSPIECE_BISHOP, BISHOP_SINGLE_SHIFTS, TOTAL_BISHOP_SHIFTS)) { return true; }
    if (our_king & get_slider_attacked_bitboard(board, !white_to_move, CHESSPIECE_ROOK, ROOK_SINGLE_SHIFTS, TOTAL_ROOK_SHIFTS)) { return true; }
    if (our_king & get_nonslider_attacked_bitboard(board, !white_to_move, CHESSPIECE_KNIGHT, KNIGHT_SHIFTSET, TOTAL_KNIGHT_SHIFTS)) { return true; }
    if (our_king & get_nonslider_attacked_bitboard(board, !white_to_move, CHESSPIECE_KING, KING_QUEEN_SHIFTSET, TOTAL_KING_QUEEN_SHIFTS)) { return true; }

    return false; 
}

bool is_square_attacked(uint8_t index, const chessboard *board, bool white_attacking) {
    // first get position (bitboard) of king
    uint64_t pieces = 0;
    SET_BIT(pieces, index);

    // generate attacking moves and check if our king is on any of the attacking squares
    if (pieces & get_pawn_attacked_bitboard(board, white_attacking)) { return true; }
    if (pieces & get_slider_attacked_bitboard(board, white_attacking, CHESSPIECE_QUEEN, KING_QUEEN_SHIFTSET, TOTAL_KING_QUEEN_SHIFTS)) { return true; }
    if (pieces & get_slider_attacked_bitboard(board, white_attacking, CHESSPIECE_BISHOP, BISHOP_SINGLE_SHIFTS, TOTAL_BISHOP_SHIFTS)) { return true; }
    if (pieces & get_slider_attacked_bitboard(board, white_attacking, CHESSPIECE_ROOK, ROOK_SINGLE_SHIFTS, TOTAL_ROOK_SHIFTS)) { return true; }
    if (pieces & get_nonslider_attacked_bitboard(board, white_attacking, CHESSPIECE_KNIGHT, KNIGHT_SHIFTSET, TOTAL_KNIGHT_SHIFTS)) { return true; }
    if (pieces & get_nonslider_attacked_bitboard(board, white_attacking, CHESSPIECE_KING, KING_QUEEN_SHIFTSET, TOTAL_KING_QUEEN_SHIFTS)) { return true; }

    return false; 
}
