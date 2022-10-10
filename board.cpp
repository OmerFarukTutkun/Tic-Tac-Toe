#include "board.h"
#include "cstring"
#include <iostream>

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


const unsigned int square_table[64] = {
0, 1, 2, 3, 3, 2, 1, 0,
1, 2, 3, 4, 4, 3, 2, 1,
2, 3, 4, 5, 5, 4, 3, 2,
3, 4, 5, 6, 6, 5, 4, 3,
3, 4, 5, 6, 6, 5, 4, 3,
2, 3, 4, 5, 5, 4, 3, 2,
1, 2, 3, 4, 4, 3, 2, 1,
0, 1, 2, 3, 3, 2, 1, 0,
};
Board::Board()
{
    ply = 0;
    nodes = 0;
    key = 184744073709551615;
    side = PLAYER_X;
    bitboards[PLAYER_O] = 0ull;
    bitboards[PLAYER_X] = 0ull;
}
void Board::make_move(Move move)
{
    nodes++;
    set_bit(bitboards[side] , move);
    move_stack[ply] = move;
    ply++;
    side = 1 - side;
}
void Board::unmake_move()
{
    ply--;
    side = 1 - side;
    Move move = move_stack[ply];
    clear_bit(bitboards[side] , move);
}
void Board::print()
{
    if(side)
        printf("O to move\n");
    else    
        printf("X to move\n");
    Bitboard occupied = bitboards[PLAYER_X] | bitboards[PLAYER_O];
    for(Square sq=0; sq < board_size*board_size ; sq++)
    {
        if(sq % 8 == 0)
        {
            printf(RED "|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|\n" RESET);
        }
        if(check_bit(occupied ,sq))
        {
            int player = check_bit(bitboards[PLAYER_O] , sq); 
            if(player)
                printf(RED "|     " YELLOW "O     " RESET);
            else
                printf(RED "|     " BLUE "X     " RESET);
        }
        else
        {
            printf( RED "|           " RESET);
        }
        if( sq % 8 == 7)
        {
            printf(RED "|\n" RESET);
            for(int i=sq-7; i <=sq ; i++)
               printf( RED "|       " RESET " %2d " , i);
            printf(RED "|\n" RESET);
        }
    }
    printf(RED "|-----------|-----------|-----------|-----------|-----------|-----------|-----------|-----------|\n\n\n" RESET);
    fflush(stdout);
}
bool Board::has_gameover()
{
    Bitboard x = bitboards[1 - side];
    Bitboard x1 = x  & ~FILE_H;
    Bitboard x2 = x1 & ~FILE_G;
    Bitboard x3 = x2 & ~FILE_F;

    Bitboard x4 = x  & ~FILE_A;
    Bitboard x5 = x4 & ~FILE_B;
    Bitboard x6 = x5 & ~FILE_C;

    //horizontal
    if( x & (x4 >> 1) & (x5 >> 2) & (x6 >> 3))
        return true;
    //vertical
    if( x & (x >> 8) & (x >> 16) & (x >> 24))
        return true;

    //diaogonal
    if( x & (x4 >> 9) & (x5 >> 18) & (x6 >> 27))
        return true;
    if( x & (x1 >> 7) & (x2 >> 14) & (x3 >> 21))
        return true;

    return false;
}
void Board::clear_for_search()
{
    nodes = 0;
    ply = 0;
    for(int i=0 ; i<MAX_DEPTH ; i++)    
    {
        killers[i][0] = killers[i][1] = NoMove;
    }
    for(int i=0 ; i<MAX_DEPTH ; i++)    
    {
       counterMove[i] = NoMove;
    }
}
int16_t eval_one_side(Board& b , int side)
{
    //enemy
    Bitboard y = b.bitboards[1 -side] ;
    //us
    Bitboard x  = b.bitboards[side];

    Bitboard x1 = x  & ~FILE_H;
    Bitboard x2 = x1 & ~FILE_G;

    Bitboard x4 = x  & ~FILE_A;
    Bitboard x5 = x4 & ~FILE_B;

    Bitboard y1 = y  & ~FILE_H;
    Bitboard y4 = y  & ~FILE_A;

    int16_t score = 0;

    if( x & (x4 >> 1) & (~y4 >> 1) & (~y1 << 1));
        score +=200;
    if( x & (x  >> 8) & (~y  >> 8) & (~y  << 8));
        score +=200;
    if( x & (x1 >> 7) & (~y1 >> 7) & (~y4 <<7 ));
        score +=200;
    if( x & (x4 >> 9) & (~y4 >> 9) & (~y1 <<9 ));
         score +=200;

    if( x & (x4 >> 1) & (x5 >> 2)   & (~y4 >> 1) & (~y1 << 1));
        score +=1000;
    if( x & (x >> 8)  & (x >> 16)   & (~y  >> 8) & (~y  << 8));
        score +=1000;
    if( x & (x1 >> 7) & (x2 >> 14)  & (~y1 >> 7) & (~y4 <<7 ));
        score +=1000;
    if( x & (x4 >> 9) & (x5 >> 18)  & (~y4 >> 9) & (~y1 <<9 ));
        score +=1000;
    while(x)
    {
        Square sq = poplsb(x);
        score += 100*square_table[sq];
    }
    return score;
}
int16_t Board::eval()
{
    return eval_one_side(*this , side) - eval_one_side(*this, 1- side);
}