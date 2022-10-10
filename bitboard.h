#ifndef _BITBOARD_H_
#define _BITBOARD_H_

#include "stdint.h"
using Bitboard = uint64_t;
constexpr Bitboard zero_bitboard = 0ull;
using Square   = int8_t;
constexpr uint64_t ONE = 1ULL;
enum {
    RANK_1 = 0x00000000000000FFULL,
    RANK_2 = 0x000000000000FF00ULL,
    RANK_3 = 0x0000000000FF0000ULL,
    RANK_4 = 0x00000000FF000000ULL,
    RANK_5 = 0x000000FF00000000ULL,
    RANK_6 = 0x0000FF0000000000ULL,
    RANK_7 = 0x00FF000000000000ULL,
    RANK_8 = 0xFF00000000000000ULL,

    FILE_A = 0x0101010101010101ULL,
    FILE_B = 0x0202020202020202ULL,
    FILE_C = 0x0404040404040404ULL,
    FILE_D = 0x0808080808080808ULL,
    FILE_E = 0x1010101010101010ULL,
    FILE_F = 0x2020202020202020ULL,
    FILE_G = 0x4040404040404040ULL,
    FILE_H = 0x8080808080808080ULL
};

void set_bit(Bitboard& bb, Square sq);
void clear_bit(Bitboard& bb, Square sq);
bool check_bit(Bitboard bb, Square sq);
int bitScanForward(Bitboard bb);
int poplsb(Bitboard& bb);

void print_bitboard(Bitboard bb);
int popcount(Bitboard bb);

#endif