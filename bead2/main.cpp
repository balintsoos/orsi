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

void writeFile(const std::string filename, NeptunIds& ids)
{
  std::ofstream output(filename);

  for (auto const& id : ids)
  {
    output << id << std::endl;
  }

  output.close();
}

int main()
{
  NeptunIds ids = readFile("tests/input_1.txt");

  writeFile("output.txt", ids);

  return 0;
}
