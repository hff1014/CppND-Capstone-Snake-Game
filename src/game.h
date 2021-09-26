#ifndef GAME_H
#define GAME_H

#include <random>
#include <memory>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

//define game states enums
enum GameState {
INIT,
RUN,
PAUSE,
END,
QUIT
};

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);

  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration, LButton &button);


  int GetLeftScore() const;
  int GetRightScore() const;

  int GetLeftSize() const;
  int GetRightSize() const;

  GameState getState() const;
  void setState(GameState state);

 private:
  std::vector<std::shared_ptr<Snake>> snakes_;

  SDL_Point food;

  // define a vector of obstacles
  std::vector<std::shared_ptr<SDL_Point>> obstacles;
  const int obst_size = 8;
  
  //game state variable to change game states
  GameState gstate;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score_left{0};
  int score_right{0};

  void InitSnakes(std::size_t grid_width, std::size_t grid_height);
  void PlaceFood();
  void Update();
  void GameOver();
  void PlaceObstacle(std::shared_ptr<SDL_Point> &obstacle);


};

#endif