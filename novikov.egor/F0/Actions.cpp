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
  
  void Change(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    size_t i, j;
    double val;
    if(!(in>>name>>i>>j>>val))
    {
      throw std::logic_error("Invalid command");
    }
    Matrix* m = db.get(name);
    m->change(i, j, val);
  }

  void AddRow(std::istream &in, std::ostream &out, MatrixHash &db)
  {
    std::string name;
    size_t index;
    if(!(in >> name >> index))
    {
      throw std::logic_error("Invalid command");
    }
    Matrix* m = db.get(name);
    size_t cols = m->getCols();
    double* values = new double[cols];
    size_t count = 0;
    double val;
    while(count < cols && (in >> val))
    {
      values[count++] = val;
    }
    if(count != cols)
    {
      delete[] values;
      throw std::logic_error("Invalid command");
    }
    m->addRow(index, values, cols);
    delete[] values;
  }

}