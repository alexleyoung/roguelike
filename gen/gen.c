#include <stdlib.h>
#include <time.h>
#include "gen.h"

int generate_dungeon(dungeon *dungeon, int num_rooms) {
    srand(time(NULL)); // seed RNG

    dungeon->rooms = malloc(sizeof (*dungeon->rooms) * num_rooms);
    generate_rooms(dungeon, num_rooms);

    return 0;
}

int generate_rooms(dungeon *dungeon, int num_rooms) {
    srand(time(NULL)); // seed RNG

    return 0;
}