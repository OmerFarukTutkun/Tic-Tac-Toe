#ifndef BOARD_H_
#define BOARD_H_
#include <cstdint>
#include <cassert>  
#include "bitboard.h"
constexpr int board_size = 8;
constexpr int MAX_DEPTH = board_size*board_size;
constexpr int WIN = 10000;
constexpr int INF = 11111;

using Piece = int8_t;
using Move = int8_t;
constexpr Move NoMove = -1;

enum Pieces{
    PLAYER_X = 0,
    PLAYER_O = 1,
    EMPTY = 2,
};

class Board{
    public:
        Bitboard bitboards[2]; // a bitboard for X and O
        Move move_stack[MAX_DEPTH];
        Move bestmove;
        Move killers[MAX_DEPTH][2];
        Move counterMove[MAX_DEPTH];
        int ply;
        int side;
        int nodes;
        uint64_t key;

        Board();
        void make_move(Move move);
        void print();
        void unmake_move();
        int16_t eval();
        bool has_gameover();
        void clear_for_search();
};
#endif

