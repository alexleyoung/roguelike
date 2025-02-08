#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "saves.h"
#include"../dsa/endian.h"

/*
Returns home dir appended with /.rlg327
*/
char *get_save_path(const char *name) {
    char *home = getenv("HOME");
    if (!home) {
        printf("Error: Could not get home directory\n");
        return NULL;
    }

    char *path = malloc(strlen(home) + strlen(name) + 10);  // Extra space for "/.rlg327"
    if (!path) {
        printf("Error: Memory allocation failed\n");
        return NULL;
    }

    // sprintf(path, "%s/.rlg327/%s", home, name);
    return name;
    return path;
}

/*
Save current dungeon to big endian binary file

Returns 0 on success, non-zero on failure.
*/
int save_dungeon(dungeon *dungeon, const char *name) {
    // create path to file in case we want different save files eventually
    char *dir = get_save_path(name);
    FILE *f = fopen(dir, "wb");
    if (f == NULL) {
        printf("Error: Could save file %s\n", dir);
        return 1;
    }

    char *marker = "RLG327-S2025";  // 12 bytes
    for (int i = 0; i < 12; i += 2) {
        uint16_t bytes;
        bytes = (marker[i] << 8) | marker[i + 1];  // Pack two chars
        bytes = htobe16(bytes);
        fwrite(&bytes, sizeof(uint16_t), 1, f);
    }

    // write version marker (4B)
    uint32_t version = 0;
    version = htobe32(version);
    fwrite(&version, 4, 1, f);

    // write size of file (4B)
    uint32_t size = 0;
    // calcualte size of file
    size += 12; // semantic marker 12 char
    size += 4; // version 1 uint32
    size += 4; // size of file 1 uint32
    size += 2; // player location, 2 uint8
    size += DUNGEON_HEIGHT * DUNGEON_WIDTH; // tile each uint8 hardness
    size += 2; // num rooms 1 uint16
    for (int i = 0; i < dungeon->num_rooms; i++) {
        size += 2; // room corner
        size += 2; // room size
    }
    size += 2; // num up stairs 1 uint16
    size += 2; // num down stairs 1 uint16
    for (int i = 0; i < dungeon->num_stairs; i++) {
        size += 2; // stairs 2 uint8
    }
    size = htobe32(size);
    fwrite(&size, sizeof(size), 1, f);

    // write location of player (2B)
    uint8_t player_x = dungeon->rooms[0].corner.c;
    uint8_t player_y = dungeon->rooms[0].corner.r;
    fwrite(&player_x, sizeof(player_x), 1, f);
    fwrite(&player_y, sizeof(player_y), 1, f);

    // write tiles array
    for (int r = 0; r < DUNGEON_HEIGHT; r++) {
        for (int c = 0; c < DUNGEON_WIDTH; c++) {
            // write tile
            uint8_t hardness = dungeon->tiles[r][c].hardness;
            fwrite(&hardness, sizeof(hardness), 1, f);
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

int load_dungeon(dungeon *dungeon, const char *name) {
    char marker[13];
    uint32_t version, file_size;
    uint8_t player_x, player_y, hardness;
    uint16_t r, u, d;

    // create path to file in case we want different save files eventually
    char *dir = get_save_path(name);
    FILE *f = fopen(dir, "rb");
    if (f == NULL) {
        printf("Error: Could not open file to load: %s\n", dir);
        return 1;
    }

    // get size of file
    fseek(f, 0, SEEK_END);
    file_size = ftell(f);
    fseek(f, 0, SEEK_SET);

    // read semantic marker
    fread(marker, sizeof(char), 12, f);
    marker[13] = '\0';
    printf("marker: %s\n", marker);
    // check if file is valid
    if (strcmp(marker, "RLG327-S2025") != 0) {
        printf("Error: Invalid marker %s\n", marker);
        return 1;
    }

    // read version marker
    fread(&version, sizeof(version), 1, f);
    // if (version != 0) {
    //     printf("Error: Invalid version %s\n", dir);
    //     return 1;
    // }
    printf("version: %d\n", version);

    // read size of file
    fread(&file_size, sizeof(file_size), 1, f);

    // read location of player
    fread(&player_x, sizeof(player_x), 1, f);
    fread(&player_y, sizeof(player_y), 1, f);

    // read tiles array
    for (int r = 0; r < DUNGEON_HEIGHT; r++) {
        for (int c = 0; c < DUNGEON_WIDTH; c++) {
            fread(&hardness, sizeof(hardness), 1, f);
            dungeon->tiles[r][c].hardness = hardness;
            // check if border
            if (c == 0 || c == DUNGEON_WIDTH-1) {
                dungeon->tiles[r][c].sprite = '|';
            }
            if (r == 0 || r == DUNGEON_HEIGHT-1) {
                dungeon->tiles[r][c].sprite = '-';
            }

            if (hardness == 0) {
                dungeon->tiles[r][c].sprite = '#';
            } else {
                dungeon->tiles[r][c].sprite = ' ';
            }
        }
    }

    // read num_rooms   
    fread(&r, sizeof(r), 1, f);
    dungeon->num_rooms = r;
    dungeon->rooms = malloc(sizeof (*dungeon->rooms) * r);

    for (int i = 0; i < r; i++) {
        fread(&dungeon->rooms[i].corner.c, sizeof(dungeon->rooms[i].corner.c), 1, f);
        fread(&dungeon->rooms[i].corner.r, sizeof(dungeon->rooms[i].corner.r), 1, f);
        fread(&dungeon->rooms[i].size.c, sizeof(dungeon->rooms[i].size.c), 1, f);
        fread(&dungeon->rooms[i].size.r, sizeof(dungeon->rooms[i].size.r), 1, f);
    }

    // read up stair count
    fread(&u, sizeof(u), 1, f);
    dungeon->stairs = malloc(sizeof (*dungeon->stairs) * u);

    for (int i = 0; i < u; i++) {
        fread(&dungeon->stairs[i].p.c, sizeof(dungeon->stairs[i].p.c), 1, f);
        fread(&dungeon->stairs[i].p.r, sizeof(dungeon->stairs[i].p.r), 1, f);
        dungeon->stairs[i].type = 0;
    }

    // read down stair count
    fread(&d, sizeof(d), 1, f);
    dungeon->stairs = realloc(dungeon->stairs, sizeof (*dungeon->stairs) * u + sizeof (*dungeon->stairs) * d);

    for (int i = 0; i < d; i++) {
        fread(&dungeon->stairs[u+i].p.c, sizeof(dungeon->stairs[u+i].p.c), 1, f);
        fread(&dungeon->stairs[u+i].p.r, sizeof(dungeon->stairs[u+i].p.r), 1, f);
        dungeon->stairs[u+i].type = 1;
    }

    fclose(f);
    return 0;
}