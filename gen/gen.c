#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "gen.h"
#include "../dsa/queue.h"

int generate_rooms(dungeon *dungeon, int num_rooms);
int place_room(dungeon *dungeon, room *room);
int init_dungeon(dungeon *dungeon);
int generate_hardness(dungeon *dungeon);
void propogate_hardness(dungeon *dungeon, queue *q, seed *s);

int generate_dungeon(dungeon *dungeon, int num_rooms) {
    srand(time(NULL)); // seed RNG

    init_dungeon(dungeon);

    int i, err;

    dungeon->rooms = malloc(sizeof (*dungeon->rooms) * num_rooms);
    if ((err = generate_rooms(dungeon, num_rooms))) {
        return err;
    }; 

    return 0;
}

int init_dungeon(dungeon *dungeon) {
    int r, c;

    // fill map with rock and default hardness
    for (r = 1; r < DUNGEON_HEIGHT-1; r++) {
        for (c = 1; c < DUNGEON_WIDTH-1; c++) {
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

    generate_hardness(dungeon);

    return 0;
}

int generate_hardness(dungeon *dungeon) {
    int i, r, c, x, y;
    queue q;
    queue_init(&q);
    int num_seeds, hardness;
    seed *s;

    num_seeds = rand() % 4 + 5;

    for (i = 0; i < num_seeds; i++) {
        s = malloc(sizeof (*s));
        s->p.r = rand() % DUNGEON_HEIGHT;
        s->p.c = rand() % DUNGEON_WIDTH;
        s->hardness = (rand() % 7 + 1) * 20;

        dungeon->tiles[s->p.r][s->p.c].hardness = s->hardness;

        queue_enqueue(&q, s);
    }

    while (!queue_is_empty(&q)) {
        void *data;
        if (queue_dequeue(&q, &data) == 0) {
            s = (seed *)data;
        }
        dungeon->tiles[s->p.r][s->p.c].hardness = s->hardness;
        propogate_hardness(dungeon, &q, s);
        free(s);
    }

    queue_destroy(&q);
    return 0;
}

void propagate_hardness(dungeon *dungeon, queue *q, seed *s) {
    int r, c;
    seed *child;

    for (r = s->p.r - 1; r <= s->p.r + 1; r++) {
        for (c = s->p.c - 1; c <= s->p.c + 1; c++) {
            if (r < 1 || r > DUNGEON_HEIGHT - 2 || c < 1 || c > DUNGEON_WIDTH - 2) {
                continue;
            }
            if (dungeon->tiles[r][c].hardness != DEFAULT_HARDNESS) {
                continue;
            }
            // add to queue
            child = malloc(sizeof (*child));
            child->p.r = r;
            child->p.c = c;
            child->hardness = s->hardness;

            queue_enqueue(q, child);
        }
    }
}

int generate_rooms(dungeon *dungeon, int num_rooms) {
    srand(time(NULL)); // seed RNG

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
            } else {
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
    int i, r, c;
    int err;

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

