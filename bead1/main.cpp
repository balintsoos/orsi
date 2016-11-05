// compilation on OSX
// $ g++ -std=c++11 main.cpp -o main

#include <fstream>
#include <vector>
#include <map>
#include <future>
#include <numeric>

typedef std::vector<int> GameTimes;
typedef std::map<std::string, GameTimes> GameMap;
typedef std::pair<int, int> StatPair;
typedef std::map<std::string, std::future<StatPair>> GameStat;

int sum(const GameTimes& vector)
{
  return std::accumulate(vector.begin(), vector.end(), 0);
}

int avg(const GameTimes& vector)
{
  return sum(vector) / vector.size();
}

StatPair calculateGameStats(const GameTimes& gameplays)
{
  return std::make_pair(sum(gameplays), avg(gameplays));
}

GameMap readFile(const std::string filename)
{
  std::ifstream input(filename);

  unsigned int lineCount, gameCount;

  GameMap games;
  std::string title;
  int seconds;

  input >> lineCount >> gameCount;

  for (int i = 0; i < lineCount; i++)
  {
    std::string temp;

    input >> temp >> title >> seconds;

    games[title].push_back(seconds);
  }

  input.close();

  return games;
}

void writeFile(const std::string filename, GameStat& results)
{
  std::ofstream output(filename);

  for (auto& result : results)
  {
    std::string title = result.first;
    StatPair stat = result.second.get();

    output << title << " " << stat.first << " " << stat.second << std::endl;
  }

  output.close();
}

int main()
{
  GameMap games = readFile("input.txt");

  // Calculating sum and avg for every game
  GameStat results;

  for (auto const & game : games)
  {
    results.insert(std::make_pair(game.first, std::async(std::launch::async, calculateGameStats, game.second)));
  }

  writeFile("output.txt", results);

  return 0;
}
