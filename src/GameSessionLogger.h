#ifndef GAME_SESSION_LOGGER_H
#define GAME_SESSION_LOGGER_H

#include <fstream>
#include <future>
#include <chrono>
#include <string>

/**
 * @class GameSessionLogger
 */
class GameSessionLogger
{
public:
  GameSessionLogger();
  ~GameSessionLogger();
  void logStart();
  void logEnd(int score, int size, long totalTime);
  void logScoreAndSizeChange(int score, int size);
  void setUserName(const std::string &userName);
  void startLoggingLoop();
  void stopLoggingLoop();
  void loggerLoop();
  void logSeparator();
  std::string getCurrentTime() const;

private:
  std::ofstream logFile;
  std::string userName;
  std::promise<void> stopPromise;
  std::future<void> stopFuture;
};

#endif // GAME_SESSION_LOGGER_H