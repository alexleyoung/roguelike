#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>

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

enum COLORS { RED, GREEN, BLUE, CYAN, YELLOW, MAGENTA, WHITE, BLACK };

class Monster_Description {
public:
  // terminated by \n
  std::string name;
  // must terminate by byte 78 with \n
  std::string description;
  // space-separated list of colors
  std::string color;
  // TODO: MAKE DICE
  std::string speed;
  // space-separated keywords
  std::string abilities;
  int hp;
  std::string ad;
  char symbol;
  int rarity;
};

#endif
