#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "dungeon.hpp"

int calc_dists(dungeon *dungeon, int map[DUNGEON_HEIGHT][DUNGEON_WIDTH],
               point source, int enable_tunnels);

#endif
