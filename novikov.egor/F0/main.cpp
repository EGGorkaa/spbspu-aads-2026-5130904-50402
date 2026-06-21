#include "Actions.hpp"
#include <iostream>
#include <limits>
#include <string>

int main()
{
  novikov::MatrixHash db;

  using Command = void (*)(std::istream &, std::ostream &, novikov::MatrixHash &);
  novikov::CuckooHash< std::string, Command > cmds;
  cmds.insert("create", novikov::Create);
  cmds.insert("show", novikov::Show);

  std::string cmd;
  while (std::cin >> cmd) {
    try {
      cmds.get(cmd)(std::cin, std::cout, db);
    } catch (const std::exception &) {
      std::cout << "<INVALID COMMAND>" << "\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
  }
  return 0;
}