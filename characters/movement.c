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
int move_to(dungeon *d, character *c, point p);
int check_los(dungeon *d, character *c);
int check_horizontal(dungeon *d, character *c);
int check_vertical(dungeon *d, character *c);

int move(dungeon *d, character *c) {
    // only random player movement for now
    if (c->traits == PLAYER_TRAIT) {
        return move_random(d, c);
    }

    // update character dist_to_player if telepathic or has los
    int los = check_los(d, c);
    if (C_IS(c, TELEPATHIC) || los) {
        if (C_IS(c, TUNNELING)) {
            c->dist_to_player = &d->tunnel_dists;
        } else {
            c->dist_to_player = &d->dists;
        }
    }

    // random move if no LOS and not telepathic, or hasn't seen player, or is erratic
    if ((!C_IS(c, TELEPATHIC) && !los) || !c->dist_to_player || (C_IS(c, ERRATIC) && rand() % 2)) {
        return move_random(d, c);
    }

    point p;

    // if intelligent, take shortest path
    if (C_IS(c, INTELLIGENT)) {
        int shortest = 99999;
        // find neighboring tile closest to player
        for (int i = c->pos.r - 1; i <= c->pos.r + 1; i++) {
            for (int j = c->pos.c - 1; j <= c->pos.c + 1; j++) {
                if (!IN_BOUNDS(i,j)) {
                    continue;
                }

                if ((*c->dist_to_player)[i][j] < shortest) {
                    p = (point){i, j};
                    shortest = (*c->dist_to_player)[i][j];
                }
            }
        }
    } else { // move in straight line path toward player
        // move up, down, or neither
        if (c->pos.r < d->player_pos.r) {
            p.r = c->pos.r + 1;
        } else if (c->pos.r > d->player_pos.r) {
            p.r = c->pos.r - 1;
        } else {
            p.r = c->pos.r;
        }

        // move left, right, or neither
        if (c->pos.c < d->player_pos.c) {
            p.c = c->pos.c + 1;
        } else if (c->pos.c > d->player_pos.c) {
            p.c = c->pos.c - 1;
        } else {
            p.c = c->pos.c;
        }
    }
    move_to(d, c, p);

    return 0;
}

int move_random(dungeon *d, character *c) {
    int new_r;
    int new_c;

    do {
        new_r = c->pos.r + rand() % 3 - 1;
        new_c = c->pos.c + rand() % 3 - 1;
    } while (!IN_BOUNDS(new_r,new_c) || 
            (!C_IS(c, TUNNELING) && d->tiles[new_r][new_c].hardness));

    if (new_r == c->pos.r && new_c == c->pos.c) {
        // skip if no move
        return 0;
    }

    move_to(d, c, (point){new_r, new_c});

    return 0;
}

int move_to(dungeon *d, character *c, point p) {
    // if no move, return
    if (c->pos.r == p.r && c->pos.c == p.c) {
        return 0;
    }

    // if moving into rock, check tunneling
    if (d->tiles[p.r][p.c].hardness && C_IS(c, TUNNELING) ) {
        d->tiles[p.r][p.c].hardness -= 85;
    }

    // if hardness still greater than 0, early exit. otherwise move into rock, create corridor
    if (d->tiles[p.r][p.c].hardness > 0) {
        return 0;
    } else if (d->tiles[p.r][p.c].hardness < 0){
        d->tiles[p.r][p.c].hardness = 0;
        d->tiles[p.r][p.c].sprite = '#';
    }

    // if player, update dungeon
    if (c->traits == PLAYER_TRAIT) {
        d->player_pos = p;
        calc_dists(d, d->dists, d->player_pos, 0);
        calc_dists(d, d->tunnel_dists, d->player_pos, 1);
    }

    // check collision
    if (d->character_map[p.r][p.c]) {
        // check if player is killed
        int end_game = 0;
        if (d->character_map[p.r][p.c]->traits == PLAYER_TRAIT) end_game = 1;

        // kill character
        free(d->character_map[p.r][p.c]);
        d->character_map[p.r][p.c] = NULL;

        if (end_game) return 1;
    }

    d->character_map[c->pos.r][c->pos.c] = NULL;
    d->character_map[p.r][p.c] = c;
    c->pos = p;

    return 0;
}

// bresenham's line drawing alg
//
// returns 0 if no LOS, 1 otherwise
int check_los(dungeon *d, character *c) {
    point source = c->pos;
    point dest = d->player_pos;
    int los = 0;

    if (abs(dest.c-source.c) > abs(dest.r - source.r)) {
        los = check_horizontal(d, c);
        return check_horizontal(d, c);
    } else {
        los = check_vertical(d, c);
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
