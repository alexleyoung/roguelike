#include <stdlib.h>

#include "pathfinding.h"
#include "../dsa/algs.h"
#include "../dsa/queue.h"
#include "../dsa/corridor_heap.h"

#define INF 214483647

int calc_dists(dungeon *dungeon, int map[DUNGEON_HEIGHT][DUNGEON_WIDTH], point source) {
    for (int r = 0; r < DUNGEON_HEIGHT; r++) {
        for (int c = 0; c < DUNGEON_WIDTH; c++) {
            map[r][c] = INF;
        }
    }
    
    int visited[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {0};
    queue *q;
    queue_init(q);
    queue_enqueue(q, &source);
    map[source.r][source.c] = 0;

    while (!queue_is_empty(q)) {
        void *data;
        if (queue_dequeue(q, &data) == 0) {
            point *p = (point *)data;
            for (int r = p->r - 1; r <= p->r + 1; r++) {
                for (int c = p->c - 1; c <= p->c + 1; c++) {
                    // bounds
                    if (r < 1 || r > DUNGEON_HEIGHT - 2 || c < 1 || c > DUNGEON_WIDTH - 2) {
                        continue;
                    }
                    // valid tile
                    if ((r == p->r && c == p->c) || visited[r][c] || dungeon->tiles[r][c].sprite == ' ') {
                        continue;
                    }
                    
                    map[r][c] = map[p->r][p->c] + 1;
                    queue_enqueue(q, &(point){r, c});
                }
            }
            free(p);
        }
    }

    return 0;
}
