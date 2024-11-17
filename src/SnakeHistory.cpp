#include "SnakeHistory.h"
#include <iostream>
#include <iomanip>
#include <ctime>

SnakeHistory::SnakeHistory()
    : stopFuture(stopPromise.get_future())
{
  logFile.open("SnakeHistory.txt", std::ios::out | std::ios::app);
  if (!logFile.is_open())
  {
    std::cerr << "Failed to open log file." << std::endl;
  }
}

SnakeHistory::~SnakeHistory()
{
  stopPromise.set_value();
  if (logFile.is_open())
  {
    logFile.close();
  }
}

void SnakeHistory::logStart()
{
  std::time_t time = std::time(nullptr);
  logFile << "=========================================================" << std::endl;
  logFile << "New Game Session Started at: " << std::ctime(&time) << "Player: " << userName << std::endl;
}

void SnakeHistory::logEnd(int score, int size, long totalTime)
{
  std::time_t time = std::time(nullptr);
  logFile << "Game Ended at: " << std::ctime(&time) << "Player: " << userName << std::endl;
  logFile << "Duration: " << totalTime << " seconds" << std::endl;
  logFile << "Final Score: " << score << std::endl;
  logFile << "Final Snake Length: " << size << std::endl;
  logFile << "=========================================================" << std::endl;
}

void SnakeHistory::logScoreAndSizeChange(int score, int size)
{
  std::time_t time = std::time(nullptr);
  logFile << "Score Changed: " << score << ", Size Changed: " << size << " at " << std::ctime(&time);
}

void SnakeHistory::setUserName(const std::string &userName)
{
  this->userName = userName;
}

void SnakeHistory::SnakeHistoryLoop()
{
  while (stopFuture.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
  {
  }
}
