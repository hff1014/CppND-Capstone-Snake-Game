#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
//#include "SDL.h"
#include "SDL_image.h"
//#include <SDL_mixer.h>
#include "snake.h"

//#include "LTexture.h"

class Renderer {
 public:
  Renderer(const std::size_t screen_width, const std::size_t screen_height,
           const std::size_t grid_width, const std::size_t grid_height, std::vector<std::shared_ptr<std::string>>  paths);
  ~Renderer();

  void Render(std::vector<std::shared_ptr<Snake>> snakes_, SDL_Point const &food, std::vector<std::shared_ptr<SDL_Point>> obstacles);
  void UpdateWindowTitle(int score_left, int score_right, int fps);
  void Render_End();

 private:
  SDL_Window *sdl_window;
  SDL_Renderer *sdl_renderer;

  const std::size_t screen_width;
  const std::size_t screen_height;
  const std::size_t grid_width;
  const std::size_t grid_height;
  std::shared_ptr<std::string> start_path;
  std::shared_ptr<std::string> end_path;
  
};

#endif