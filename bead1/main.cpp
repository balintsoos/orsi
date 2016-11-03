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
typedef std::map< GameTitle, GameTimes > GameMap;
typedef std::pair<int, double> StatPair;
typedef std::future<StatPair> AsyncStatPair;
typedef std::map< GameTitle, AsyncStatPair> GameStat;

int sum(const GameTimes& vector)
{
  int sumOfElems = 0;

  for (int n : vector)
    sumOfElems += n;

  return sumOfElems;
}

double avg(const GameTimes& vector)
{
  return std::floor(sum(vector) / vector.size());
}

StatPair processGame(const GameTimes& vector)
{
  return std::make_pair(sum(vector), avg(vector));
}

int main()
{
  std::ifstream input("./tests/input_1.txt");

  unsigned int lineCount, gameCount;

  input >> lineCount >> gameCount;

  // Read data from input file
  GameMap games;
  GameTitle title;
  GameTime seconds;
  std::string temp;

  for (int i = 0; i < lineCount; i++)
  {
    input >> temp >> title >> seconds;

    if (!games.count(title)) games[title];

    games[title].push_back(seconds);
  }

  input.close();

  // Start calculating the sum and avg for every game
  GameStat results;

  for (auto const & game : games)
  {
    results.insert(std::make_pair(game.first, std::async(std::launch::async, processGame, game.second)));
  }

  // Write the result into a file
  std::ofstream output("output.txt");

  for (auto& result : results)
  {
    GameTitle title = result.first;
    StatPair stat = result.second.get();

    output << title << " " << stat.first << " " << stat.second << std::endl;
  }

  output.close();

  return 0;
}
