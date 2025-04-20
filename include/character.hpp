#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include <string>

#include <dice.hpp>
#include <object.hpp>
#include <types.hpp>
#include <utils.hpp>

class Monster_Description;

#define NUM_INVENTORY_SLOTS 10

#define NUM_ABILITIES 9
enum ABILITY {
  SMART = 1,
  TELE = 2,
  TUNNEL = 4,
  ERRATIC = 8,
  PASS = 16,
  PICKUP = 32,
  DESTROY = 64,
  UNIQ = 128,
  BOSS = 256,
  INVALID_ABILITY = -1
};

enum CHARACTER_TYPE { PLAYER, MONSTER };

#define NUM_EQUIPMENT_SLOTS 12
enum EQUIPMENT_SLOTS {
  WEAPON_SLOT,
  OFFHAND_SLOT,
  RANGED_SLOT,
  ARMOR_SLOT,
  HELMET_SLOT,
  CLOAK_SLOT,
  GLOVES_SLOT,
  BOOTS_SLOT,
  AMULET_SLOT,
  LIGHT_SLOT,
  RING1_SLOT,
  RING2_SLOT
};

class Character {
public:
  std::string name;
  std::string desc;
  COLOR color;
  int hp;
  Dice dam;

  int id;
  point pos;
  int speed;
  char sprite;
  int alive;
  CHARACTER_TYPE type;

  virtual ~Character() = default;
};

class Player : public Character {
public:
  /*char terrain[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {};*/
  /*Character *characters[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {};*/

  Object *equipment[NUM_EQUIPMENT_SLOTS] = {};
  Object *inventory[NUM_INVENTORY_SLOTS] = {};

  Player(Point p);
};

class Monster : public Character {
public:
  int traits;
  int rrty;
  int dist_to_player[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {};

  Monster(int id);
  Monster(int id, std::string name, std::string desc, COLOR color, int speed,
          int abil, int hp, Dice dam, char symb, int rrty);
  Monster(Monster_Description);
};

#endif
