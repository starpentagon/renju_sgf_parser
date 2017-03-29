#include "gtest/gtest.h"

#include "RenjuSGFParser.h"

using namespace std;

class GameInfoTest
: public ::testing::Test
{
public:
  GameInfoTest()
  {
    sgf_data_ = ""
"(;"
"GM[5]FF[4]"
"SW[NO]"
"5A[gg]"
"GN[PlayerA-PlayerB(B) : RIF rule]"
"EV[none]"
"RE[W+Resign]"
"PW[PlayerA]WR[3d*]"
"PB[PlayerB]BR[1d ]"
"DT[2003-10-07]"
"SZ[15]TM[600]KM[5.5]"
""
";B[hh];W[hg];B[jf];W[ig]C["
" A started observation."
"]"
";B[ie];W[kg];B[jg];W[jh];B[ki];W[gg]"
";)"
""
"---";
  }

  void ParseGameDateTest(){
    GameInfo game_info;

    const string parse_result = game_info.ParseGameDate(sgf_data_);
    EXPECT_EQ("2003-10-07", parse_result);
  }

  void ParseBlackPlayerNameTest()
  {
    GameInfo game_info;

    const string parse_result = game_info.ParseBlackPlayerName(sgf_data_);
    EXPECT_EQ("PlayerB", parse_result);
  }

  void ParseBlackPlayerRankTest()
  {
    GameInfo game_info;

    const string parse_result = game_info.ParseBlackPlayerRank(sgf_data_);
    EXPECT_EQ("1d ", parse_result);
  }

  void ParseWhitePlayerNameTest()
  {
    GameInfo game_info;

    const string parse_result = game_info.ParseWhitePlayerName(sgf_data_);
    EXPECT_EQ("PlayerA", parse_result);
  }

  void ParseWhitePlayerRankTest()
  {
    GameInfo game_info;

    const string parse_result = game_info.ParseWhitePlayerRank(sgf_data_);
    EXPECT_EQ("3d*", parse_result);
  }

  void ParseGameRuleTest()
  {
    GameInfo game_info;

    const string game_rule = game_info.ParseGameRule(sgf_data_);
    EXPECT_EQ("RIF", game_rule);
  }

  void ParseGameEndStatusTest()
  {
    GameInfo game_info;

    const GameEndStatus end_status = game_info.ParseGameEndStatus(sgf_data_);
    EXPECT_EQ(kResign, end_status);
  }

  void ParseGameResultTest()
  {
    GameInfo game_info;

    const GameResult game_result = game_info.ParseGameResult(sgf_data_);
    EXPECT_EQ(kWhiteWin, game_result);
  }

  void ParseDiagramTest()
  {
    GameInfo game_info;

    const string diagram = game_info.ParseDiagram(sgf_data_);
    const string expect_diagram = "hhhgjfigiekgjgjhkigg";

    EXPECT_EQ(expect_diagram, diagram);
  }

  void ParseAlternativeMovesTest()
  {
    GameInfo game_info;

    const string alternative = game_info.ParseAlternativeMoves(sgf_data_);
    const string expect_alternative = "gg";

    EXPECT_EQ(alternative, expect_alternative);
  }

  void ParseEventNameTest()
  {
    GameInfo game_info;

    const string game_rule = game_info.ParseEventName(sgf_data_);
    EXPECT_EQ("none", game_rule);
  }

private:
  string sgf_data_;
};

TEST_F(GameInfoTest, ParseGameDateTest)
{
  ParseGameDateTest();
}

TEST_F(GameInfoTest, ParseBlackPlayerNameTest)
{
  ParseBlackPlayerNameTest();
}

TEST_F(GameInfoTest, ParseBlackPlayerRankTest)
{
  ParseBlackPlayerRankTest();
}

TEST_F(GameInfoTest, ParseWhitePlayerNameTest)
{
  ParseWhitePlayerNameTest();
}

TEST_F(GameInfoTest, ParseWhitePlayerRankTest)
{
  ParseWhitePlayerRankTest();
}

TEST_F(GameInfoTest, ParseGameRuleTest)
{
  ParseGameRuleTest();
}

TEST_F(GameInfoTest, ParseGameEndStatusTest)
{
  ParseGameEndStatusTest();
}

TEST_F(GameInfoTest, ParseGameResultTest)
{
  ParseGameResultTest();
}

TEST_F(GameInfoTest, ParseDiagramTest)
{
  ParseDiagramTest();
}

TEST_F(GameInfoTest, ParseAlternativeMovesTest)
{
  ParseAlternativeMovesTest();
}

TEST_F(GameInfoTest, ParseEventNameTest)
{
  ParseEventNameTest();
}