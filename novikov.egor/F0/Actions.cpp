#include "Actions.hpp"
#include <limits>

namespace novikov
{
  void Create(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    size_t rows, cols;
    if (!(in >> name >> rows >> cols)) {
      out << "INVALID COMMAND" << "\n";
      in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      return;
    }
    if (db.has(name)) {
      out << "INVALID COMMAND" << "\n";
      return;
    }
    Matrix *m = new Matrix(rows, cols);
    db.insert(name, m);
  }

  void Show(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    if (!(in >> name)) {
      out << "INVALID COMMAND" << "\n";
      return;
    }
    try {
      Matrix *m = db.get(name);
      m->print();
    } catch (const std::out_of_range &) {
      out << "INVALID COMMAND" << "\n";
    }
  }

}