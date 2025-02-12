#include <stdlib.h>
#include <stdio.h>

#include "pathfinding.h"
#include "../dsa/algs.h"
#include "../dsa/queue.h"
#include "../dsa/corridor_heap.h"

#define INF 21448364//7

int calc_dists(dungeon *dungeon, int map[DUNGEON_HEIGHT][DUNGEON_WIDTH], point source, int enable_tunnels) {
    for (int r = 0; r < DUNGEON_HEIGHT; r++) {
        for (int c = 0; c < DUNGEON_WIDTH; c++) {
            map[r][c] = INF;
        }
    }
    
    heap h;
    heap_init(&h);
    heap_push(&h, source, 0);
    map[source.r][source.c] = 0;
    while (!heap_is_empty(&h)) {
        point p; // point
        int d; // distance
        heap_pop(&h, &p, &d);

        for (int r = p.r - 1; r <= p.r + 1; r++) {
            for (int c = p.c - 1; c <= p.c + 1; c++) {
                if ((r == p.r && c == p.c) ||
                    (r < 1 || r > DUNGEON_HEIGHT - 2 || c < 1 || c > DUNGEON_WIDTH - 2)) {
                    continue;
                }
                int hardness = dungeon->tiles[r][c].hardness;
                if (!enable_tunnels && hardness) { // if tunnels disabled and hardness not 0, skip
                    continue;
                }

                int w;
                if (hardness == 0) {
                    w = 1;
                } else if (hardness == 255) {
                    w = INF;
                } else {
                    w = 1 + (hardness / 85);
                }

                int new_dist = d + w;

                if (new_dist < map[r][c]) {
                    map[r][c] = new_dist;
                    heap_push(&h, (point){r, c}, new_dist);
                }
            }
        }
    }

    return 0;
}
