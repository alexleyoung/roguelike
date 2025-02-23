#include <stdlib.h>
#include <stdio.h>

#include "movement.h"
#include "../characters/pathfinding.h"

int move(dungeon *d, character *c) {
    if (c->traits == PLAYER_TRAIT) {
        // RANDOM PLAYER MOVEMENT
        // randomly move
        int new_r = c->pos.r + rand() % 3 - 1;
        int new_c = c->pos.c + rand() % 3 - 1;
        if (new_r < 1 || new_r > DUNGEON_HEIGHT - 2) {
            new_r = c->pos.r;
        }
        if (new_c < 1 || new_c > DUNGEON_WIDTH - 2) {
            new_c = c->pos.c;
        }

        if (new_r == c->pos.r && new_c == c->pos.c) {
            // skip if no move
            return 0;
        }

        // empty current location in pointer map
        d->character_map[c->pos.r][c->pos.c] = NULL;

        // update player's pos
        c->pos = (point){new_r, new_c};
        d->player_pos = (point){new_r, new_c};

        // move pointer to new pos
        d->character_map[c->pos.r][c->pos.c] = c;

        // update dist maps
        calc_dists(d, d->dists, d->player_pos, 0);
        calc_dists(d, d->tunnel_dists, d->player_pos, 1);

        // check if moved into rock
        if (d->tiles[c->pos.r][c->pos.c].sprite == ' ') {
            d->tiles[c->pos.r][c->pos.c].sprite = '#';
        }

    }

    return 0;
}
