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

int sum(GameTimes &vector)
{
  int sumOfElems = 0;

  for (int n : vector)
    sumOfElems += n;

  return sumOfElems;
}

double avg(GameTimes &vector)
{
  return std::floor(sum(vector) / vector.size());
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
  std::vector<std::future<int>> results;

  for (auto& game : games) {
    std::cout << game.first << " " << sum(game.second) << " "<< avg(game.second) << std::endl;
  }

  return 0;
}
