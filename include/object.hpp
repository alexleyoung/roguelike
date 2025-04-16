#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <string>

#include <dice.hpp>
#include <types.hpp>
#include <utils.hpp>

class Object_Description;

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

class Object {
public:
  std::string name;
  std::string desc;
  OBJECT_TYPE type;
  COLOR color;
  int hit;
  Dice dam;
  int dodge;
  int def;
  int weight;
  int speed;
  int attr;
  int val;
  bool art;
  int rrty;

  char sprite;
  Point pos;

  Object(std::string name, std::string desc, OBJECT_TYPE type, COLOR color,
         int hit, Dice dam, int dodge, int def, int weight, int speed, int attr,
         int val, bool art, int rrty)
      : name(name), desc(desc), type(type), color(color), hit(hit), dam(dam),
        dodge(dodge), def(def), weight(weight), speed(speed), attr(attr),
        val(val), art(art), rrty(rrty) {
    switch (type) {
    case WEAPON:
      sprite = '|';
      break;
    case OFFHAND:
      sprite = ')';
      break;
    case RANGED:
      sprite = '}';
      break;
    case ARMOR:
      sprite = '[';
      break;
    case HELMET:
      sprite = ']';
      break;
    case CLOAK:
      sprite = '(';
      break;
    case GLOVES:
      sprite = '{';
      break;
    case BOOTS:
      sprite = '\\';
      break;
    case RING:
      sprite = '=';
      break;
    case AMULET:
      sprite = '"';
      break;
    case LIGHT:
      sprite = '_';
      break;
    case SCROLL:
      sprite = '~';
      break;
    case BOOK:
      sprite = '?';
      break;
    case FLASK:
      sprite = '!';
      break;
    case GOLD:
      sprite = '$';
      break;
    case AMMUNITION:
      sprite = '/';
      break;
    case FOOD:
      sprite = ',';
      break;
    case WAND:
      sprite = '-';
      break;
    case CONTAINER:
      sprite = '%';
      break;
    default:
      sprite = '*';
      break;
    }
  }
};

#endif
