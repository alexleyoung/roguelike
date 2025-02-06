#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "saves.h"

#define SAVE_FILE ".rlg327/dungeon"

int save_dungeon(dungeon *dungeon) {
    char *home = getenv("HOME");

    char *path = malloc(sizeof (home) + sizeof (SAVE_FILE) + 1); 
    strcat(path, path);
    strcat(path, SAVE_FILE);

    printf("saving to %s\n", path);

    free (path);

    return 0;
}

int load_dungeon(dungeon *dungeon) {
    return 0;
}