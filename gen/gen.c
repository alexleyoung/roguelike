#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "gen.h"
#include "../dsa/queue.h"
#include "../dsa/corridor_heap.h"

#define INF 2147483647

int generate_rooms(dungeon *dungeon, int num_rooms);
int place_room(dungeon *dungeon, room *room);
int init_dungeon(dungeon *dungeon);
int generate_hardness(dungeon *dungeon);
void propagate_hardness(dungeon *dungeon, int propagated[DUNGEON_HEIGHT][DUNGEON_WIDTH], queue *q, seed *s);
void smooth_hardness(dungeon *dungeon);
int generate_corridors(dungeon *dungeon, room *rooms, int num_rooms);
int place_stairs(dungeon *dungeon);

/*
Generate a dungeon with num_rooms number of rooms and at least 2 stairs.

Randomly places rectangular rooms, then creates a smooth hardness gradient
for corridor placement.

Returns 0 on success, non-zero on failure.
*/
int generate_dungeon(dungeon *dungeon, int num_rooms) {
    srand(time(NULL)); // seed RNG

    init_dungeon(dungeon);

    int i, err;

    dungeon->rooms = malloc(sizeof (*dungeon->rooms) * num_rooms);
    if ((err = generate_rooms(dungeon, num_rooms))) {
        return err;
    }; 

    generate_hardness(dungeon);

    generate_corridors(dungeon, dungeon->rooms, num_rooms);



    place_stairs(dungeon);

    return 0;
}

int init_dungeon(dungeon *dungeon) {
    int r, c;

    // fill map with rock and default hardness
    for (r = 0; r < DUNGEON_HEIGHT; r++) {
        for (c = 0; c < DUNGEON_WIDTH; c++) {
            dungeon->tiles[r][c].sprite = ' ';
            dungeon->tiles[r][c].hardness = DEFAULT_HARDNESS;
        }
    }

    // draw border
    for (r = 0; r < DUNGEON_HEIGHT; r++) {
        dungeon->tiles[r][0].sprite = '|';
        dungeon->tiles[r][DUNGEON_WIDTH-1].sprite = '|';
    }
    for (c = 0; c < DUNGEON_WIDTH; c++) {
        dungeon->tiles[0][c].sprite = '-';
        dungeon->tiles[DUNGEON_HEIGHT-1][c].sprite = '-';
    }

    return 0;
}

/*
Generates a smooth hardness map over the dungeon

Returns 0 on success, non-zero otherwise
*/
int generate_hardness(dungeon *dungeon) {
    int i; 
    queue q;
    queue_init(&q);
    int num_seeds;
    seed *s;
    int propagated[DUNGEON_HEIGHT][DUNGEON_WIDTH];

    num_seeds = rand() % 10 + 20;

    // place hardness seeds, add to queue for bfs
    for (i = 0; i < num_seeds; i++) {
        s = malloc(sizeof (*s));

        // pick a random empty tile
        do {
            s->p.r = rand() % DUNGEON_HEIGHT;
            s->p.c = rand() % DUNGEON_WIDTH;
        } while (dungeon->tiles[s->p.r][s->p.c].sprite != ' ');

        s->hardness = (rand() % 40 + 1);

        queue_enqueue(&q, s);
    }

    // propagate hardness outward
    while (!queue_is_empty(&q)) {
        void *data;
        if (queue_dequeue(&q, &data) == 0) {
            s = (seed *)data;
        }
        dungeon->tiles[s->p.r][s->p.c].hardness = s->hardness;
        propagate_hardness(dungeon, propagated, &q, s);
        free(s);
    }

    queue_destroy(&q);

    smooth_hardness(dungeon);

    return 0;
}

/*
BFS helper to propagate hardness from seeds
*/
void propagate_hardness(dungeon *dungeon, int propagated[DUNGEON_HEIGHT][DUNGEON_WIDTH], queue *q, seed *s) {
    int i, r, c;
    seed *child;

    // add neighbors to queue
    int dirs = 4;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    for (i = 0; i < dirs; i++) {
        r = s->p.r + dr[i];
        c = s->p.c + dc[i];
        // check bounds
        if (r < 0 || r > DUNGEON_HEIGHT || c < 0 || c > DUNGEON_WIDTH) { continue; }
        // check if neighbor's hardness is unmodified
        if (dungeon->tiles[r][c].hardness != DEFAULT_HARDNESS) { continue; }
        // check if neighbor is already visited
        if (propagated[r][c] == 1) { continue; }

        // update hardness
        dungeon->tiles[s->p.r][s->p.c].hardness = s->hardness;

        // add to queue
        child = malloc(sizeof (*child));
        child->p.r = r;
        child->p.c = c;
        child->hardness = s->hardness;
        queue_enqueue(q, child);

        // mark as visited
        propagated[r][c] = 1;
    }
}

/*
Apply gaussian blur to hardness map for smoothing
*/
void smooth_hardness(dungeon *dungeon) {
    int r, c, i, j;

    // kernel for guassian blur
    // int kernel[5][5] = {
    //     {1, 2, 3, 2, 1},
    //     {2, 4, 6, 4, 2},
    //     {3, 6, 9, 6, 3},
    //     {2, 4, 6, 4, 2},
    //     {1, 2, 3, 2, 1}
    // };
    // int kernel_size = 5;
    // int kernal_offset = 2;
    // int kernel_sum = 273;
    int kernel[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
    };
    int kernel_size = 3;
    int kernal_offset = 1;
    int kernel_sum = 16;

    // hardness map copy
    int blurred_hardness[DUNGEON_HEIGHT][DUNGEON_WIDTH];

    for (r = 1; r < DUNGEON_HEIGHT - 1; r++) {
        for (c = 1; c < DUNGEON_WIDTH - 1; c++) {
            if (dungeon->tiles[r][c].hardness == 0) {
                continue;
            }
            int new_hardness = 0;
            for (i = 0; i < kernel_size; i++) {
                for (int j = 0; j < kernel_size; j++) {
                    int nr = r + i - kernal_offset; // neighbor row
                    int nc = c + j - kernal_offset; // neighbor column
                    new_hardness += dungeon->tiles[nr][nc].hardness * kernel[i][j];
                }
            }
            // average with kernel sum
            if (new_hardness / kernel_sum > 200) {
                blurred_hardness[r][c] = 200;
            } else {
                blurred_hardness[r][c] = new_hardness / kernel_sum;
            }
        }
    }

    // transfer calculated hardness back to the dungeon tiles
    for (int r = 1; r < DUNGEON_HEIGHT - 1; r++) {
        for (int c = 1; c < DUNGEON_WIDTH - 1; c++) {
            dungeon->tiles[r][c].hardness = blurred_hardness[r][c];
        }
    }
}

/*
Randomly generates num_rooms rectangular rooms in the dungeon to be placed

Returns 0 on success, non-zero on failure.
*/
int generate_rooms(dungeon *dungeon, int num_rooms) {
    int i, r, c, err;

    for (i = 0; i < num_rooms; i++) {
        room *room = &dungeon->rooms[i];
        point corner, size; // for room validation

        int invalid = 0;
        int tries = 1;

        while (!invalid && tries < 2000) {
            // pick room corner and size
            corner.r = (rand() % DUNGEON_HEIGHT-1) + 1; // avoid immutable boundary
            corner.c = (rand() % DUNGEON_WIDTH-1) + 1;
            size.r = rand() % (ROOM_MAX_HEIGHT-ROOM_MIN_HEIGHT) + ROOM_MIN_HEIGHT;
            size.c = rand() % (ROOM_MAX_WIDTH-ROOM_MIN_WIDTH) + ROOM_MIN_WIDTH;

            // check if room is valid
            for (r = 0; r < size.r; r++) {
                for (c = 0; c < size.c; c++) {
                    if (dungeon->tiles[corner.r+r][corner.c+c].sprite != ' ') {
                        invalid = 1;
                        break;
                    }
                }
                if (invalid) {
                    break;
                }
            }

            if (invalid) {
                tries++;
                invalid = 0;
                continue;
            } else { // room must be valid or tries exceeded, stop trying
                break;
            }
        }

        // tried and failed to place all the rooms
        if (tries >= 2000) {
            printf("failed to place room. too many tries\n");
            return -1;
        }

        room->corner = corner;
        room->size = size;

        if ((err = place_room(dungeon, room))) {
            printf("failed to place room\n");
            return err;
        }
    }

    return 0;
}

/*
Places a rectangular room in the dungeon
*/
int place_room(dungeon *dungeon, room *room) {
    int r, c;
    int err; // shouldn't ever error, here just in case

    // place room
    for (r = 0; r < room->size.r; r++) {
        for (c = 0; c < room->size.c; c++) {
            dungeon->tiles[room->corner.r+r][room->corner.c+c].sprite = '.';
            dungeon->tiles[room->corner.r+r][room->corner.c+c].hardness = 0;
        }
    }

    return 0;
}

/*
Generates corridors between rooms

Function uses dijkstra to find the shortest path between rooms, with slight randomization
to add randomness to hallways.

Returns 0 on success, non-zero on failure.
*/
int generate_corridors(dungeon *dungeon, room *rooms, int num_rooms) {
    int i, r, c,err;

    // find centers of rooms
    point *centers = malloc(sizeof (*centers) * num_rooms);
    for (i = 0; i < num_rooms; i++) {
        centers[i] = (point){rooms[i].corner.r + rooms[i].size.r / 2,
                             rooms[i].corner.c + rooms[i].size.c / 2};
    }

    int distances[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    point predecessors[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    point source, target;

    // dijkstra to connect each room
    for (i = 0; i < num_rooms; i++) {
        // init dijkstra
        for (r = 0; r < DUNGEON_HEIGHT; r++) {
            for (c = 0; c < DUNGEON_WIDTH; c++) {
                distances[r][c] = INF;
                predecessors[r][c] = (point){-1, -1};
            }
        }

        source = centers[i];
        if (i == num_rooms - 1) {
            target = centers[0];
        } else {
            target = centers[i+1];
        }
        heap h;
        heap_init(&h);
        heap_push(&h, source, 0);
        distances[source.r][source.c] = 0;

        while (!heap_is_empty(&h)) {
            point u;
            int w;
            heap_pop(&h, &u, &w);
            
            // process neighbors (up, down, left, right)
            int dr[] = {-1, 1, 0, 0};
            int dc[] = {0, 0, -1, 1};
            for (int dir = 0; dir < 4; dir++) {
                int nr = u.r + dr[dir];
                int nc = u.c + dc[dir];

                if (nr < 1 || nr >= DUNGEON_HEIGHT - 1 || nc < 1 || nc >= DUNGEON_WIDTH - 1) {
                    continue;
                }

                int dist = w + dungeon->tiles[nr][nc].hardness + (rand() % 5); // add randomness
                if (dist < distances[nr][nc]) {
                    distances[nr][nc] = dist;
                    predecessors[nr][nc] = u;
                    heap_push(&h, (point){nr, nc}, dist);
                }
            }
        }

        // draw corridor
        point current = target;
        int hardened = (rand() % 30) + 3; // add randomness to hallway hardness for 
        while (current.r != source.r || current.c != source.c) {
            if (dungeon->tiles[current.r][current.c].sprite == ' ') {
                dungeon->tiles[current.r][current.c].sprite = '#';
                dungeon->tiles[current.r][current.c].hardness = hardened + rand() % 6 - 3;
            }
            current = predecessors[current.r][current.c];
        }

        heap_destroy(&h);
    }
    
    free(centers);
    return 0;
}

/*
Places at least 2 stairs in any room or corridor tile

Returns 0 on success, non-zero otherwise.
*/
int place_stairs(dungeon *dungeon) {
    int i, r, c, n;

    char stairs[2] = {'<', '>'};

    // place at least one of each stair
    for (i = 0; i < 2; i++) {
        do {
            r = rand() % DUNGEON_HEIGHT;
            c = rand() % DUNGEON_WIDTH;
        } while (dungeon->tiles[r][c].sprite != '.' && dungeon->tiles[r][c].sprite != '#');
        dungeon->tiles[r][c].sprite = stairs[i];
    }

    n = rand() % 3;
    for (i = 0; i < n; i++) {
        // pick random room or corridor tile
        do {
            r = rand() % DUNGEON_HEIGHT;
            c = rand() % DUNGEON_WIDTH;
        } while (dungeon->tiles[r][c].sprite != '.' && dungeon->tiles[r][c].sprite != '#');

        // 50/50 up or down stair
        if (rand() % 2 == 0) {
            dungeon->tiles[r][c].sprite = '<';
        } else {
            dungeon->tiles[r][c].sprite = '>';
        }
    }
}