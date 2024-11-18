#include "GameSessionLogger.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <sstream>

GameSessionLogger::GameSessionLogger()
    : stopFuture(stopPromise.get_future())
{
    logFile.open("GameSessionLog.txt", std::ios::out | std::ios::app);
    if (!logFile.is_open())
    {
        std::cerr << "Error: Unable to open the log file." << std::endl;
    }
}

GameSessionLogger::~GameSessionLogger()
{
    stopPromise.set_value();
    if (logFile.is_open())
    {
        logFile.close();
    }
}

void GameSessionLogger::logStart()
{
    if (!logFile.is_open())
        return;

    logSeparator();
    logFile << "Session Start: " << getCurrentTime() << std::endl;
    logFile << "Player: " << userName << std::endl;
}

void GameSessionLogger::logEnd(int score, int size, long totalTime)
{
    if (!logFile.is_open())
        return;

    logFile << "Session End: " << getCurrentTime() << std::endl;
    logFile << "Player: " << userName << std::endl;
    logFile << "Duration: " << totalTime << " seconds" << std::endl;
    logFile << "Final Score: " << score << std::endl;
    logFile << "Final Snake Length: " << size << std::endl;
    logSeparator();
}

void GameSessionLogger::logScoreAndSizeChange(int score, int size)
{
    if (!logFile.is_open())
        return;

    logFile << "Update [" << getCurrentTime() << "]: Score = " << score
            << ", Length = " << size << std::endl;
}

void GameSessionLogger::setUserName(const std::string &userName)
{
    this->userName = userName;
}

void GameSessionLogger::loggerLoop()
{
    while (stopFuture.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
    {
        // Simulate logging or monitoring tasks here
    }
}

// Private Helper Functions
void GameSessionLogger::logSeparator()
{
    if (!logFile.is_open())
        return;

    logFile << "=========================================================" << std::endl;
}

std::string GameSessionLogger::getCurrentTime() const
{
    std::ostringstream oss;
    auto now = std::time(nullptr);
    oss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}