#include <stdlib.h>
#include <stdio.h>

#include "movement.h"
#include "../characters/pathfinding.h"

#define ATTRIBUTE_INTELLIGENT 0x1
#define ATTRIBUTE_TELEPATHIC 0x2
#define ATTRIBUTE_TUNNELING 0x4
#define ATTRIBUTE_ERRATIC 0x8

#define min(x, y) (x < y ? x : y)
#define C_IS(c, attr) (c->traits & ATTRIBUTE_ ## attr)
#define abs(x) (x < 0 ? x * -1 : x)

int move_random(dungeon *d, character *c);
int move_to(dungoen *d, character *c, point p);
int check_los(dungeon *d, character *c);
int check_horizontal(dungeon *d, character *c);
int check_vertical(dungeon *d, character *c);

int move(dungeon *d, character *c) {
    if (c->traits == PLAYER_TRAIT) {
        return move_random(d, c);
    }

    check_los(d, c);

    // if intelligent
    int i, j;
    int shortest = 9999;
    point p;
    if (C_IS(c, INTELLIGENT)) {
        // find neighboring tile closest to player
        for (i = c->pos.r - 1; i <= c->pos.r + 1; i++) {
            for (j = c->pos.c - 1; j <= c->pos.c + 1; j++) {
                if (c->dist_to_player[i][j] < shortest) {
                    p = (point){i, j};
                    shortest = c->dist_to_player[i][j];
                }
            }
        }
        move_to(d, c, point);
    }

    return 0;
}

int move_random(dungeon *d, character *c) {
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

    return 0;
}

// bresenham's line drawing alg
//
// returns 0 if no LOS, 1 otherwise
int check_los(dungeon *d, character *c) {
    point source = c->pos;
    point dest = d->player_pos;

    if (abs(dest.c-source.c) > abs(dest.r - source.r)) {
        return check_horizontal(d, c);
    } else {
        return check_vertical(d, c);
    }
}

int check_horizontal(dungeon *d, character *c) {
    point source = c->pos;
    point dest = d->player_pos;

    if (source.c > dest.c) {
        source = dest;
        dest = c->pos;
    }

    int dx = dest.c - source.c;
    int dy = dest.r - source.r;

    int dir = dy < 0 ? -1 : 1;
    dy *= dir;

    if (dx) {
        int y = source.r;
        int p = 2 * dy - dx;
        for (int i = 0; i < dx + 1; i++) {
            if (d->tiles[source.r][source.c + i].hardness) {
                // solid block in LOS, return false
                return 0;
            }
            if (p >= 0) {
                y += dir;
                p -= 2 * dx;
            }
            p += 2 * dy;
        }
    }

    return 1;
}

int check_vertical(dungeon *d, character *c) {
    point source = c->pos;
    point dest = d->player_pos;

    if (source.r > dest.r) {
        source = dest;
        dest = c->pos;
    }

    int dx = dest.c - source.c;
    int dy = dest.r - source.r;

    int dir = dx < 0 ? -1 : 1;
    dx *= dir;

    if (dy) {
        int x = source.c;
        int p = 2 * dx - dy;
        for (int i = 0; i < dy + 1; i++) {
            if (d->tiles[source.r + i][source.c].hardness) {
                // solid block in LOS, return false
                return 0;
            }
            if (p >= 0) {
                x += dir;
                p -= 2 * dy;
            }
            p += 2 * dx;
        }
    }

    return 1;
}
