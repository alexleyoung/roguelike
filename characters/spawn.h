#ifndef SPAWN_H
#define SPAWN_H

#include "../types/character.h"

int create_player(character *c, point p);
int create_monster(character *c, int id);

#endif
