// compilation
// $ g++ -std=c++11 main.cpp
// or
// $ clang++ -std=c++11 main.cpp

#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include "pipeline.hpp"

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

void writeVectors(const std::string& filename, Vectors& V)
{
  std::ofstream output(filename);

  for (auto const& v : V)
  {
    output << v[0] << " " << v[1] << " " << v[2] << std::endl;
  }

  output.close();
}

Vector linTransform(const Matrix& m, const Vector& v)
{
  Vector result;

  for (size_t i = 0; i < v.size(); i++) {
    result[i] = 0;

    for (size_t j = 0; j < v.size(); j++) {
      result[i] += m[i][j] * v[j];
    }
  }

  return result;
}

void threadHandler(Pipeline<Vector>& input, Pipeline<Vector>& output, const Matrix& m, int vectorCount)
{
  for (size_t i = 0; i < vectorCount; i++)
  {
    Vector v = input.pop();

    Vector result = linTransform(m,v);
    output.push(result);
  }
}

void calculate(Vectors& V, Matrices& M)
{
  std::vector<std::thread> threads;
  std::vector<Pipeline<Vector>> pipes(M.size() + 1);

  for (size_t i = 0; i < M.size(); i++) {
    threads.push_back(std::thread(threadHandler, std::ref(pipes[i]), std::ref(pipes[i + 1]), M[i], V.size()));
  }

  for (auto& v : V)
  {
    pipes[0].push(v);
  }

  for (auto& t : threads)
  {
    t.join();
  }

  for (auto& v : V)
  {
    Vector result = pipes[pipes.size() - 1].pop();

    v = result;
  }
}

int main()
{
  using namespace std::chrono;

  Matrices M = readMatrices("input_matrices.txt");
  Vectors V = readVectors("input_points.txt");

  time_point<system_clock> start, end;
  start = system_clock::now();

  calculate(V, M);

  end = system_clock::now();
  std::cout << duration_cast<milliseconds>(end - start).count() << "ms\n";

  writeVectors("output.txt", V);

  return 0;
}
