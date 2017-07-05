#ifndef RENJU_SGF_PARSER_H
#define RENJU_SGF_PARSER_H

#include <string>

//! @brief SGFファイルを解析し対局情報のリストを出力する
//! @param file_path SGFファイルのパス
void ParseSGFFile(const std::string &file_path);

//! @brief CSV形式のヘッダを出力する
//! @note 対局日,大会名,対局者名,ランク,開局ルール,終局状態,対局結果,5手目提示珠,棋譜の順で出力する
std::string GetCSVHeader();

//! @brief CSV形式のデータを出力する
//! @note 対局日,大会名,対局者名,ランク,開局ルール,終局状態,対局結果,5手目提示珠,棋譜の順で出力する
std::string GetCSVData(const std::string &sgf_data);

#endif    // RENJU_SGF_PARSER_H
