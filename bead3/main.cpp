// compilation
// $ g++ -std=c++11 main.cpp -o main

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <string>
#include <chrono>

typedef std::array<int, 4> Vector;
typedef std::array<std::array<int, 4>, 4> Matrix;

typedef std::vector<Vector> Vectors;
typedef std::vector<Matrix> Matrices;

Matrices readMatrices(const std::string& filename)
{
  Matrices M;
  std::ifstream input(filename);
  unsigned int linTransCount;

  input >> linTransCount;

  for (size_t i = 0; i < linTransCount; i++)
  {
    Matrix m;

    for (size_t i = 0; i < 4; i++) {
      input >> m[i][0] >> m[i][1] >> m[i][2] >> m[i][3];
    }

    M.push_back(m);
  }

  input.close();
  return M;
}

Vectors readVectors(const std::string& filename)
{
  Vectors V;
  std::ifstream input(filename);
  unsigned int vectorCount;

  input >> vectorCount;

  for (size_t i = 0; i < vectorCount; i++)
  {
    Vector v;

    input >> v[0] >> v[1] >> v[2];

    v[3] = 1;

    V.push_back(v);
  }

  input.close();
  return V;
}

void writeFile(const std::string& filename, Vectors& V)
{
  std::ofstream output(filename);

  for (auto const& v : V)
  {
    for (size_t i = 0; i < v.size(); i++) {
      output << v[i] << " ";
    }

    output << std::endl;
  }

  output.close();
}

void multiplication(const Matrix& m, Vector& v)
{
  const int length = v.size();

  Vector result;

  for (size_t i = 0; i < length; i++) {
    int item = 0;

    for (size_t j = 0; j < length; j++) {
      item += m[i][j] * v[j];
    }

    v[i] = item;
  }
}

void linTransforms(Vectors& V, Matrices& M)
{
  for (auto& v : V)
  {
    for (auto const& m : M)
    {
      multiplication(m, v);
    }
  }
}

int main()
{
  using namespace std::chrono;

  Matrices M = readMatrices("tests/0/input_matrices.txt");
  Vectors V = readVectors("tests/0/input_points.txt");

  time_point<system_clock> t0, t1;
  t0 = system_clock::now();

  linTransforms(V, M);

  t1 = system_clock::now();
  std::cout << duration_cast<milliseconds>(t1 - t0).count() << "ms\n";

  writeFile("output.txt", V);

  return 0;
}
