#ifndef SAVES_H
#define SAVES_H

#include "../dsa/types.h"

int save_dungeon(dungeon *dungeon, const char *name);
int load_dungeon(dungeon *dungeon, const char *name);

#endif