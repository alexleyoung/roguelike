#include <stdlib.h>
#include <time.h>

typedef struct {
    char sprite;
    int hardness;
} tile;

typedef struct {
    int tl;
    int br;
} room;

tile* generate_dungeon() {
    srand(time(NULL)); // seed RNG

    room *rooms = generate_rooms(6);
    tile dungeon[80][21];

    return dungeon;
}

room* generate_rooms(int size) {
    srand(time(NULL)); // seed RNG

    room rooms[size];

    return rooms;
}