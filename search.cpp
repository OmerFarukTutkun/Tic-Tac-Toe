#include "search.h"
#include <math.h> 
#include <iostream>
int nodes = 0;
const Square best_moves[64] = {
36, 35, 28, 27, 44, 43, 37, 34, 
29, 26, 20, 19, 52, 51, 45, 42, 
38, 33, 30, 25, 21, 18, 12, 11, 
60, 59, 53, 50, 46, 41, 39, 32, 
31, 24, 22, 17, 13, 10, 4, 3, 
61, 58, 54, 49, 47, 40, 23, 16, 
14, 9, 5, 2, 62, 57, 55, 48, 15, 
8, 6, 1, 63, 56, 7, 0
};

int16_t AlphaBeta(Board& b ,int16_t alpha, int16_t beta, int depth)
{
    nodes++;
    bool over = b.has_gameover();
    if(over)
        return -WIN + b.ply;
    Bitboard occ = (b.bitboards[PLAYER_O] | b.bitboards[PLAYER_X]);
    Bitboard empty_squares = ~occ;

    if(empty_squares == 0ull)
        return 0;

    int16_t eval = b.eval();
    if(depth <=0 )
    {
        return eval;
    }

    //futility pruning
    if(( eval - depth*500 > beta || eval + 500*depth < alpha) )
        return eval;
    
    //Null move pruning
    if( eval > beta)
    {
        int R = 3 + (eval - beta)/500;
        b.side = 1 - b.side;
        b.ply++;

        int16_t value = -AlphaBeta(b, -(beta), -(beta -1), depth -R);

        b.side = 1 - b.side;
        b.ply--;

        if(value >= beta)
            return value;
    }

    int16_t bestscore = -INF ,score;
    int i=0 , played = 0 , oldalpha = alpha;
    Move move,bestmove;

    Move killer1 = NoMove; 
    Move killer2 = NoMove;
    Move countermove = NoMove;

    if(b.killers[b.ply][0] != NoMove && check_bit(empty_squares, b.killers[b.ply][0]))
        killer1 = b.killers[b.ply][0];
    if(b.killers[b.ply][1] != NoMove && check_bit(empty_squares, b.killers[b.ply][1]))
        killer2 = b.killers[b.ply][1];

    if(b.ply && b.counterMove[b.move_stack[b.ply]] != NoMove && check_bit(empty_squares, b.counterMove[b.move_stack[b.ply]]))
        countermove = b.counterMove[b.move_stack[b.ply]];

    int skips[3] ={ killer1 , killer2 , countermove};
    while(empty_squares && i <= 63)
    {
        if(killer1 != NoMove)
        {
            move = killer1;
            killer1 = NoMove;
        }
        else if(countermove != NoMove)
        {
            move = countermove;
            countermove = NoMove;
        }
        else if(killer2 != NoMove)
        {
            move = killer2;
            killer2 = NoMove;
        }
        else if(i == skips[0] || i == skips[1] || i == skips[2])
        {
            i++;
            continue;
        }
        else if(check_bit(empty_squares , best_moves[i]))
        {
            move = best_moves[i];
            clear_bit(empty_squares , best_moves[i]);
            i++;
        }
        else
        {
            i++;
            continue;
        }
        b.make_move(move);
        if(played < 10)
            score = -AlphaBeta(b , -beta , -alpha, depth -1);
        else
        {
            score = -AlphaBeta(b , -(beta) , -alpha, depth -1 - played/10);
            if(score > alpha)
                score = -AlphaBeta(b , -beta , -alpha, depth -1 );
        }
        b.unmake_move();
        played++;

        if(score > bestscore)
        {
            bestscore = score;
            bestmove = move;
            if(score > alpha)
                alpha = score;

            if(b.ply == 0)
            {
                b.bestmove = move;
            }
            if(bestscore >= beta)
            {
                //Killer move heuristic
                if(b.killers[b.ply][0] != move)
                {
                    b.killers[b.ply][1] = b.killers[b.ply][0];
                    b.killers[b.ply][0] = move;
                }
                b.counterMove[b.move_stack[b.ply]] = move;
                return bestscore;
            }
        }
    }
    return bestscore;
}