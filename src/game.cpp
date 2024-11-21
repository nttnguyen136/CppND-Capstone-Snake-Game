#include "game.h"
#include <SDL.h>

Game::Game(std::size_t grid_width, std::size_t grid_height, GameSessionLogger &gameLogger)
    : snake(grid_width, grid_height),
      engine(std::random_device{}()),
      distribution_w(0, static_cast<int>(grid_width - 1)),
      distribution_h(0, static_cast<int>(grid_height - 1)),
      gameLogger(gameLogger)
{
  PlaceFood();
  PlaceSpecialFood();
  scoreThread = std::thread(&Game::ScoreUpdateThread, this);
}

Game::~Game()
{
  {
    std::unique_lock<std::mutex> lock(scoreMutex);
    running = false;
    scoreCV.notify_one();
  }
  exitSignal.set_value();
  if (scoreThread.joinable())
  {
    scoreThread.join();
  }
}

void Game::Run(Controller const &controller, std::unique_ptr<Renderer> &renderer,
               std::size_t target_frame_duration)
{
  Uint32 title_time = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_time;
  int frames = 0;
  bool is_running = true;

  while (is_running)
  {
    frame_start = SDL_GetTicks();

    controller.HandleInput(is_running, snake, *this);
    Update();
    renderer->Render(snake, food, SpecialFood);

    frame_end = SDL_GetTicks();

    frames++;
    frame_time = frame_end - frame_start;

    if (frame_end - title_time >= 1000)
    {
      renderer->UpdateWindowTitle(score, frames, countdown);
      if (countdown == 0)
      {
        recreate = true;
        countdown = 10;
      }
      countdown--;
      frames = 0;
      title_time = frame_end;
    }

    if (frame_time < target_frame_duration)
    {
      SDL_Delay(target_frame_duration - frame_time);
    }
  }
}

void Game::PlaceFood()
{
  int x, y;
  while (true)
  {
    x = distribution_w(engine);
    y = distribution_h(engine);
    if (!snake.SnakeCell(x, y))
    {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::PlaceSpecialFood()
{
  int x, y;
  while (true)
  {
    x = distribution_w(engine);
    y = distribution_h(engine);

    if (!snake.SnakeCell(x, y) && (x != food.x || y != food.y))
    {
      SpecialFood.x = x;
      SpecialFood.y = y;
      return;
    }
  }
}

void Game::Update()
{
  if (!snake.alive || pause)
    return;

  snake.Update();

  int head_x = static_cast<int>(snake.head_x);
  int head_y = static_cast<int>(snake.head_y);

  bool score_changed = false;

  {
    std::lock_guard<std::mutex> lock(scoreMutex);
    if (food.x == head_x && food.y == head_y)
    {
      score++;
      PlaceFood();
      snake.GrowBody();
      snake.speed += 0.02;
      score_changed = true;
    }

    if (is_bonus_food_active && SpecialFood.x == head_x && SpecialFood.y == head_y)
    {
      score = (score == 0) ? 3 : score + 3;
      PlaceSpecialFood();
      score_changed = true;
    }

    if (score_changed)
    {
      scoreUpdated = true;
      scoreCV.notify_one();
    }
  }

  // if (recreate)
  // {
  //   recreate = false;
  //   PlaceSpecialFood();
  // }
}

void Game::ScoreUpdateThread()
{
  std::future<void> futureObj = exitSignal.get_future();
  while (futureObj.wait_for(std::chrono::milliseconds(1)) == std::future_status::timeout)
  {
    std::unique_lock<std::mutex> lock(scoreMutex);
    scoreCV.wait(lock, [this]
                 { return scoreUpdated || !running; });

    if (!running)
      break;

    if (scoreUpdated)
    {
      gameLogger.logScoreAndSizeChange(score, snake.size);
      scoreUpdated = false;
    }
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }

void Game::TogglePause(bool isPause)
{
  pause = !pause && isPause;
}

void Game::BonusFoodTimer()
{
  const int bonusSeconds = 15;
  auto startTime = std::chrono::high_resolution_clock::now();
  std::unique_lock<std::mutex> lock(bonusMutex);
  while (is_bonus_food_active)
  {
    lock.unlock();
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto elapsedSeconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
    if (elapsedSeconds >= bonusSeconds)
    {
      // Bonus food time is up
      is_bonus_food_active = false;
      bonus_add_food.x = -1; // -1 can indicate the bonus food is not active
      bonus_add_food.y = -1;
      break;
    }
    lock.lock();
    // Wait for a short interval or until the condition_variable is notified
    bonusCV.wait_for(lock, std::chrono::milliseconds(500));
  }
}