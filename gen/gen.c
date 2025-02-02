#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "gen.h"
#include "../dsa/queue.h"

int generate_rooms(dungeon *dungeon, int num_rooms);
int place_room(dungeon *dungeon, room *room);
int init_dungeon(dungeon *dungeon);
int generate_hardness(dungeon *dungeon);
void propagate_hardness(dungeon *dungeon, int hitmap[DUNGEON_HEIGHT][DUNGEON_WIDTH], queue *q, seed *s);
void smooth_hardness(dungeon *dungeon);

/*
Generate a dungeon with num_rooms number of rooms.

Randomly places rectangular rooms, then creates a smooth hardness gradient
for corridor placement.abort

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

int generate_hardness(dungeon *dungeon) {
    int i; 
    queue q;
    queue_init(&q);
    int num_seeds;
    seed *s;
    int hitmap[DUNGEON_HEIGHT][DUNGEON_WIDTH];

    num_seeds = rand() % 4 + 5;

    // place hardness seeds, add to queue for bfs
    for (i = 0; i < num_seeds; i++) {
        s = malloc(sizeof (*s));

        // pick a random empty tile
        do {
            s->p.r = rand() % DUNGEON_HEIGHT;
            s->p.c = rand() % DUNGEON_WIDTH;
        } while (dungeon->tiles[s->p.r][s->p.c].sprite != ' ');

        s->hardness = (rand() % 7 + 1) * 20;

        queue_enqueue(&q, s);
    }

    // propagate hardness outward
    while (!queue_is_empty(&q)) {
        void *data;
        if (queue_dequeue(&q, &data) == 0) {
            s = (seed *)data;
        }
        dungeon->tiles[s->p.r][s->p.c].hardness = s->hardness;
        propagate_hardness(dungeon, hitmap, &q, s);
        free(s);
    }

    queue_destroy(&q);
    
    smooth_hardness(dungeon);

    return 0;
}

/*
BFS helper to propagate hardness from seeds
*/
void propagate_hardness(dungeon *dungeon, int hitmap[DUNGEON_HEIGHT][DUNGEON_WIDTH], queue *q, seed *s) {
    int r, c;
    seed *child;

    // add neighbors to queue
    for (r = s->p.r - 1; r <= s->p.r + 1; r++) {
        for (c = s->p.c - 1; c <= s->p.c + 1; c++) {
            // check bounds
            if (r < 0 || r > DUNGEON_HEIGHT || c < 0 || c > DUNGEON_WIDTH) { continue; }
            // check if neighbor's hardness is unmodified
            if (dungeon->tiles[r][c].hardness != DEFAULT_HARDNESS) { continue; }
            // check if neighbor is already visited
            if (hitmap[r][c] == 1) { continue; }

            // update hardness
            dungeon->tiles[s->p.r][s->p.c].hardness = s->hardness;

            // add to queue
            child = malloc(sizeof (*child));
            child->p.r = r;
            child->p.c = c;
            child->hardness = s->hardness;
            queue_enqueue(q, child);

            // mark as visited
            hitmap[r][c] = 1;
        }
    }
}

void smooth_hardness(dungeon *dungeon) {
    // kernel for guassian blur
    int kernel[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
    };
    int kernel_size = 3;
    int kernel_sum = 16;

    // hardness map copy
    int blurred_hardness[DUNGEON_HEIGHT][DUNGEON_WIDTH];

    for (int r = 1; r < DUNGEON_HEIGHT - 1; r++) {
        for (int c = 1; c < DUNGEON_WIDTH - 1; c++) {
            int new_hardness = 0;
            for (int kr = 0; kr < kernel_size; kr++) {
                for (int kc = 0; kc < kernel_size; kc++) {
                    int nr = r + kr - 1; // Neighbor row index
                    int nc = c + kc - 1; // Neighbor column index
                    new_hardness += dungeon->tiles[nr][nc].hardness * kernel[kr][kc];
                }
            }
            // Averaging with kernel sum
            blurred_hardness[r][c] = new_hardness / kernel_sum;
        }
    }

    // Transfer computed hardness back to the dungeon tiles
    for (int r = 1; r < DUNGEON_HEIGHT - 1; r++) {
        for (int c = 1; c < DUNGEON_WIDTH - 1; c++) {
            dungeon->tiles[r][c].hardness = blurred_hardness[r][c];
        }
    }
}

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

int generate_corridors(dungeon *dungeon, room *rooms, int num_rooms) {
    int i, r, c, err;

    return 0;
}

