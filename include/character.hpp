#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "types.hpp"

#define PLAYER_TRAIT 16

class character {
public:
  virtual ~character() = default;
  int id;
  point pos;
  int speed;
  char sprite;
  int alive;
};

class player : public character {
public:
  int terrain[DUNGEON_HEIGHT][DUNGEON_WIDTH];
};

class monster : public character {
public:
  int traits;
  int dist_to_player[DUNGEON_HEIGHT][DUNGEON_WIDTH];
};

#endif
