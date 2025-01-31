#include "gen/gen.h"

int main(int argc, char **argv) {
    tile dungeon[DUNGEON_WIDTH][DUNGEON_HEIGHT];
    generate_dungeon(dungeon, 6);
    return 0;
}