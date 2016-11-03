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
  GameTitle key;
  GameTime value;
  std::string temp;

  for (int i = 0; i < lineCount; i++)
  {
    input >> temp >> key >> value;

    if (!games.count(key)) games[key];

    games[key].push_back(value);
  }

  input.close();

  // Start calculating the sum and avg for every game
  GameStat results;

  for (auto& game : games)
  {
    results.insert(std::make_pair(game.first, std::async(std::launch::async, processGame, game.second)));
  }

  for (auto& result : results)
  {
    result.second.wait();
    std::cout << result.first << " " << result.second.get().first << " "<< result.second.get().second << std::endl;
  }

  return 0;
}
