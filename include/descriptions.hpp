#ifndef DESCRIPTIONS_HPP
#define DESCRIPTIONS_HPP

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <character.hpp>
#include <dice.hpp>
#include <saves.hpp>

enum COLOR { RED, GREEN, BLUE, CYAN, YELLOW, MAGENTA, WHITE, BLACK };
enum OBJECT_TYPE {
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

class Object_Description {
public:
  std::string name;
  std::string desc;
  OBJECT_TYPE type;
  COLOR color[8];
  Dice hit;
  Dice dam;
  Dice dodge;
  Dice def;
  Dice weight;
  Dice speed;
  Dice attr;
  Dice val;
  bool art;
  int rrty;

  Object_Description() {};
  ~Object_Description() {};

  bool set_name(std::string name) {
    this->name = name;
    return true;
  }
  bool set_desc(std::string desc) {
    this->desc = desc;
    return true;
  }
  bool set_type(std::string type) {
    if (type == "WEAPON")
      this->type = WEAPON;
    else if (type == "OFFHAND")
      this->type = OFFHAND;
    else if (type == "RANGED")
      this->type = RANGED;
    else if (type == "ARMOR")
      this->type = ARMOR;
    else if (type == "HELMET")
      this->type = HELMET;
    else if (type == "CLOAK")
      this->type = CLOAK;
    else if (type == "GLOVES")
      this->type = GLOVES;
    else if (type == "BOOTS")
      this->type = BOOTS;
    else if (type == "RING")
      this->type = RING;
    else if (type == "AMULET")
      this->type = AMULET;
    else if (type == "LIGHT")
      this->type = LIGHT;
    else if (type == "SCROLL")
      this->type = SCROLL;
    else if (type == "BOOK")
      this->type = BOOK;
    else if (type == "FLASK")
      this->type = FLASK;
    else if (type == "GOLD")
      this->type = GOLD;
    else if (type == "AMMUNITION")
      this->type = AMMUNITION;
    else if (type == "FOOD")
      this->type = FOOD;
    else if (type == "WAND")
      this->type = WAND;
    else if (type == "CONTAINER")
      this->type = CONTAINER;
    else {
      return false;
    }
    return true;
  }
  bool set_color(std::string color) {
    int count = 0;
    std::stringstream ss(color);

    std::string cur_color;
    while (count < 8 && std::getline(ss, cur_color, ' ')) {
      if (cur_color == "RED")
        this->color[count] = RED;
      else if (cur_color == "GREEN")
        this->color[count] = GREEN;
      else if (cur_color == "BLUE")
        this->color[count] = BLUE;
      else if (cur_color == "CYAN")
        this->color[count] = CYAN;
      else if (cur_color == "YELLOW")
        this->color[count] = YELLOW;
      else if (cur_color == "MAGENTA")
        this->color[count] = MAGENTA;
      else if (cur_color == "WHITE")
        this->color[count] = WHITE;
      else if (cur_color == "BLACK")
        this->color[count] = BLACK;
      else {
        continue;
      }
      count++;
    }

    return true;
  }
  bool set_hit(std::string hit) {
    Dice hit_dice;
    this->hit = hit_dice;
    return true;
  }
  bool set_dam(std::string dam) {
    Dice dam_dice;
    this->dam = dam_dice;
    return true;
  }
  bool set_dodge(std::string dodge) {
    Dice dodge_dice;
    this->dodge = dodge_dice;
    return true;
  }
  bool set_def(std::string def) {
    Dice def_dice;
    this->def = def_dice;
    return true;
  }
  bool set_speed(std::string speed) {
    Dice speed_dice;
    this->speed = speed_dice;
    return true;
  }
  bool set_attr(std::string attr) {
    Dice attr_dice;
    this->attr = attr_dice;
    return true;
  }
  bool set_val(std::string val) {
    Dice val_dice;
    this->val = val_dice;
    return true;
  }
  bool set_art(std::string art) {
    this->art = FALSE;
    return true;
  }
  bool set_rrty(std::string rrty) {
    this->rrty = rrty;
    return true;
  }
};

int load_monster_descriptions(const char *);
int load_object_descriptions(const char *);

#endif
