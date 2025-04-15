#ifndef DESCRIPTIONS_HPP
#define DESCRIPTIONS_HPP

#include <iostream>
#include <string>

#include <character.hpp>
#include <saves.hpp>

enum COLORS { RED, GREEN, BLUE, CYAN, YELLOW, MAGENTA, WHITE, BLACK };
enum ITEM_TYPE {
  WEAPON,
  OFFHAND,
  RANGED,
  ARMOR,
  HELMET,
  CLOAK,
  GLOVES,
  BOOTS,
  RING,
  AMULET,
  LIGHT,
  SCROLL,
  BOOK,
  FLASK,
  GOLD,
  AMMUNITION,
  FOOD,
  WAND,
  CONTAINER
};

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
    std::cout << description;
    std::cout << symbol << std::endl;
    std::cout << color << std::endl;
    std::cout << speed << std::endl;
    std::cout << abilities << std::endl;
    std::cout << hp << std::endl;
    std::cout << ad << std::endl;
    std::cout << rarity << std::endl;
  }
};

class Item_Description {
public:
  std::string name;
  std::string description;
  std::string color;
  std::string speed;
  std::string abilities;
  std::string hp;
  std::string ad;
  std::string symbol;
  std::string rarity;
};

int load_monster_descriptions(const char *);
int load_object_descriptions(const char *);

#endif
