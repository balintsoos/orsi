// compilation
// $ g++ -std=c++11 main.cpp -o main

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>
#include <future>
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

template<typename Iter>
void bubbleSort(Iter begin, Iter end)
{
  for (Iter i = begin; i != end; ++i)
  {
    for (Iter j = begin; j < i; ++j)
    {
      if (*i < *j)
      {
        std::iter_swap(i, j);
      }
    }
  }
}

template<typename Iter>
std::vector<typename Iter::value_type> merge(Iter begin, Iter middle, Iter end) {
  std::vector<typename Iter::value_type> buffer;
  buffer.reserve(std::distance(begin, end));

  Iter left(begin);
  Iter right(middle);

  const Iter &left_end = middle;
  const Iter &right_end = end;

  while (left != left_end && right != right_end)
  {
      buffer.push_back((*left <= *right) ? *left++ : *right++);
  }

  buffer.insert(buffer.end(), left, left_end);
  buffer.insert(buffer.end(), right, right_end);

  return buffer;
}

template<typename Iter>
void mergeSort(Iter begin, Iter end) {
  auto length = std::distance(begin, end);

  if (length <=  64)
  {
    bubbleSort(begin, end);
    return;
  }

  Iter middle = std::next(begin, length / 2);

  auto future = std::async(mergeSort<Iter>, begin, middle);
  mergeSort(middle, end);

  future.wait();

  auto &&v = merge(begin, middle, end);
  std::move(v.cbegin(), v.cend(), begin);
}

int main()
{
  using namespace std::chrono;

  NeptunIds ids = readFile("input.txt");

  time_point<system_clock> t0, t1;
  t0 = system_clock::now();

  mergeSort(ids.begin(), ids.end());

  t1 = system_clock::now();
  std::cout << duration_cast<milliseconds>(t1 - t0).count() << "ms\n";

  writeFile("output.txt", ids);

  return 0;
}
