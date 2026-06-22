#include "Actions.hpp"
#include <limits>

namespace novikov
{
  void Create(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    size_t rows, cols;
    if (!(in >> name >> rows >> cols)) {
      throw std::logic_error("Invalid command");
    }
    if (db.has(name)) {
      throw std::logic_error("Matrix already exists");
    }
    Matrix *m = new Matrix(rows, cols);
    db.insert(name, m);
  }

  void Show(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    if (!(in >> name)) {
      throw std::logic_error("Invalid command");
    }
    Matrix* m = db.get(name);
    m->print();
  }

  void Delete(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    if(!(in>>name))
    {
      throw std::logic_error("Invalid command");
    }
    Matrix* m = db.get(name);
    delete m;
    db.remove(name);
  }

}