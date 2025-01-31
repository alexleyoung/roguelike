#include <stdlib.h>
#include <stdio.h>

#include "gen/gen.h"

void print_dungeon(dungeon *dungeon) {
    int i, j;
    for (i = 0; i < DUNGEON_WIDTH; i++) {
        for (j = 0; j < DUNGEON_HEIGHT; j++) {
            printf("%c", dungeon->tiles[i][j].sprite);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    // create a dungeon
    dungeon *dungeon = malloc(sizeof (*dungeon));
    generate_dungeon(dungeon, 6);
    print_dungeon(dungeon);
    return 0;
}
