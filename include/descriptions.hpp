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
private:
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

public:
  Object_Description() {}
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
    std::string val;

    try {
      std::stringstream ss(hit);
      // base
      std::getline(ss, val, '+');
      hit_dice.set_base(std::stoi(val));

      // # of dice
      std::getline(ss, val, 'd');
      hit_dice.set_number(std::stoi(val));

      // sides
      std::getline(ss, val);
      hit_dice.set_sides(std::stoi(val));
    } catch (...) {
      return false;
    }

    this->hit = hit_dice;
    return true;
  }

  bool set_dam(std::string dam) {
    Dice dam_dice;
    std::string val;

    try {
      std::stringstream ss(dam);
      // base
      std::getline(ss, val, '+');
      dam_dice.set_base(std::stoi(val));

      // # of dice
      std::getline(ss, val, 'd');
      dam_dice.set_number(std::stoi(val));

      // sides
      std::getline(ss, val);
      dam_dice.set_sides(std::stoi(val));
    } catch (...) {
      return false;
    }

    this->dam = dam_dice;
    return true;
  }

  bool set_dodge(std::string dodge) {
    Dice dodge_dice;
    std::string val;

    try {
      std::stringstream ss(dodge);
      // base
      std::getline(ss, val, '+');
      dodge_dice.set_base(std::stoi(val));

      // # of dice
      std::getline(ss, val, 'd');
      dodge_dice.set_number(std::stoi(val));

      // sides
      std::getline(ss, val);
      dodge_dice.set_sides(std::stoi(val));
    } catch (...) {
      return false;
    }

    this->dodge = dodge_dice;
    return true;
  }

  bool set_def(std::string def) {
    Dice def_dice;
    std::string val;

    try {
      std::stringstream ss(def);
      // base
      std::getline(ss, val, '+');
      def_dice.set_base(std::stoi(val));

      // # of dice
      std::getline(ss, val, 'd');
      def_dice.set_number(std::stoi(val));

      // sides
      std::getline(ss, val);
      def_dice.set_sides(std::stoi(val));
    } catch (...) {
      return false;
    }

    this->def = def_dice;
    return true;
  }

  bool set_weight(std::string weight) {
    Dice weight_dice;
    std::string val;

    try {
      std::stringstream ss(weight);
      // base
      std::getline(ss, val, '+');
      weight_dice.set_base(std::stoi(val));

      // # of dice
      std::getline(ss, val, 'd');
      weight_dice.set_number(std::stoi(val));

      // sides
      std::getline(ss, val);
      weight_dice.set_sides(std::stoi(val));
    } catch (...) {
      return false;
    }

    this->def = weight_dice;
    return true;
  }

  bool set_speed(std::string speed) {
    Dice speed_dice;
    std::string val;

    try {
      std::stringstream ss(speed);
      // base
      std::getline(ss, val, '+');
      speed_dice.set_base(std::stoi(val));

      // # of dice
      std::getline(ss, val, 'd');
      speed_dice.set_number(std::stoi(val));

      // sides
      std::getline(ss, val);
      speed_dice.set_sides(std::stoi(val));
    } catch (...) {
      return false;
    }

    this->speed = speed_dice;
    return true;
  }

  bool set_attr(std::string attr) {
    Dice attr_dice;
    std::string val;

    try {
      std::stringstream ss(attr);

      // base
      std::getline(ss, val, '+');
      attr_dice.set_base(std::stoi(val));

      // # of dice
      std::getline(ss, val, 'd');
      attr_dice.set_number(std::stoi(val));

      // sides
      std::getline(ss, val);
      attr_dice.set_sides(std::stoi(val));
    } catch (...) {
      return false;
    }

    this->attr = attr_dice;
    return true;
  }

  bool set_val(std::string val) {
    Dice val_dice;
    std::string dice_val;

    try {
      std::stringstream ss(val);

      // base
      std::getline(ss, dice_val, '+');
      val_dice.set_base(std::stoi(dice_val));

      // # of dice
      std::getline(ss, dice_val, 'd');
      val_dice.set_number(std::stoi(dice_val));

      // sides
      std::getline(ss, dice_val);
      val_dice.set_sides(std::stoi(dice_val));
    } catch (...) {
      return false;
    }

    this->val = val_dice;
    return true;
  }

  bool set_art(std::string art) {
    if (art == "FALSE")
      this->art = false;
    else if (art == "TRUE")
      this->art = true;
    else
      return false;
    return true;
  }

  bool set_rrty(std::string rrty) {
    try {
      this->rrty = std::stoi(rrty);
    } catch (...) {
      return false;
    }
    return true;
  }

  void print_info() {
    std::cout << "Name: " << name << "\n";
    std::cout << "Description:\n" << desc;
    std::cout << "Type: " << type << "\n";
    std::cout << "Color(s): ";
    for (int i = 0; i < 8; ++i) {
      // Assuming COLOR has a sentinel like COLOR_NONE or similar to mark unused
      // slots Otherwise, check against an internal count or null/default value
      std::cout << color[i] << " ";
    }
    std::cout << "\n";

    std::cout << "Hit: " << hit << "\n";
    std::cout << "Damage: " << dam << "\n";
    std::cout << "Dodge: " << dodge << "\n";
    std::cout << "Defense: " << def << "\n";
    std::cout << "Weight: " << weight << "\n";
    std::cout << "Speed: " << speed << "\n";
    std::cout << "Attribute: " << attr << "\n";
    std::cout << "Value: " << val << "\n";
    std::cout << "Artifact: " << (art ? "Yes" : "No") << "\n";
    std::cout << "Rarity: " << rrty << "\n";
  }
};

int load_monster_descriptions(const char *);
int load_object_descriptions(const char *);

#endif
