#include <iostream>
#include <fstream>
#include <iterator>

#include <boost/regex.hpp>
#include <boost/lexical_cast.hpp>

#include "RenjuSGFParser.h"

using namespace std;
using namespace boost;

GameInfo::GameInfo()
: game_end_status_(kUnknownEndStatus), game_result_(kUnknownResult)
{
}

bool GameInfo::Parse(const string &file_name)
{
  // SGFファイルを読み込む
  ifstream sgf_file(file_name);

  if(sgf_file.fail()){
    cerr << "Failed to open the file: " << file_name << endl;
    return false;
  }

  istreambuf_iterator<char> start(sgf_file);
  istreambuf_iterator<char> last;
  string sgf_data(start, last);

  try{
    game_date_ = ParseGameDate(sgf_data);
    black_player_name_ = ParseBlackPlayerName(sgf_data);
    black_player_rank_ = ParseBlackPlayerRank(sgf_data); 
    white_player_name_ = ParseWhitePlayerName(sgf_data);
    white_player_rank_ = ParseWhitePlayerRank(sgf_data);

    game_rule_ = ParseGameRule(sgf_data);
    game_end_status_ = ParseGameEndStatus(sgf_data);
    game_result_ = ParseGameResult(sgf_data);
    diagram_ = ParseDiagram(sgf_data);
    alternative_moves_ = ParseAlternativeMoves(sgf_data);
  }catch(logic_error &ex){
    cerr << ex.what() << endl;
    return false;
  }

  return true;
}

string GameInfo::ParseGameDate(const std::string &sgf_data) const
{
  regex date_expr("DT\\[(.*?)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), date_expr);
  sregex_iterator it_end;

  if(it != it_end){
    return it->str(1);
  }else{
    logic_error error("Date(DT) is not found.");
    throw error;
  }
}

string GameInfo::ParseBlackPlayerName(const std::string &sgf_data) const
{
  regex black_player_name_expr("PB\\[(.*?)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), black_player_name_expr);
  sregex_iterator it_end;

  if(it != it_end){
    return it->str(1);
  }else{
    logic_error error("Black Player Name(PB) is not found.");
    throw error;
  }
}

string GameInfo::ParseBlackPlayerRank(const std::string &sgf_data) const
{
  regex black_player_rank_expr("BR\\[(.*?)\\r?\\n?\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), black_player_rank_expr);
  sregex_iterator it_end;

  if(it != it_end){
    return it->str(1);
  }else{
    logic_error error("Black Player Rank(BR) is not found.");
    throw error;
  }
}

string GameInfo::ParseWhitePlayerName(const std::string &sgf_data) const
{
  regex white_player_name_expr("PW\\[(.*?)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), white_player_name_expr);
  sregex_iterator it_end;

  if(it != it_end){
    return it->str(1);
  }else{
    logic_error error("White Player Name(PW) is not found.");
    throw error;
  }
}

string GameInfo::ParseWhitePlayerRank(const std::string &sgf_data) const
{
  regex white_player_rank_expr("WR\\[(.*?)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), white_player_rank_expr);
  sregex_iterator it_end;

  if(it != it_end){
    return it->str(1);
  }else{
    logic_error error("White Player Rank(WR) is not found.");
    throw error;
  }
}

string GameInfo::ParseGameRule(const std::string &sgf_data) const
{
  regex game_rule_expr("GN\\[.*?((RIF)|(Sakata)|(Yamaguchi)|(Tarannikov)|(Jonsson)|(Unknown)).*?\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), game_rule_expr);
  sregex_iterator it_end;

  if(it != it_end){
    return it->str(1);
  }else{
    logic_error error("Game Rule(GN) is not found.");
    throw error;
  }
}

GameEndStatus GameInfo::ParseGameEndStatus(const std::string &sgf_data) const
{
  regex result_expr("RE\\[(B|W)\\+(Resign|Time|Draw)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), result_expr);
  sregex_iterator it_end;

  if(it != it_end){
    const string end_status = it->str(2);

    if(end_status == "Resign"){
      return kResign;
    }else if(end_status == "Time"){
      return kTimeup;
    }else{
      return kAgreedDraw;
    }
  }else{
    logic_error error("Game result(RE) is not found.");
    throw error;
  }
}

GameResult GameInfo::ParseGameResult(const std::string &sgf_data) const
{
  regex result_expr("RE\\[(B|W)\\+(Resign|Time|Draw)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), result_expr);
  sregex_iterator it_end;

  if(it != it_end){
    const string win_player = it->str(1);
    const string end_status = it->str(2);

    cerr << end_status << endl;
    const bool is_draw = end_status == "Draw";
    
    if(is_draw){
      return kDraw;
    }

    if(win_player == "B"){
      return kBlackWin;
    }else{
      return kWhiteWin;
    }
  }else{
    logic_error error("Game result(RE) is not found.");
    throw error;
  }
}

string GameInfo::ParseDiagram(const std::string &sgf_data) const
{
  regex black_move_expr(";B\\[(..)\\]");
  regex white_move_expr(";W\\[(..)\\]");

  sregex_iterator black_move_it(sgf_data.begin(), sgf_data.end(), black_move_expr);
  sregex_iterator white_move_it(sgf_data.begin(), sgf_data.end(), white_move_expr);
  sregex_iterator it_end;
  
  set<string> move_set;
  bool black_turn = true;
  string diagram;

  while(black_move_it != it_end || white_move_it != it_end)
  {
    const string stone = black_turn ? black_move_it->str(1) : white_move_it->str(1);
    
    // timeup
    if(stone == "tt"){
      break;
    }

    bool is_legal_move = stone.at(0) >= 'a' && stone.at(0) <= 'o';
    is_legal_move &= stone.at(1) >= 'a' && stone.at(1) <= 'o';

    if(!is_legal_move){
      const string error_message = "Illegal move: " + stone;
      logic_error error(error_message);
      throw error;
    }

    // 同じ場所への着手がある場合が稀にあるのでチェック
    set<string>::const_iterator find_it = move_set.find(stone);

    if(find_it != move_set.end()){
      const string error_message = "Conflict move: " + stone;
      logic_error error(error_message);
      throw error;
    }

    diagram += stone;

    if(black_turn){
      ++black_move_it;
    }else{
      ++white_move_it;
    }

    black_turn = !black_turn;
  }

  return diagram;
}

string GameInfo::ParseAlternativeMoves(const std::string &sgf_data) const
{
  regex alternative_expr("5A\\[(.*?)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), alternative_expr);
  sregex_iterator it_end;

  if(it != it_end){
    return it->str(1);
  }else{
    logic_error error("Alternative move(5A) is not found.");
    throw error;
  }
}
