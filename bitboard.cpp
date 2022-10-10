#include "bitboard.h"
#include <iostream>
void set_bit(Bitboard& bb, Square sq)
{
    bb |= (ONE << sq);
}
void clear_bit(Bitboard& bb, Square sq)
{
    bb &= ~(ONE << sq);
}
bool check_bit(Bitboard bb, Square sq)
{
    return (bb >> sq) & ONE;
}

int bitScanForward(Bitboard bb)
{
    return __builtin_ctzll(bb);
}
int poplsb(Bitboard& bb)
{
    int lsb = bitScanForward(bb);
    bb &= bb - 1;
    return lsb;
}
void print_bitboard(Bitboard bb)
{
    for(int i = 0; i < 8 ; i++)
    {
        for(int j = 0; j < 8 ;j++)
        {
            std::cout << check_bit(bb , 8*i +j) << " ";
        }
        std::cout << std::endl;
    }
}
int popcount(Bitboard bb)
{
    return __builtin_popcountll(bb);
}
