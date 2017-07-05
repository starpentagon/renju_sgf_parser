#include <iostream>
#include <fstream>
#include <iterator>
#include <array>
#include <set>
#include <regex>

#include "SGFParser.h"
#include "RenjuSGFParser.h"

using namespace std;
using namespace realcore;

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
    
    try{
      cout << GetCSVData(sgf_data) << endl;
    }catch(logic_error &ex){
      cerr << file_path << "," << ex.what() << "," << sgf_data << endl;
    }

    ++it;
  }
}

string GetCSVHeader()
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

  header += "game_record,";
  header += "alternative_moves";

  return header;
}

string ReplaceComma(const string &str)
{
  string replaced_str = str;
  replace(replaced_str.begin(), replaced_str.end(), ',', ' ');

  return replaced_str;
}

string GetCSVData(const string &sgf_data)
{
  SGFParser sgf_parser(kSGFCheckNone);
  sgf_parser.ParseSGF(sgf_data);

  string info_text;
  std::array<string, 4> game_end_status_str{{"Resign", "Timeup", "AgreedDraw", "Unknown"}};
  std::array<string, 4> game_result_str{{"BlackWin", "WhiteWin", "Draw", "Unknown"}};

  info_text += ReplaceComma(sgf_parser.GetGameDate()) + ",";
  info_text += ReplaceComma(sgf_parser.GetEventName()) + ",";
  
  info_text += ReplaceComma(sgf_parser.GetBlackPlayerName()) + ",";
  info_text += ReplaceComma(sgf_parser.GetBlackPlayerRank()) + ",";
  info_text += ReplaceComma(sgf_parser.GetWhitePlayerName()) + ",";
  info_text += ReplaceComma(sgf_parser.GetWhitePlayerRank()) + ",";

  info_text += ReplaceComma(sgf_parser.GetGameRule()) + ",";

  info_text += ReplaceComma(game_end_status_str[sgf_parser.GetEndStatus()]) + ",";
  info_text += ReplaceComma(game_result_str[sgf_parser.GetGameResult()]) + ",";

  info_text += ReplaceComma(sgf_parser.GetGameRecord()) + ",";
  info_text += ReplaceComma(sgf_parser.GetAlternativeMoves());

  return info_text;
}
