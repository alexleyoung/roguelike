#ifndef DESCRIPTIONS_HPP
#define DESCRIPTIONS_HPP

#include <string>
#include <vector>

#include <character.hpp>
#include <dice.hpp>
#include <utils.hpp>

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
private:
  std::string name;
  std::string desc;
  COLOR color[NUM_COLORS];
  Dice speed;
  ABILITY abil[NUM_ABILITIES];
  Dice hp;
  Dice dam;
  char symb;
  int rrty;

public:
  Monster_Description() {
    std::fill_n(color, NUM_COLORS, INVALID_COLOR);
    std::fill_n(abil, NUM_ABILITIES, INVALID_ABILITY);
  }
  ~Monster_Description() {}

  bool set_name(const std::string &);
  bool set_desc(const std::string &);
  bool set_color(const std::string &);
  bool set_speed(const std::string &);
  bool set_hp(const std::string &);
  bool set_dam(const std::string &);
  bool set_abil(const std::string &);
  bool set_symb(const std::string &);
  bool set_rrty(const std::string &);

  Monster *generate(int id);

  void print_info();
};

class Object_Description {
private:
  std::string name;
  std::string desc;
  OBJECT_TYPE type;
  COLOR color[NUM_COLORS];
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
  Object_Description() { std::fill_n(color, NUM_COLORS, INVALID_COLOR); };
  ~Object_Description() {}

  bool set_name(const std::string &);
  bool set_desc(const std::string &);
  bool set_type(const std::string &);
  bool set_color(const std::string &);

  bool set_hit(const std::string &);
  bool set_dam(const std::string &);
  bool set_dodge(const std::string &);
  bool set_def(const std::string &);
  bool set_weight(const std::string &);
  bool set_speed(const std::string &);
  bool set_attr(const std::string &);
  bool set_val(const std::string &);
  bool set_art(const std::string &);
  bool set_rrty(const std::string &);
  void print_info();

  /*Object *generate();*/
};

std::vector<Monster_Description> load_monster_descriptions(const char *);
std::vector<Object_Description> load_object_descriptions(const char *);

#endif
