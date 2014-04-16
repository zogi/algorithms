#include <iostream>
#include <iomanip>

#include "linalg.h"

using linalg::Matrix;
using linalg::Vector;
using linalg::identity;
using linalg::zeros;

template <typename T, int N, int M>
std::ostream & operator<<(std::ostream& ost, linalg::Matrix<T, N, M> const & m)
{
  for (int i = 0; i < m.rows; ++i) {
    for (int j = 0; j < m.cols; ++j) {
      ost << std::setw(6) << m(i, j);
    }
    ost << std::endl;
  }
  return ost;
}

int main()
{
  Matrix<int, 2, 3> a;
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 3; ++j)
      a(i, j) = rand()%100;

  std::cout << a;
  std::cout << "==================\n";
  std::cout << identity<int, 2>() * a;
  std::cout << "==================\n";
  std::cout << a * identity<int, 3>();
  std::cout << "==================\n";
  std::cout << (a == a) << "\n";
  std::cout << (a - a == zeros<int, 2, 3>()) << "\n";

  auto b = Matrix<double, 2, 3>(a);
  std::cout << a + b;
  auto v = Vector<double, 3>(1);
  std::cout <<  b * v;
  return 0;
}
