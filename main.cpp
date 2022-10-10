#include <iostream>
#include "search.h"
#include <algorithm>
#include  <cstring>
#include <fstream>

void play_game()
{
     Board b;
     int score;
     int k=0;
     int player = 0, with_computer = 0;
     std::cout << "Enter 1 to play vs computer, 0 for 2 players\n";
     std::cin >> with_computer;
     if(with_computer)
     {
          std::cout << "Enter 0 to play first, 1 otherwise\n";
          std::cin >> player;
     }
     k = player;
     b.print();
     while(!b.has_gameover())
     {
          Bitboard empty_squares = ~(b.bitboards[PLAYER_O] | b.bitboards[PLAYER_X]);
          int num_of_move  = popcount(empty_squares);
          if(!num_of_move)
               break;
          if(with_computer && k % 2)
          {
               std::cout << "Computer is thinking... \n";
               std::cout.flush();
               b.clear_for_search();
               score = AlphaBeta(b, -INF, INF, 12);
               std::cout << "info depth " << 12 << " nodes " << b.nodes << " score " << score <<" bestmove " << (int)b.bestmove << "\n";
               std::cout.flush();
               b.make_move(b.bestmove);
          }
          else
          {
               int x; 
               while(1)
               {
                    std::cout << "Enter a square [0 , 63]: \n";
                    std::cout.flush();
                    std::cin >> x;
                    if(check_bit(empty_squares , x) && x >= 0 && x <=63 )
                         break;
                    else 
                         throw std::invalid_argument( "received invalid input\n" );
                    
               }
               Move m = x;
               b.make_move(m);
          }
          system("clear\n");
          b.print();
          k++;
     }
     if(b.has_gameover())
     {
          if( b.side == player)
               std::cout <<"You lost\n";
          else
               std::cout <<"You won\n";
     }
     else 
     {
          std::cout<<"A draw\n";
     }
     std::cout.flush();

}
int main()
{   
     play_game();
     return 0;
} 