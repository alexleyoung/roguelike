#include <stdlib.h>
#include <stdio.h>

#include "gen/gen.h"

void print_dungeon(dungeon *dungeon) {
    int r, c;
    for (r = 0; r < DUNGEON_HEIGHT; r++) {
        for (c = 0; c < DUNGEON_WIDTH; c++) {
            if (!dungeon->tiles[r][c].sprite) {
                printf(" ");
                continue;
            }
            printf("%c", dungeon->tiles[r][c].sprite);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    // create a dungeon
    dungeon *dungeon = malloc(sizeof (*dungeon));
    generate_dungeon(dungeon, 1);
    print_dungeon(dungeon);
    return 0;
}
