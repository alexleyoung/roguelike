#ifndef GEN_H
#define GEN_H

#define DUNGEON_WIDTH 80
#define DUNGEON_HEIGHT 21

#define DEFAULT_HARDNESS -1

#define ROOM_MIN_WIDTH 3
#define ROOM_MIN_HEIGHT 2
#define ROOM_MAX_WIDTH 20
#define ROOM_MAX_HEIGHT 9

typedef struct point {
    int r;
    int c;
} point;

typedef struct tile {
    char sprite;
    int hardness;
} tile;

typedef struct seed {
    point p;
    int hardness;
} seed;

typedef struct room {
    point corner;
    point size;
} room;

typedef struct dungeon {
    tile tiles[DUNGEON_HEIGHT][DUNGEON_WIDTH];
    room *rooms;
} dungeon;

int generate_dungeon(dungeon *dungeon,int num_rooms);

#endif