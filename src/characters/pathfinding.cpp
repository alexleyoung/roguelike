#include <stdio.h>
#include <stdlib.h>

#include <corridor_heap.hpp>
#include <pathfinding.hpp>

#define INF 2147483647

int calc_dists(Dungeon *dungeon, int map[DUNGEON_HEIGHT][DUNGEON_WIDTH],
               point source, int enable_tunnels) {
  for (int r = 0; r < DUNGEON_HEIGHT; r++) {
    for (int c = 0; c < DUNGEON_WIDTH; c++) {
      map[r][c] = INF;
    }
  }

  corridor_heap h;
  corridor_heap_init(&h);
  corridor_heap_push(&h, source, 0);

  while (!corridor_heap_is_empty(&h)) {
    point p; // point
    int d;   // distance
    corridor_heap_pop(&h, &p, &d);
    int hardness = dungeon->tiles[p.r][p.c].hardness;

    for (uint8_t r = p.r - 1; r <= p.r + 1; r++) {
      for (uint8_t c = p.c - 1; c <= p.c + 1; c++) {
        if ((r == p.r && c == p.c) || !IN_BOUNDS(r, c)) {
          continue;
        }
        if (!enable_tunnels &&
            dungeon->tiles[r][c]
                .hardness) { // if tunnels disabled and hardness not 0, skip
          continue;
        }

        int w = 1 + (hardness / 85);
        int new_dist = d + w;
        if (new_dist < map[r][c]) {
          map[r][c] = new_dist;
          corridor_heap_push(&h, (point){r, c}, new_dist);
        }
      }
    }
  }

  return 0;
}
