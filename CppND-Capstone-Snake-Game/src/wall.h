#ifndef WALL_H
#define WALL_H

#include <string>


class Wall {
  
public:
  void setWall(int wall_width, int wall_height);
  Wall(int wall_width, int wall_height)
      : wall_width(wall_width),
        wall_height(wall_height){;}

  
private:

  int wall_width;
  int wall_height;
};

#endif