#include <iostream>
#include <boost/program_options.hpp>

#include "RenjuSGFParser.h"

using namespace std;
using namespace boost::program_options;

int main(int argc, char* argv[])
{
  // オプション設定
  options_description option;

  option.add_options()
    ("file,f", value<string>(), "SGF形式ファイル")
    ("header", "CSV形式のヘッダを表示")
    ("help,h", "ヘルプを表示");
  
  variables_map arg_map;
  store(parse_command_line(argc, argv, option), arg_map);

  if(arg_map.count("header")){
    cout << GetCSVHeader() << endl;

    return 0;
  }

  if(arg_map.count("help") || !arg_map.count("file")){
    cout << "Usage: " << argv[0] << " [options]" << endl;
    cout << option;
    cout << endl;

    return 0;
  }

  const string file_path = arg_map["file"].as<string>();
  ParseSGFFile(file_path);

  return 0;
}