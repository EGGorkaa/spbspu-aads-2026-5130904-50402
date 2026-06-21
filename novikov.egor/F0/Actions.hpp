#ifndef ACTIONS_HPP
#define ACTIONS_HPP

#include <iostream>
#include <string>
#include "CuckooHash.hpp"
#include "Matrix.hpp"

namespace novikov
{
  using MatrixHash = CuckooHash< std::string, Matrix * >;
  void Create(std::istream &in, std::ostream &out, MatrixHash &db);
  void Show(std::istream &in, std::ostream &out, MatrixHash &db);
}

#endif