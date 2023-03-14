#include "controller.h"
#include "game.h"
#include "custom_window.h"
#include "renderer.h"
#include <iostream>
#include <fstream>
#include <string>
#include "snake.h"
#include "wall.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight);
  game.Run(controller, renderer, kMsPerFrame);
  
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Score: " << game.GetScore() << "\n";
  std::cout << "Size: " << game.GetSize() << "\n";
  std::string file_path = __FILE__;
  std::string dir_path = file_path.substr(0, file_path.rfind("\\"));
  std::string dir_path_name = dir_path.substr(0, dir_path.find_last_of("\\/"));
  std::string dir_path_name_total = dir_path_name +"/previous_score.txt";
  std::string line;
  std::string input;
  std::ifstream myfile (dir_path_name_total);

 
  if (myfile.is_open()) 
  {
    while ( std::getline (myfile,line) )
    {
      std::cout << "Previous highest score was:";
      std::cout << line << '\n';
    }
    myfile.close();
  }

  else std::cout << "Unable to open previous score"<< "\n";

  int prev_score = std::stoi( line );

  if (prev_score < game.GetScore())
    {
      std::cout << "Current score higher than previous score! Congrats"<< "\n";
      std::cout << "Would you like to overwrite the previous score (y or n)" <<"?\n";
      std::getline (std::cin,input);
      if(input == "y")
      {
        std::ofstream myfile (dir_path_name_total);
        if (myfile.is_open())
        {
          myfile << game.GetScore();
          myfile.close();
        }
      }
      else
      std::cout << "Ok, Goodbye."<< "\n";
    }

  return 0;
}