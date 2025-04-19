#include <stdlib.h>

#include <character.hpp>

Player::Player(Point p) {
  // player id always 0
  this->name = "Player";
  this->desc = "mc frfr";
  this->color = WHITE;
  this->hp = 100;
  this->dam = Dice(0, 1, 4);

  this->id = 0;
  this->pos = p;
  this->speed = 10;
  this->sprite = '@';
  this->alive = 1;
  this->type = PLAYER;
};

Monster::Monster(int id) {
  this->id = id;
  this->speed = (rand() % 21) + 5;
  this->sprite = "0123456789ABCDEF"[this->traits & 0xF]; // get hex char
  this->alive = 1;
  this->type = MONSTER;

  this->traits = rand() % 16;
};

Monster::Monster(int id, std::string name, std::string desc, COLOR color,
                 int speed, int abil, int hp, Dice dam, char symb, int rrty) {
  this->id = id;
  this->name = name;
  this->desc = desc;
  this->color = color;
  this->speed = speed;
  this->traits = abil;
  this->hp = hp;
  this->dam = dam;
  this->sprite = symb;
  this->rrty = rrty;
  this->alive = 1;
  this->type = MONSTER;
};
