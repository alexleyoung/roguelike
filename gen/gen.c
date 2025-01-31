#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "gen.h"

int generate_rooms(dungeon *dungeon, int num_rooms);
int place_room(dungeon *dungeon, room *room);

int init_dungeon(dungeon *dungeon) {
    int r, c;

    for (r = 0; r < DUNGEON_HEIGHT; r++) {
        for (c = 0; c < DUNGEON_WIDTH; c++) {
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

    return 0;
}

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

int generate_rooms(dungeon *dungeon, int num_rooms) {
    srand(time(NULL)); // seed RNG

    int i, r, c, err;

    for (i = 0; i < num_rooms; i++) {
        room *room = &dungeon->rooms[i];
        int corner[2], size[2]; // for room validation

        int invalid = 0;
        int tries = 1;

        while (!invalid && tries < 2000) {
            // pick room corner and size
            corner[0]= (rand() % DUNGEON_HEIGHT-1) + 1; // avoid immutable boundary
            corner[1]= (rand() % DUNGEON_WIDTH-1) + 1;
            size[0]= rand() % (ROOM_MAX_HEIGHT-ROOM_MIN_HEIGHT) + ROOM_MIN_HEIGHT;
            size[1]= rand() % (ROOM_MAX_WIDTH-ROOM_MIN_WIDTH) + ROOM_MIN_WIDTH;

            // check if room is valid
            for (r = 0; r < size[0]; r++) {
                for (c = 0; c < size[1]; c++) {
                    if (dungeon->tiles[corner[0]+r][corner[1]+c].sprite != ' ') {
                        printf("invalid room at (%d, %d)\n", corner[0]+r, corner[1]+c);
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
            return -1;
        }

        room->corner[0] = corner[0];
        room->corner[1] = corner[1];
        room->size[0] = size[0];
        room->size[1] = size[1];

        if ((err = place_room(dungeon, room))) {
            return err;
        }
    }

    return 0;
}

int place_room(dungeon *dungeon, room *room) {
    int i, r, c;
    int err;

    printf("placing room at (%d, %d)\n", room->corner[0], room->corner[1]);
    printf("size (%d, %d)\n", room->size[0], room->size[1]);
    // place room
    for (r = 0; r < room->size[0]; r++) {
        for (c = 0; c < room->size[1]; c++) {
            dungeon->tiles[room->corner[0]+r][room->corner[1]+c].sprite = '.';
        }
    }

    return 0;
}