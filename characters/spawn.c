#include <stdlib.h>

#include "spawn.h"

int create_monster(character *c, int id) {
    int r;

    c->id = id;
    c->traits = rand() % 15;
    c->speed = (rand() % 21) + 5;

    return 0;
}
