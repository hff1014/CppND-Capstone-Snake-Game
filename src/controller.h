#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
#include "LButton.h"


class Controller {
 public:
  

  int HandleInput(int& cur_state, std::vector<std::shared_ptr<Snake>> snakes_) const;
  int  HandleMouseInput(int &cur_state, LButton &button) const;
  int HandleInput(int& cur_state) const;

 private:
  void ChangeDirection(std::shared_ptr<Snake> snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

#endif