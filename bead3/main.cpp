// compilation
// $ g++ -std=c++11 main.cpp -o main

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
    for (size_t i = 0; i < v.size() - 1; i++) {
      output << v[i] << " ";
    }

    output << std::endl;
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
  for (size_t i = 0; i < vectorCount; i++) {
    Vector v = input.pop();

    output.push(linTransform(m, v));
  }
}

void linTransforms(Vectors& V, Matrices& M)
{
  std::vector<std::thread> threads;
  std::vector<std::reference_wrapper<Pipeline<Vector>>> pipes;

  Pipeline<Vector> firstPipe;

  firstPipe.push(V);
  pipes.push_back(std::ref(firstPipe));

  for (size_t i = 0; i < M.size(); i++) {
    Pipeline<Vector> p;

    pipes.push_back(std::ref(p));
  }

  for (size_t i = 0; i < M.size(); i++) {
    threads.push_back(std::thread(threadHandler, pipes[i], pipes[i + 1], M[i], V.size()));
  }

  for (auto& t : threads)
  {
    t.join();
  }

  for (size_t i = 0; i < V.size(); i++) {
    Vector v = pipes[pipes.size() - 1].get().pop();

    V[i] = v;
  }
}

int main()
{
  using namespace std::chrono;

  Matrices M = readMatrices("input_matrices.txt");
  Vectors V = readVectors("input_points.txt");

  time_point<system_clock> t0, t1;
  t0 = system_clock::now();

  linTransforms(V, M);

  t1 = system_clock::now();
  std::cout << duration_cast<milliseconds>(t1 - t0).count() << "ms\n";

  writeVectors("output.txt", V);

  return 0;
}
