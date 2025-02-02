#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "gen.h"

int generate_rooms(dungeon *dungeon, int num_rooms);
int place_room(dungeon *dungeon, room *room);
int init_dungeon(dungeon *dungeon);
int generate_hardness(dungeon *dungeon);
void propogate_hardness(dungeon *dungeon, point seed, int hardness);

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

    // fill map with rock and hardness 0
    for (r = 1; r < DUNGEON_HEIGHT-1; r++) {
        for (c = 1; c < DUNGEON_WIDTH-1; c++) {
            dungeon->tiles[r][c].sprite = ' ';
            dungeon->tiles[r][c].hardness = 0;
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
    int i, r, c;
    int num_seeds, hardness;
    point *seeds, seed;
    int *hardnesses;

    num_seeds = rand() % 4 + 5;
    seeds = malloc(sizeof (*seeds) * num_seeds);
    hardnesses = malloc(sizeof (*hardnesses) * num_seeds);

    for (i = 0; i < num_seeds; i++) {
        seed.r = rand() % DUNGEON_HEIGHT;
        seed.c = rand() % DUNGEON_WIDTH;
        hardness = (rand() % 7 + 1) * 20;
        
        dungeon->tiles[seed.r][seed.c].hardness = hardness;

        seeds[i] = seed;
        hardnesses[i] = hardness;
    }

    for (i = 0; i < num_seeds; i++) {
        propogate_hardness(dungeon, seeds[i], hardnesses[i]);
    }

    free(seeds);
    free(hardnesses);

    return 0;
}

void propogate_hardness(dungeon *dungeon, point seed, int hardness) {
    int r, c;
    point p;

    for (r = seed.r - 1; r <= seed.r + 1; r++) {
        for (c = seed.c - 1; c <= seed.c + 1; c++) {
            if (r < 1 || r > DUNGEON_HEIGHT - 2 || c < 1 || c > DUNGEON_WIDTH - 2) {
                continue;
            }
            if (dungeon->tiles[r][c].hardness != 0) {
                continue;
            }
            dungeon->tiles[r][c].hardness = hardness;
            p.r = r;
            p.c = c;
            propogate_hardness(dungeon, p, hardness);
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
        }
    }

    return 0;
}

int generate_corridors(dungeon *dungeon, room *rooms, int num_rooms) {
    int i, r, c, err;

    return 0;
}

