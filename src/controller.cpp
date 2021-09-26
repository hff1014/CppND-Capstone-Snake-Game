#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"


void Controller::ChangeDirection(std::shared_ptr<Snake> snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake->direction != opposite || snake->size == 1) snake->direction = input;
  return;
}


int Controller::HandleInput(int &cur_state, std::vector<std::shared_ptr<Snake>> snakes_) const {
  SDL_Event e;
  int ret = cur_state;
  std::shared_ptr<Snake> snake_left = snakes_[0];
  std::shared_ptr<Snake> snake_right = snakes_[1];

  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      ret = 4;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        //control right snake move
        case SDLK_UP:
          ChangeDirection(snake_right, Snake::Direction::kUp,
                          Snake::Direction::kDown);
          break;

        case SDLK_DOWN:
          ChangeDirection(snake_right, Snake::Direction::kDown,
                          Snake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake_right, Snake::Direction::kLeft,
                          Snake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake_right, Snake::Direction::kRight,
                          Snake::Direction::kLeft);
          break;

        //control left snake move
        case SDLK_w:
          ChangeDirection(snake_left, Snake::Direction::kUp,
                          Snake::Direction::kDown);
          break;

        case SDLK_z:
          ChangeDirection(snake_left, Snake::Direction::kDown,
                          Snake::Direction::kUp);
          break;

        case SDLK_a:
          ChangeDirection(snake_left, Snake::Direction::kLeft,
                          Snake::Direction::kRight);
          break;

        case SDLK_d:
          ChangeDirection(snake_left, Snake::Direction::kRight,
                          Snake::Direction::kLeft);
          break;

        case SDLK_SPACE:
          ret = 2;
          break;

        case SDLK_RETURN:
          if (ret == 2) ret = 1;
          break; 

      }
      
      }
  }
  return ret;
}

int Controller::HandleMouseInput(int &cur_state, LButton &button) const {
      SDL_Event e;
      int ret = cur_state;
      while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
          ret = 4;
          } else {
            button.handleEvent(e);
        }
      }
      return ret;
}

int Controller::HandleInput(int& cur_state) const {
    SDL_Event e;
    int ret = cur_state;
    while (SDL_PollEvent(&e)) {
      if (e.type == SDL_QUIT) {
        ret = 4;
        }
    }
    return ret;

}