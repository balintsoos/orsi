// compilation on OSX
// $ g++ -std=c++11 main.cpp -o main

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <cmath>
#include <future>

typedef std::string GameTitle;
typedef int GameTime;
typedef std::vector<GameTime> GameTimes;
typedef std::map<GameTitle, GameTimes> GameMap;
typedef std::pair<int, double> StatPair;
typedef std::map<GameTitle, std::future<StatPair>> GameStat;

int sum(const GameTimes& gameplays)
{
  int result = 0;

  for (int n : gameplays)
    result += n;

  return result;
}

double avg(const GameTimes& gameplays)
{
  return std::floor(sum(gameplays) / gameplays.size());
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
  GameTitle title;
  GameTime seconds;
  std::string temp;

  input >> lineCount >> gameCount;

  for (int i = 0; i < lineCount; i++)
  {
    input >> temp >> title >> seconds;

    if (!games.count(title)) games[title];

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
    GameTitle title = result.first;
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
