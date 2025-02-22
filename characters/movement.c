#include <stdlib.h>

#include "movement.h"
#include "../characters/pathfinding.h"

int move(dungeon *d, character *c) {
    if (c->traits == PLAYER_TRAIT) {
        d->player_pos.r += rand() % 3 - 1;
        d->player_pos.c += rand() % 3 - 1;
        d->tunnel_dists = calc_dists(d, d->tunnel_dists, d->player_pos, 1);
    }

    return 0;
}
