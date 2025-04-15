#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <types.hpp>

#define PLAYER_TRAIT 16

enum CHARACTER_TYPE { PLAYER, MONSTER };

class Character {
public:
  virtual ~Character() = default;
  int id;
  point pos;
  int speed;
  char sprite;
  int alive;
  CHARACTER_TYPE type;
};

class Player : public Character {
public:
  char terrain[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {};
  Character *characters[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {};
};

class Monster : public Character {
public:
  int traits;
  int dist_to_player[DUNGEON_HEIGHT][DUNGEON_WIDTH];
};

#endif
