#include <iostream>
#include <memory>
#include <future>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "GameSessionLogger.h"
#include <chrono>

class GameManager
{
public:
  GameManager(const std::string &userName, std::size_t screenWidth, std::size_t screenHeight,
              std::size_t gridWidth, std::size_t gridHeight, std::size_t msPerFrame)
      : renderer(std::make_unique<Renderer>(screenWidth, screenHeight, gridWidth, gridHeight)),
        controller(), gameLogger(), game(gridWidth, gridHeight, gameLogger), kMsPerFrame(msPerFrame)
  {

    gameLogger.setUserName(userName);
    gameLogger.logStart();
  }

  void Run()
  {
    // Create promises and futures for game results
    std::promise<int> scorePromise;
    std::future<int> scoreFuture = scorePromise.get_future();

    std::promise<int> sizePromise;
    std::future<int> sizeFuture = sizePromise.get_future();

    std::promise<long> timePromise;
    std::future<long> timeFuture = timePromise.get_future();

    // Start the game in a separate thread
    std::thread gameThread(&GameManager::GameThread, this, std::ref(scorePromise), std::ref(sizePromise), std::ref(timePromise));

    // Wait for the game to finish
    {
      std::unique_lock<std::mutex> lock(mtx);
      cv.wait(lock, [this]
              { return gameFinished; });
    }

    // Retrieve the game results
    int finalScore = scoreFuture.get();
    int finalSize = sizeFuture.get();
    long elapsedTime = timeFuture.get();

    // Log the end of the game
    gameLogger.logEnd(finalScore, finalSize, elapsedTime);

    std::cout << "Game Ended\n";
    std::cout << "Duration of Game: " << elapsedTime << " seconds\n";
    std::cout << "Final Score Achieved: " << finalScore << "\n";
    std::cout << "Final Length of Snake: " << finalSize << "\n";

    // Ensure the game thread finishes execution
    gameThread.join();
  }

private:
  void GameThread(std::promise<int> &scorePromise, std::promise<int> &sizePromise, std::promise<long> &timePromise)
  {
    std::lock_guard<std::mutex> lock(mtx);

    auto startTime = std::chrono::steady_clock::now();
    game.Run(controller, renderer, kMsPerFrame);
    auto endTime = std::chrono::steady_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

    // Set the promises with game results
    scorePromise.set_value(game.GetScore());
    sizePromise.set_value(game.GetSize());
    timePromise.set_value(elapsedTime);

    // Notify that the game has finished
    gameFinished = true;
    cv.notify_one();
  }

  std::unique_ptr<Renderer> renderer;
  Controller controller;
  GameSessionLogger gameLogger;
  Game game;
  const std::size_t kMsPerFrame;

  std::mutex mtx;
  std::condition_variable cv;
  bool gameFinished = false;
};

int main()
{
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  std::string userName;
  std::cout << "Enter your name: ";
  std::getline(std::cin, userName);

  GameManager gameManager(userName, kScreenWidth, kScreenHeight, kGridWidth, kGridHeight, kMsPerFrame);
  gameManager.Run();

  return 0;
}
