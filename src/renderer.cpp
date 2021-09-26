#include "renderer.h"
//#include <SDL_image.h>
//#include <SDL_mixer.h>
#include <iostream>
#include <string>



Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height, std::vector<std::shared_ptr<std::string>> paths)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height) {
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
  
  // Create Window
  sdl_window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN);

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
  }

  // Create renderer
  sdl_renderer = SDL_CreateRenderer(sdl_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
  }
  start_path = paths[0];
  end_path = paths[1];

 // load the start image to screen and display the image

  SDL_Surface *surface = SDL_LoadBMP((*start_path).c_str());
  if (!surface) {
        std::cerr << "Failed to created a surface!!\n";
  }
  SDL_Texture*  mTexture = SDL_CreateTextureFromSurface(sdl_renderer, surface);

  SDL_FreeSurface(surface);

  if (!mTexture) {
        std::cerr << "Failed to created a texture!!\n";
  }

  if (mTexture) {
        SDL_RenderClear(sdl_renderer);
        SDL_RenderCopy(sdl_renderer, mTexture, nullptr, nullptr);
        SDL_RenderPresent(sdl_renderer);
        SDL_Delay(1000);
  }

}


// render game_over images when game state is END
void Renderer::Render_End()
 {

 // load the end image to screen and display the image
  SDL_Surface *surface = SDL_LoadBMP((*end_path).c_str());
  if (!surface) {
        std::cerr << "Failed to created a surface!!\n";
  }
  SDL_Texture*  mTexture = SDL_CreateTextureFromSurface(sdl_renderer, surface);

  SDL_FreeSurface(surface);

  if (!mTexture) {
        std::cerr << "Failed to created a texture!!\n";
  }

  if (mTexture) {
        SDL_RenderClear(sdl_renderer);
        SDL_RenderCopy(sdl_renderer, mTexture, nullptr, nullptr);
        SDL_RenderPresent(sdl_renderer);
        SDL_Delay(1000);
  }

}

Renderer::~Renderer() {
  SDL_DestroyWindow(sdl_window);
  SDL_Quit();
}

void Renderer::Render(std::vector<std::shared_ptr<Snake>> snakes_, SDL_Point const &food, std::vector<std::shared_ptr<SDL_Point>> obstacles) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  std::shared_ptr<Snake> snake_left = snakes_[0];
  std::shared_ptr<Snake> snake_right = snakes_[1];

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer, 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer);

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer, &block);

  // Render obstacle
  for (auto const obstacle : obstacles) {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
    block.x = obstacle->x * block.w;
    block.y = obstacle->y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snakes's body
  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake_left->body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake_right->body) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer, &block);
  }

  // Render snakes's head
  block.x = static_cast<int>(snake_left->head_x) * block.w;
  block.y = static_cast<int>(snake_left->head_y) * block.h;
  if (snake_left->alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  block.x = static_cast<int>(snake_right->head_x) * block.w;
  block.y = static_cast<int>(snake_right->head_y) * block.h;

  if (snake_right->alive) {
    SDL_SetRenderDrawColor(sdl_renderer, 0x00, 0xBB, 0x33, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer, 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer, &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer);
}

void Renderer::UpdateWindowTitle(int score_left, int score_right, int fps) {
  std::string title{"Left Score: " + std::to_string(score_left) + " V.S " + 
  "Right Score: " + std::to_string(score_right) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window, title.c_str());
}
