// compilation
// $ g++ -std=c++11 main.cpp -o main

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <string>
#include <chrono>

typedef std::array<int, 3> Vector;
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

    V.push_back(v);
  }

  input.close();
  return V;
}

void writeFile(const std::string& filename, Matrices& M)
{
  std::ofstream output(filename);

  for (auto const& m : M)
  {
    for (size_t i = 0; i < 4; i++) {
      output << m[i][0] << " " << m[i][1] << " " << m[i][2] << " " << m[i][3] << std::endl;
    }
  }

  output.close();
}

int main()
{
  using namespace std::chrono;

  Matrices M = readMatrices("tests/0/input_matrices.txt");
  Vectors V = readVectors("tests/0/input_points.txt");

  time_point<system_clock> t0, t1;
  t0 = system_clock::now();

  // TODO

  t1 = system_clock::now();
  std::cout << duration_cast<milliseconds>(t1 - t0).count() << "ms\n";

  writeFile("output.txt", M);

  return 0;
}
