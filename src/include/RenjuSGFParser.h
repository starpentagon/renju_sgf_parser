#ifndef RENJU_SGF_PARSER_H
#define RENJU_SGF_PARSER_H

#include <string>
#include <vector>

//! @brief 対局結果
enum GameResult{
  kBlackWin,        //!< 黒勝
  kWhiteWin,        //!< 白勝
  kDraw,            //!< 満局
  kUnknownResult,   //!< 不明
};

//! @brief 終局ステータス
enum GameEndStatus
{
  kResign,            //!< 投了
  kTimeup,            //!< 時間切れ
  kAgreedDraw,        //!< 合意満局
  kUnknownEndStatus,  //!< 不明
};

class GameInfo;

//! @brief SGFファイルを解析し対局情報のリストを出力する
//! @param file_path SGFファイルのパス
void ParseSGFFile(const std::string &file_path);

//! @brief 対局情報を保持するクラス
class GameInfo{
  friend class GameInfoTest;

public:
  GameInfo();

  //! @param sgf_data SGFデータ
  //! @note 解析に失敗した場合はlogic_error例外を送出する
  void Parse(const std::string &sgf_data);

  //! @brief CSV形式のヘッダを出力する
  //! @note 対局日,大会名,対局者名,ランク,開局ルール,終局状態,対局結果,5手目提示珠,棋譜の順で出力する
  std::string GetCSVHeader() const;

  //! @brief CSV形式のデータを出力する
  //! @note 対局日,大会名,対局者名,ランク,開局ルール,終局状態,対局結果,5手目提示珠,棋譜の順で出力する
  std::string str() const;

private:
  //! @brief 対局日を取得する
  std::string ParseGameDate(const std::string &sgf_data) const;

  //! @brief 黒番の対局者名を取得する
  std::string ParseBlackPlayerName(const std::string &sgf_data) const;

  //! @brief 黒番のランク(1dなど)を取得する
  std::string ParseBlackPlayerRank(const std::string &sgf_data) const;

  //! @brief 白番の対局者名を取得する
  std::string ParseWhitePlayerName(const std::string &sgf_data) const;

  //! @brief 白番のランク(1dなど)を取得する
  std::string ParseWhitePlayerRank(const std::string &sgf_data) const;

  //! @brief 対局ルールを取得する
  std::string ParseGameRule(const std::string &sgf_data) const;

  //! @brief 終局状態を取得する
  GameEndStatus ParseGameEndStatus(const std::string &sgf_data) const;

  //! @brief 対局結果を取得する
  GameResult ParseGameResult(const std::string &sgf_data) const;

  //! @brief 棋譜([a-o]形式)を取得する
  std::string ParseGameRecord(const std::string &sgf_data) const;

  //! @brief 5手目の取り除かれた提示珠を取得する
  std::string ParseAlternativeMoves(const std::string &sgf_data) const;

  //! @brief 大会名を取得する
  std::string ParseEventName(const std::string &sgf_data) const;

  //! @brief CSVフィールドにカンマが入らないようにカンマをスペースに置換する
  std::string ReplaceComma(const std::string &str) const;

  std::string game_date_;     //!< 対局日

  std::string black_player_name_;   //!< 黒番の対局者名
  std::string black_player_rank_;   //!< 黒番のランク(option)
  std::string white_player_name_;   //!< 白番の対局者名
  std::string white_player_rank_;   //!< 白番のランク(option)

  std::string game_rule_;         //!< 対局ルール(RIF, Sakata, Yamaguchi, Tarannikov, Jonsson, Unknown)
  GameEndStatus game_end_status_; //!< 終局状態
  GameResult game_result_;        //!< 対局結果
  std::string game_record_;           //!< 棋譜（[a-o]形式）
  std::string alternative_moves_; //!< 5手目の提示（取り除かれた珠）(option)

  std::string event_name_;    //!< 大会名(option)
};

#endif    // RENJU_SGF_PARSER_H
