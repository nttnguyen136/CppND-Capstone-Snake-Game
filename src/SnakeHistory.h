#ifndef SNAKE_HISTORY_H
#define SNAKE_HISTORY_H

#include <fstream>
#include <future>
#include <chrono>
#include <string>

class SnakeHistory
{
public:
  SnakeHistory();
  ~SnakeHistory();

  void logStart();
  void logEnd(int score, int size, long totalTime);
  void logScoreAndSizeChange(int score, int size);
  void setUserName(const std::string &userName);
  void SnakeHistoryLoop();

private:
  std::ofstream logFile;
  std::string userName;
  std::promise<void> stopPromise;
  std::future<void> stopFuture;
};

#endif // SNAKE_HISTORY_H
