#include <iostream>

#include "RenjuSGFParser.h"

using namespace std;

int main()
{
  GameInfo game_info;
  game_info.Parse("ahkyeok-wzq-200310070155.sgf");
  
  return 0;
}