#include <stdlib.h>

#include "spawn.h"

int create_player(character *c, point p) {
  // player id always 0
  c->id = 0;
  c->traits = PLAYER_TRAIT; // special identifier for PC
  c->speed = 10;
  c->pos = p;
  c->sprite = '@';
  c->alive = 1;

  return 0;
}

int create_monster(character *c, int id) {
  c->id = id;
  c->traits = rand() % 15;
  c->speed = (rand() % 21) + 5;
  c->sprite = "0123456789ABCDEF"[c->traits & 0xF]; // get hex char
  c->alive = 1;

  return 0;
}
