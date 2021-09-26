#include "game.h"
#include <iostream>
#include "SDL.h"
#include <memory>

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : engine(dev()),
      //add a private member variable gstate to reflect game state {init, run, pause, end}
      gstate(INIT),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {

  InitSnakes(grid_width, grid_height);
  PlaceFood();

  for (int i = 0; i < obst_size; ++i) {
    std::shared_ptr<SDL_Point> obstacle = std::make_shared<SDL_Point>();
    PlaceObstacle(obstacle);
    obstacles.emplace_back(obstacle);
  }
}

// initialize two snakes 
void Game::InitSnakes(std::size_t grid_width, std::size_t grid_height) {

    std::shared_ptr<Snake> snake_left = std::make_shared<Snake>(grid_width, grid_height, 0);
    std::shared_ptr<Snake> snake_right = std::make_shared<Snake>(grid_width, grid_height, 1);
    snakes_.emplace_back(snake_left);
    snakes_.emplace_back(snake_right);
}

//getter and setter state functions
GameState Game::getState() const{
  return gstate;
}

void Game::setState(GameState state) {
  gstate = state;
}


void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration, LButton &button) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;

  static int cur_state = gstate;


  while (gstate != QUIT) {
    frame_start = SDL_GetTicks();

    //update current state with game state
    cur_state = gstate;
    // Input, Update, Render - the main game loop.

    //switch state machine between between different game states
    switch (gstate) {

       case INIT:
           gstate = (GameState)controller.HandleMouseInput(cur_state, button);
           //if with button range, user click mouse, we can start the game
           if (button.getCurrentSprite() == LButtonSprite::BUTTON_SPRITE_MOUSE_DOWN) {
             gstate = RUN;
           }
           break;

       case RUN:
           gstate = (GameState)controller.HandleInput(cur_state, snakes_);
           //unless user hit pause, we should keep update the snakes' location
           if (gstate != GameState::PAUSE) {
             Update();
           } 
           renderer.Render(snakes_, food, obstacles);
           break;

       case PAUSE:
           // if user hit "ENTER KEY", we can resume the game.
           gstate = (GameState)controller.HandleInput(cur_state, snakes_);
           break;
           
       case END:
           // both users hit dead snake, we display game-over screen
           renderer.Render_End();
           gstate = (GameState)controller.HandleInput(cur_state);
           break;

       case QUIT:
           break;    

    }

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score_left, score_right, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item or a obstacle before placing
    // food 
    bool occupied = false;

    for (auto const snake : snakes_) {
      if (snake->SnakeCell(x, y)) {
        occupied = true;
        break;
      }
    }
    
    for (auto const item : obstacles) {
      if (item->x == x && item->y == y) {
        occupied = true;
        break;
      }
    }
    
    if (occupied) {
      continue;
    }
    food.x = x;
    food.y = y;
    return;
    
  }
}


void Game::PlaceObstacle(std::shared_ptr<SDL_Point> &obstacle) {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item or food before placing
    // obstacle.
    bool occupied = false;

    for (auto const snake : snakes_) {
      if (snake->SnakeCell(x, y)) {
        occupied = true;
        break;
      }
    }

    for (auto const item : obstacles) {
      if (item->x == x && item->y == y) {
        occupied = true;
        break;
      }
    }
    
    if (occupied || (food.x == x && food.y == y)) {
      occupied = true;
      continue;
    }
    obstacle->x = x;
    obstacle->y = y;
    return;
  }
}

void Game::GameOver() {
    return;
}

void Game::Update() {

  int dead_cnt = 0;
  for (auto& snake : snakes_) {
    if (!snake->alive) {
      dead_cnt++;
      continue;
    }
    snake->Update();
    int new_x1 = static_cast<int>(snake->head_x);
    int new_y1 = static_cast<int>(snake->head_y);

    if (food.x == new_x1 && food.y == new_y1) {
      if (snake->getId() == 0) score_left++;
      if (snake->getId() == 1) score_right++;
      PlaceFood();
      // Grow snake and increase speed.
      snake->GrowBody();
      snake->speed += 0.02;
    }

    for (auto const obstacle : obstacles){
      if (obstacle->x == new_x1 && obstacle->y == new_y1) {
        snake->alive = false;
        break;
      }
    }

  }
   // if both players come to dead snake, change game state to END
   if (dead_cnt == 2) {
     gstate = GameState::END;
     return;
  }

}

int Game::GetRightScore() const { return score_right; }
int Game::GetLeftScore() const { return score_left; }
int Game::GetRightSize() const { return snakes_[1]->size; }
int Game::GetLeftSize() const { return snakes_[0]->size;}