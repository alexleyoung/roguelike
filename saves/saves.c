#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <machine/endian.h>

#include "saves.h"

/*
Returns home dir appended with /.rlg327
*/
char *get_save_path(char *name) {
    char *home = getenv("HOME");

    // append .rlg327 to home directory
    strcat(home, "/.rlg327");
    strcat(home, name);

    return home;
}

/*
Save current dungeon to big endian binary file

Returns 0 on success, non-zero on failure.
*/
int save_dungeon(dungeon *dungeon) {
    // create path to file
    char *dir = get_save_path("/dungeon");
    FILE *f = fopen("dungeon", "wb");
    if (f == NULL) {
        printf("Error: Could not open file %s\n", dir);
        return 1;
    }

    // write semantic marker (12B)
    char *marker = "RLG327-S2025";
    fwrite(marker, sizeof(char), strlen(marker), f);

    // write version marker (4B )
    int version = 0;
    fwrite(&version, sizeof(version), 1, f);

    // write size of file (4B)
    int size = 0;
    fwrite(&size, sizeof(size), 1, f);

    // write location of player (2B)
    uint8_t player_x = 0;
    uint8_t player_y = 0;
    fwrite(&player_x, sizeof(player_x), 1, f);
    fwrite(&player_y, sizeof(player_y), 1, f);

    // write tiles array
    for (int r = 0; r < DUNGEON_HEIGHT; r++) {
        for (int c = 0; c < DUNGEON_WIDTH; c++) {
            // write tile
            fwrite(&dungeon->tiles[r][c].hardness, sizeof(dungeon->tiles[r][c].hardness), 1, f);
        }
    }

    // write num_rooms (2B)
    uint16_t r = dungeon->num_rooms;
    fwrite(&r, sizeof(r), 1, f);

    // write positions of rooms (r * 4B)
    for (int i = 0; i < r; i++) {
        fwrite(&dungeon->rooms[i].corner.c, sizeof(dungeon->rooms[i].corner.c), 1, f);
        fwrite(&dungeon->rooms[i].corner.r, sizeof(dungeon->rooms[i].corner.r), 1, f);
        fwrite(&dungeon->rooms[i].size.c, sizeof(dungeon->rooms[i].size.c), 1, f);
        fwrite(&dungeon->rooms[i].size.r, sizeof(dungeon->rooms[i].size.r), 1, f);
    }

    // count stairs
    uint16_t u = 0;
    uint16_t d = 0;
    for (int i = 0; i < dungeon->num_stairs; i++) {
        if (dungeon->stairs[i].type == 0) { u++; }
        else { d++; }
    }

    // write # up stairs (2B)
    fwrite(&u, sizeof(u), 1, f);

    // write positions of up stairs (u * 2B)
    for (int i = 0; i < u + d; i++) {
        if (dungeon->stairs[i].type == 0) {
            fwrite(&dungeon->stairs[i].p.c, sizeof(dungeon->stairs[i].p.c), 1, f);
            fwrite(&dungeon->stairs[i].p.r, sizeof(dungeon->stairs[i].p.r), 1, f);
        }
    }

    // write # down stairs (2B)
    fwrite(&d, sizeof(d), 1, f);

    // write positions of down stairs (d * 2B)
    for (int i = 0; i < u + d; i++) {
        if (dungeon->stairs[i].type == 1) {
            fwrite(&dungeon->stairs[i].p.c, sizeof(dungeon->stairs[i].p.c), 1, f);
            fwrite(&dungeon->stairs[i].p.r, sizeof(dungeon->stairs[i].p.r), 1, f);
        }
    }

    fclose(f);
    return 0;
}

int load_dungeon(dungeon *dungeon) {
    return 0;
}