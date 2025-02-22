#include <stdlib.h>

#include "movement.h"
#include "../characters/pathfinding.h"

int move(dungeon *d, character *c) {
    if (c->traits == PLAYER_TRAIT) {
        // empty current location in pointer map
        d->character_map[c->pos.r][c->pos.c] = NULL;

        // randomly move
        d->player_pos.r += rand() % 3 - 1;
        d->player_pos.c += rand() % 3 - 1;
        c->pos = d->player_pos;

        // update dist maps
        calc_dists(d, d->dists, d->player_pos, 0);
        calc_dists(d, d->tunnel_dists, d->player_pos, 1);

        // move pointer to new pos
        d->character_map[c->pos.r][c->pos.c] = c;
    }

    return 0;
}
