#include <stdlib.h>
#include <stdio.h>

#include "movement.h"
#include "../characters/pathfinding.h"

int move(dungeon *d, character *c) {
    if (c->traits == PLAYER_TRAIT) {
        // RANDOM PLAYER MOVEMENT

        // randomly move
        d->player_pos.r += rand() % 3 - 1;
        d->player_pos.c += rand() % 3 - 1;
        if (d->player_pos.r < 1 || d->player_pos.r > DUNGEON_HEIGHT - 2) {
            d->player_pos.r = c->pos.r;
        }
        if (d->player_pos.c < 1 || d->player_pos.c > DUNGEON_WIDTH - 2) {
            d->player_pos.c = c->pos.c;
        }

        if (d->player_pos.r != c->pos.r || d->player_pos.c != c->pos.c) {
            // empty current location in pointer map
            d->character_map[c->pos.r][c->pos.c] = NULL;

            // update dist maps
            calc_dists(d, d->dists, d->player_pos, 0);
            calc_dists(d, d->tunnel_dists, d->player_pos, 1);

            // update character's pos
            c->pos = d->player_pos;
        } else {
            return 0;
        }


        if (d->tiles[c->pos.r][c->pos.c].sprite == ' ') {
            d->tiles[c->pos.r][c->pos.c].sprite = '#';
        }


        // move pointer to new pos
        d->character_map[c->pos.r][c->pos.c] = c;
    }

    return 0;
}
