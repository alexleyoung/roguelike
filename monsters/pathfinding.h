#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "../dsa/types.h"

int calc_dists(dungeon *dungeon, int map[DUNGEON_HEIGHT][DUNGEON_WIDTH], point source);
int calc_tunnel_dists(dungeon *dungoen, int map[DUNGEON_HEIGHT][DUNGEON_WIDTH], point source);

#endif
