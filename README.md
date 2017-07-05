# Renju SGF Parser

Renju SGF Parser is a SGF(Smart Game Format) file parser for Renju games.

The program input and parse the SGF file, then output the following information in CSV format:
* Game date
* Event name
* Black player name
* Black player rank
* White player name
* White player rank
* Opening rule(RIF, Yamaguchi, etc)
* Game end status(Resign, Timeup, AgreedDraw)
* Game result(BlackWin, WhiteWin, Draw)
* Game record(black and white moves in [a-o] format)
* Alternative moves

I tested the SGF files:
- published in ORC Game Center
- exported by Renjubase

## Requirement
* CMake 3.5.1 or newer
* boost 1.63 or newer
* C++ compiler
   * clang 3.8 or newer
* Google Test 1.8.0 or newer(if you build test program in ./test)
* ccache 3.3.4 or newer(optional)

## Environment variables
Build shell scripts and CMake require the following environment variables:
* BOOST_DIR: the top directory of the Boost Libraries
* GTEST_DIR: the top directory of the Google Test
* MAKE_JOB_NUMBER: the number of jobs to compile C++ files simultaneously

## How to build

Move to the root directory of the Renju SGF Parser, and run
`./build.sh`

You can find the program in build/renju_sgf_parser

## Usage

1. Get CSV header
Run:
`./renju_sgf_parser --header`

2. Parse the SGF file
Run:
`./renju_sgf_parser --file (SGF file)`

3. Show help
Run:
`./renju_sgf_parser --help`

## Supported information elements

The parser supports following SGF properties:
* DT: game date
* EV: event name
* PB, PW: black/white player name
* BR, WR: black/white player rank
* RU, GN: Opening rule(SGF files in ORC Game Center don't provide the RU property, so the program parses GN(Game Name) infomation)
* RE: Game end status and result
* B, W: black/white player's move
   * A move at 'tt'(out of the renju board) is sometimes recorded in ORC Game Center. The move means pass and usually implies draw agreement. So, the parser regards the game as finished when the move at 'tt' is appeared.

## Developer's blog

Please visit the developer's blog(http://quinstella.net/dev/) if you are interested in Renju programming.

