#include <iostream>
#include <fstream>
#include <iterator>
#include <array>
#include <set>
#include <regex>

#include "RenjuSGFParser.h"

using namespace std;

void ParseSGFFile(const string &file_path)
{
  // SGFファイルを読み込む
  ifstream sgf_file(file_path);

  if(sgf_file.fail()){
    cerr << "Failed to open the file: " << file_path << endl;
    return;
  }

  istreambuf_iterator<char> start(sgf_file);
  istreambuf_iterator<char> last;
  string sgf_file_data(start, last);

  // SGFデータ(FF[4]->B[..]->かっこが閉じられるまで)を抽出する
  static regex sgf_data_expr("(FF\\[4\\][\\s\\S]*?;B\\[..\\][\\s\\S]*?)\\)");
  sregex_iterator it(sgf_file_data.begin(), sgf_file_data.end(), sgf_data_expr);
  sregex_iterator it_end;

  while(it != it_end){
    const string sgf_data = it->str(1);
    GameInfo game_info;
    
    try{
      game_info.Parse(sgf_data);
      cout << game_info.str() << endl;
    }catch(logic_error &ex){
      cerr << file_path << "," << ex.what() << "," << sgf_data << endl;
    }

    ++it;
  }
}

GameInfo::GameInfo()
: game_end_status_(kUnknownEndStatus), game_result_(kUnknownResult)
{
}

void GameInfo::Parse(const string &sgf_data)
{
  try{
    game_date_ = ParseGameDate(sgf_data);
    event_name_ = ParseEventName(sgf_data);
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
    throw ex;
  }
}

string GameInfo::ParseGameDate(const std::string &sgf_data) const
{
  static regex date_expr("DT\\[(.*?)\\]");
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
  static regex black_player_name_expr("PB\\[(.*?)\\]");
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
  static regex black_player_rank_expr("BR\\[(.*?)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), black_player_rank_expr);
  sregex_iterator it_end;

  if(it != it_end){
    return it->str(1);
  }else{
    return "";
  }
}

string GameInfo::ParseWhitePlayerName(const std::string &sgf_data) const
{
  static regex white_player_name_expr("PW\\[(.*?)\\]");
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
  static regex white_player_rank_expr("WR\\[(.*?)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), white_player_rank_expr);
  sregex_iterator it_end;

  if(it != it_end){
    return it->str(1);
  }else{
    return "";
  }
}

string GameInfo::ParseGameRule(const std::string &sgf_data) const
{
  static regex game_rule_expr("(GN|RU)\\[.*?((RIF)|(Sakata)|(Yamaguchi)|(Tarannikov)|(Jonsson)|(Unknown)).*?\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), game_rule_expr);
  sregex_iterator it_end;

  if(it != it_end){
    return it->str(2);
  }else{
    return "Unknown";
  }
}

GameEndStatus GameInfo::ParseGameEndStatus(const std::string &sgf_data) const
{
  static regex draw_expr("RE\\[B?W?\\+?Draw\\]");
  sregex_iterator it_draw(sgf_data.begin(), sgf_data.end(), draw_expr);
  sregex_iterator it_end;

  if(it_draw != it_end){
    return kAgreedDraw;
  }

  static regex result_expr("RE\\[(B|W)\\+(Resign|R|Time)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), result_expr);

  if(it != it_end){
    const string end_status = it->str(2);

    if(end_status == "Resign" || end_status == "R"){
      return kResign;
    }else{
      return kTimeup;
    }
  }else{
    logic_error error("Game result(RE) is not found.");
    throw error;
  }
}

GameResult GameInfo::ParseGameResult(const std::string &sgf_data) const
{
  if(ParseGameEndStatus(sgf_data) == kAgreedDraw){
    return kDraw;
  }

  static regex result_expr("RE\\[(B|W)\\+(Resign|R|Time)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), result_expr);
  sregex_iterator it_end;

  if(it != it_end){
    const string win_player = it->str(1);

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
  static regex move_expr(";(B|W)\\[([a-z][a-z])\\]");

  sregex_iterator move_it(sgf_data.begin(), sgf_data.end(), move_expr);
  sregex_iterator it_end;
  
  set<string> move_set;
  bool black_turn = true;
  string diagram;

  while(move_it != it_end)
  {
    const string turn = move_it->str(1);
    const string stone = move_it->str(2);
    
    // 黒番、白番が交互に出現しているかチェックする
    bool is_turn_consistent = (black_turn && turn == "B") || (!black_turn && turn == "W");

    if(!is_turn_consistent){
      throw logic_error("Turn is not consistent.");
    }

    // timeup or pass
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

    ++move_it;
    black_turn = !black_turn;
  }

  return diagram;
}

string GameInfo::ParseAlternativeMoves(const std::string &sgf_data) const
{
  static regex alternative_expr("(5A|FA)\\[([a-o0-9\\s\\[\\]]*)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), alternative_expr);
  sregex_iterator it_end;

  if(it != it_end){
    string alternative_move = it->str(2);

    replace(alternative_move.begin(), alternative_move.end(), '[', ' ');
    replace(alternative_move.begin(), alternative_move.end(), ']', ';');

    return alternative_move;
  }else{
    return "";
  }
}

string GameInfo::ParseEventName(const std::string &sgf_data) const
{
  static regex event_expr("EV\\[(.*?)\\]");
  sregex_iterator it(sgf_data.begin(), sgf_data.end(), event_expr);
  sregex_iterator it_end;

  if(it != it_end){
    return it->str(1);
  }else{
    return "";
  }
}

string GameInfo::GetCSVHeader() const
{
  string header;

  header += "game_date,";
  header += "event,";

  header += "black_player,";
  header += "black_rank,";
  header += "white_player,";
  header += "white_rank,";

  header += "opening_rule,";

  header += "game_end_status,";
  header += "game_result,";

  header += "diagram,";
  header += "alternative_moves";

  return header;
}

string GameInfo::ReplaceComma(const std::string &str) const
{
  string replaced_str = str;
  replace(replaced_str.begin(), replaced_str.end(), ',', ' ');

  return replaced_str;
}

string GameInfo::str() const
{
  string info_text;
  std::array<string, 4> game_end_status_str{{"Resign", "Timeup", "AgreedDraw", "Unknown"}};
  std::array<string, 4> game_result_str{{"BlackWin", "WhiteWin", "Draw", "Unknown"}};

  info_text += ReplaceComma(game_date_) + ",";
  info_text += ReplaceComma(event_name_) + ",";
  
  info_text += ReplaceComma(black_player_name_) + ",";
  info_text += ReplaceComma(black_player_rank_) + ",";
  info_text += ReplaceComma(white_player_name_) + ",";
  info_text += ReplaceComma(white_player_rank_) + ",";

  info_text += ReplaceComma(game_rule_) + ",";

  info_text += ReplaceComma(game_end_status_str[game_end_status_]) + ",";
  info_text += ReplaceComma(game_result_str[game_result_]) + ",";

  info_text += ReplaceComma(diagram_) + ",";
  info_text += ReplaceComma(alternative_moves_);

  return info_text;
}
