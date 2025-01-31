#ifndef GEN_H
#define GEN_H

#define DUNGEON_WIDTH 80
#define DUNGEON_HEIGHT 21

#define ROOM_MIN_WIDTH 3
#define ROOM_MIN_HEIGHT 2
#define ROOM_MAX_WIDTH 20
#define ROOM_MAX_HEIGHT 9

typedef struct tile {
    char sprite;
    int hardness;
} tile;

typedef struct room {
    int corner[2];
    int size[2];
} room;

typedef struct dungeon {
    tile tiles[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    room *rooms;
} dungeon;

int generate_dungeon(dungeon *dungeon,int num_rooms);

#endif