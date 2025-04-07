#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "dungeon.hpp"

int calc_dists(Dungeon *dungeon, int map[DUNGEON_HEIGHT][DUNGEON_WIDTH],
               Point source, int enable_tunnels);

#endif
