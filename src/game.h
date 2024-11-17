#ifndef GAME_H
#define GAME_H

#include <memory>
#include <random>
#include <mutex>
#include <condition_variable>
#include <thread>
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "SnakeHistory.h"

class Game
{
public:
  Game(std::size_t grid_width, std::size_t grid_height, SnakeHistory &snakeHistory);
  ~Game();
  void Run(Controller const &controller, std::unique_ptr<Renderer> &renderer,
           std::size_t target_frame_duration);

  int GetScore() const;
  int GetSize() const;

private:
  void PlaceFood();
  void PlaceSpecialFood();
  void Update();
  void ScoreUpdateThread();

  Snake snake;
  std::mt19937 engine;
  std::uniform_int_distribution<int> distribution_w;
  std::uniform_int_distribution<int> distribution_h;
  int score{0};
  SDL_Point food;
  SDL_Point SpecialFood;
  bool recreate{false};
  int countdown{10};
  SnakeHistory &snakeHistory;

  std::mutex scoreMutex;
  std::condition_variable scoreCV;
  bool scoreUpdated{false};
  bool running{true};
  std::thread scoreThread;
  std::promise<void> exitSignal;
};

#endif // GAME_H