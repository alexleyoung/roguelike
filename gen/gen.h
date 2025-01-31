#ifndef GEN_H
#define GEN_H

#define DUNGEON_WIDTH 80
#define DUNGEON_HEIGHT 21

typedef struct tile {
    char sprite;
    int hardness;
} tile;

typedef struct room {
    int tl;
    int br;
} room;

typedef struct dungeon {
    tile tiles[DUNGEON_WIDTH][DUNGEON_HEIGHT];
    room *rooms;
} dungeon;

int generate_dungeon(dungeon *dungeon,int num_rooms);
int generate_rooms(dungeon *dungeon, int num_rooms);

#endif