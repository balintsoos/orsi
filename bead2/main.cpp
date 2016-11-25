// compilation on OSX
// $ g++ -std=c++11 main.cpp -o main

// Resources:
// http://stackoverflow.com/questions/24130307/performance-problems-in-parallel-mergesort-c

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>
#include <future>
#include <thread>
#include <chrono>

typedef std::vector<std::string> NeptunIds;

NeptunIds readFile(const std::string& filename)
{
  NeptunIds ids;
  std::ifstream input(filename);
  unsigned int lineCount;

  input >> lineCount;

  for (int i = 0; i < lineCount; i++)
  {
    std::string id;

    input >> id;
    ids.push_back(id);
  }

  input.close();
  return ids;
}

void writeFile(const std::string& filename, NeptunIds& ids)
{
  std::ofstream output(filename);

  for (auto const& id : ids)
  {
    output << id << std::endl;
  }

  output.close();
}

template <typename T>
void bubbleSort(std::vector<T>& v)
{
  bool swapped = true;

  for (unsigned j = 1; swapped && j < v.size(); ++j)
  {
    swapped = false;

    for (unsigned i = 0; i < v.size() - j; ++i)
    {
      if (v[i] > v[i + 1])
      {
        std::swap(v[i], v[i + 1]);
        swapped = true;
      }
    }
  }
}

template <typename T>
void merge(std::vector<T>& v, std::vector<T>& v1, std::vector<T>& v2) {
  int i, j, k;
  v.clear();

  for (i = 0, j = 0, k = 0; i < v1.size() && j < v2.size(); ++k)
  {
    if (v1.at(i) <= v2.at(j))
    {
      v.push_back(v1.at(i));
      ++i;
    }
    else if (v1.at(i) > v2.at(j))
    {
      v.push_back(v2.at(j));
      ++j;
    }
    ++k;
  }

  while(i < v1.size())
  {
    v.push_back(v1.at(i));
    ++i;
  }

  while(j < v2.size())
  {
    v.push_back(v2.at(j));
    ++j;
  }
}

template <typename T>
void mergeSort(std::vector<T>& v, unsigned int threadLimit = std::thread::hardware_concurrency() / 2) {
  if (v.size() <=  1024)
  {
    bubbleSort(v);
    return;
  }

  std::vector<T> v1(v.begin(), v.begin() + v.size() / 2);
  std::vector<T> v2(v.begin() + v.size() / 2, v.end());

  if (threadLimit > 1) {
    auto future = std::async([&v1, &threadLimit]() mutable {
      mergeSort(v1, threadLimit - 2);
    });

    mergeSort(v2, threadLimit - 2);

    future.wait();
  }
  else
  {
    mergeSort(v1, 0);
    mergeSort(v2, 0);
  }

  merge(v, v1, v2);
}

int main()
{
  using namespace std::chrono;

  NeptunIds ids = readFile("tests/input_3.txt");

  time_point<system_clock> t0, t1;
  t0 = system_clock::now();

  mergeSort(ids);

  t1 = system_clock::now();
  std::cout << duration_cast<milliseconds>(t1 - t0).count() << "ms\n";

  writeFile("output.txt", ids);

  return 0;
}
