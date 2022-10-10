#ifndef SEARCH_H_
#define SEARCH_H_

#include "board.h"
#include "bitboard.h"
extern int nodes;
int16_t AlphaBeta(Board& b ,int16_t alpha, int16_t beta, int depth);
#endif