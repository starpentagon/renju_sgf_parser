#ifndef RENJU_SGF_PARSER_H
#define RENJU_SGF_PARSER_H

#include <string>

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

//! @brief 対局情報を保持するクラス
class GameInfo{
  friend class GameInfoTest;

public:
  GameInfo();

  //! @param file_name SGFファイルパス
  //! @retval true 解析に成功
  //! @retval false 解析エラーが発生
  bool Parse(const std::string &file_name);

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
  std::string ParseDiagram(const std::string &sgf_data) const;

  //! @brief 5手目の取り除かれた提示珠を取得する
  std::string ParseAlternativeMoves(const std::string &sgf_data) const;

  std::string game_date_;     //!< 対局日

  std::string black_player_name_;   //!< 黒番の対局者名
  std::string black_player_rank_;   //!< 黒番のランク
  std::string white_player_name_;   //!< 白番の対局者名
  std::string white_player_rank_;   //!< 白番のランク

  std::string game_rule_;         //!< 対局ルール(RIF, Sakata, Yamaguchi, Tarannikov, Jonsson, Unknown)
  GameEndStatus game_end_status_; //!< 終局状態
  GameResult game_result_;        //!< 対局結果
  std::string diagram_;           //!< 棋譜（[a-o]形式）
  std::string alternative_moves_; //!< 5手目の提示（取り除かれた珠）
};

#endif    // RENJU_SGF_PARSER_H
