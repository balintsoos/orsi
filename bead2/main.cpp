// compilation on OSX
// $ g++ -std=c++11 main.cpp -o main

#include <fstream>
#include <vector>
#include <string>
#include <numeric>
#include <future>

typedef std::vector<std::string> NeptunIds;

NeptunIds readFile(const std::string filename)
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

void writeFile(const std::string filename, NeptunIds &ids)
{
  std::ofstream output(filename);

  for (auto const& id : ids)
  {
    output << id << std::endl;
  }

  output.close();
}

void bubbleSort(NeptunIds &v)
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

int main()
{
  NeptunIds ids = readFile("tests/input_1.txt");

  bubbleSort(ids);

  writeFile("output.txt", ids);

  return 0;
}
