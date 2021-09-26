#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"


int main() {
  constexpr std::size_t kFramesPerSecond{40};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

 //pass two images path into renderer to display images accordingly 
  std::shared_ptr<std::string> start_path = std::make_shared<std::string>("../res/start.bmp");
  std::shared_ptr<std::string>  end_path = std::make_shared<std::string>("../res/game_over.bmp");
  std::vector<std::shared_ptr<std::string>>  paths;
  paths.emplace_back(start_path);
  paths.emplace_back(end_path);

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight, paths);
 
 //define a button click to control whether user starts game or not
  LButton button;
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  game.Run(controller, renderer, kMsPerFrame, button);
  

  std::cout << "Game has terminated successfully!\n";
  std::cout << "Left Score: " << game.GetLeftScore() << "\n";
  std::cout << "Left Size: " << game.GetLeftSize() << "\n";
  std::cout << "Right Score: " << game.GetRightScore() << "\n";
  std::cout << "Right Size: " << game.GetRightSize() << "\n";
  return 0;
}