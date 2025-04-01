#include <stdlib.h>

#include <spawn.hpp>

int create_player(player *c, point p) {
  // player id always 0
  c->id = 0;
  c->speed = 10;
  c->pos = p;
  c->sprite = '@';
  c->alive = 1;
  c->type = PLAYER;

  return 0;
}

int create_monster(monster *c, int id) {
  c->id = id;
  c->traits = rand() % 16;
  c->speed = (rand() % 21) + 5;
  c->sprite = "0123456789ABCDEF"[c->traits & 0xF]; // get hex char
  c->alive = 1;
  c->type = MONSTER;

  return 0;
}
