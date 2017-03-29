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

I test the SGF files:
- published in ORC Game Center
- exported by Renjubase

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

The parse supports following SGF properties:
* DT: game date
* EV: event name
* PB, PW: black/white player name
* BR, WR: black/white player rank
* RU, GN: Opening rule(SGF files in ORC Game Center does not provide the RU property, so the program parses GN(Game Name) infomation)
* RE: Game end status and result
