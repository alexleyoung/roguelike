#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <iostream>
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
  std::string hp;
  std::string ad;
  std::string symbol;
  std::string rarity;

  Monster_Description(std::string name, std::string description,
                      std::string color, std::string speed,
                      std::string abilities, std::string hp, std::string ad,
                      std::string symbol, std::string rarity)
      : name(name), description(description), color(color), speed(speed),
        abilities(abilities), hp(hp), ad(ad), symbol(symbol), rarity(rarity) {};
  ~Monster_Description() {};

  void print_info() {
    std::cout << name << std::endl;
    std::cout << description << std::endl;
    std::cout << color << std::endl;
    std::cout << speed << std::endl;
    std::cout << abilities << std::endl;
    std::cout << hp << std::endl;
    std::cout << ad << std::endl;
    std::cout << symbol << std::endl;
    std::cout << rarity << std::endl;
  }
};

#endif
