#include <stdio.h>
#include <stdlib.h>

#include <movement.h>
#include <ncurses.h> // drawing in the move file feels super bad but oh well...
#include <pathfinding.h>
#include <ui.h>
#include <utils.h>

#define ATTRIBUTE_INTELLIGENT 0x1
#define ATTRIBUTE_TELEPATHIC 0x2
#define ATTRIBUTE_TUNNELING 0x4
#define ATTRIBUTE_ERRATIC 0x8

int move_random(dungeon *d, character *c);
int move_to(dungeon *d, character *c, point p);
int check_los(dungeon *d, character *c);
int check_horizontal(dungeon *d, character *c);
int check_vertical(dungeon *d, character *c);

int move_player(dungeon *d, character *c, int move) {
  point p;

  switch (move) {
  //// movement
  // up left
  case KEY_HOME:
  case '7':
  case 'y':
    p.r = c->pos.r - 1;
    p.c = c->pos.c - 1;
    if (!IN_BOUNDS(p.r, p.c)) {
      draw_status_message("Can't move out of bounds!");
      return 0;
    } else {
      return move_to(d, c, p);
    }
  // up
  case KEY_UP:
  case '8':
  case 'k':
    p.r = c->pos.r - 1;
    p.c = c->pos.c;
    if (!IN_BOUNDS(p.r, p.c)) {
      draw_status_message("Can't move out of bounds!");
      return 0;
    } else {
      return move_to(d, c, p);
    }
  // up right
  case KEY_PPAGE:
  case '9':
  case 'u':
    p.r = c->pos.r - 1;
    p.c = c->pos.c + 1;
    if (!IN_BOUNDS(p.r, p.c)) {
      draw_status_message("Can't move out of bounds!");
      return 0;
    } else {
      return move_to(d, c, p);
    }
  // right
  case KEY_RIGHT:
  case '6':
  case 'l':
    p.r = c->pos.r;
    p.c = c->pos.c + 1;
    if (!IN_BOUNDS(p.r, p.c)) {
      draw_status_message("Can't move out of bounds!");
      return 0;
    } else {
      return move_to(d, c, p);
    }
  // down right
  case KEY_NPAGE:
  case '3':
  case 'n':
    p.r = c->pos.r + 1;
    p.c = c->pos.c + 1;
    if (!IN_BOUNDS(p.r, p.c)) {
      draw_status_message("Can't move out of bounds!");
      return 0;
    } else {
      return move_to(d, c, p);
    }
  // down
  case KEY_DOWN:
  case '2':
  case 'j':
    p.r = c->pos.r + 1;
    p.c = c->pos.c;
    if (!IN_BOUNDS(p.r, p.c)) {
      draw_status_message("Can't move out of bounds!");
      return 0;
    } else {
      return move_to(d, c, p);
    }
  // down left
  case KEY_END:
  case '1':
  case 'b':
    p.r = c->pos.r + 1;
    p.c = c->pos.c - 1;
    if (!IN_BOUNDS(p.r, p.c)) {
      draw_status_message("Can't move out of bounds!");
      return 0;
    } else {
      return move_to(d, c, p);
    }
  // left
  case KEY_LEFT:
  case '4':
  case 'h':
    p.r = c->pos.r;
    p.c = c->pos.c - 1;
    if (!IN_BOUNDS(p.r, p.c)) {
      draw_status_message("Can't move out of bounds!");
      return 0;
    } else {
      return move_to(d, c, p);
    }
  // rest
  case KEY_B2:
  case ' ':
  case '.':
  case '5':
    return 0;
  // stairs
  case '>':
    draw_status_message("down stair");
    break;
  case '<':
    draw_status_message("up stair");
    break;

  //// actions
  // drop item
  case 'd':
    draw_status_message("drop item");
    break;
  // take off item
  case 't':
    draw_status_message("take off item");
    break;
  // wear item
  case 'w':
    draw_status_message("wear item");
    break;
  // expunge item
  case 'x':
    draw_status_message("expunge item");
    break;
  // display equipment
  case 'e':
    draw_status_message("display equipment");
    break;
  // display inv
  case 'i':
    draw_status_message("display inventory");
    break;
  // inspect item
  case 'E':
    draw_status_message("expunge item");
    break;
  // player info
  case 'c':
    draw_status_message("character info");
    break;
  // monster list
  case 'm':
    draw_status_message("monster list");
    break;

  //// debug
  // toggle fow
  case 'f':
    draw_status_message("fog of war");
    break;
  // tp (goto)
  case 'g':
    draw_status_message("teleport");
    break;
  // default terrain map
  case 's':
    draw_status_message("terrain map");
    break;
  // hardness map
  case 'H':
    draw_status_message("hardness map");
    break;
  // non-tunneling dist map
  case 'D':
    draw_status_message("non-tunneling dist map");
    break;
  // tunneling dist map
  case 'T':
    draw_status_message("tunneling dist map");
    break;
  // look monster
  case 'L':
    draw_status_message("look monster");
    break;

  //// quit game
  case 'Q':
    draw_status_message("Quitting game...");
    refresh();
    return 1;

  default:
    draw_status_message("Invalid key press: %c", move);
    break;
  };

  return 0;
};

int move_character(dungeon *d, character *c) {
  // only random player movement for now
  if (c->traits == PLAYER_TRAIT) {
    return move_random(d, c);
  }

  // update character dist_to_player if telepathic or has los
  int los = check_los(d, c);
  if (C_IS(c, TELEPATHIC) || los) {
    if (C_IS(c, TUNNELING)) {
      COPY_2D_ARRAY(c->dist_to_player, d->tunnel_dists, DUNGEON_HEIGHT,
                    DUNGEON_WIDTH);
    } else {
      COPY_2D_ARRAY(c->dist_to_player, d->dists, DUNGEON_HEIGHT, DUNGEON_WIDTH);
    }
  }

  // random move if no LOS and not telepathic, or hasn't seen player, or is
  // erratic
  if ((!C_IS(c, TELEPATHIC) && !los) || (C_IS(c, ERRATIC) && rand() % 2)) {
    return move_random(d, c);
  }

  point p;

  // if intelligent, take shortest path
  if (C_IS(c, INTELLIGENT)) {
    int shortest = 99999;
    // find neighboring tile closest to player
    for (int i = c->pos.r - 1; i <= c->pos.r + 1; i++) {
      for (int j = c->pos.c - 1; j <= c->pos.c + 1; j++) {
        if (!IN_BOUNDS(i, j)) {
          continue;
        }
        if (c->dist_to_player[i][j] < shortest) {
          p = (point){i, j};
          shortest = c->dist_to_player[i][j];
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

  if (c->pos.r == p.r && c->pos.c == p.c) {
    return 0;
  }

  /*printf("moving to (%d, %d)\n", p.r, p.c);*/
  return (move_to(d, c, p));
}

int move_random(dungeon *d, character *c) {
  int new_r;
  int new_c;

  /*printf("TUNNELING: %d\n", C_IS(c, TUNNELING));*/
  /*printf("id: %d, sprite: %c\n", c->id, c->sprite);*/
  /*printf("r: %d, c: %d\n", c->pos.r, c->pos.c);*/
  int tries = 0;
  do {
    new_r = c->pos.r + rand() % 3 - 1;
    new_c = c->pos.c + rand() % 3 - 1;
    tries++;
  } while ((!IN_BOUNDS(new_r, new_c) ||
            (!C_IS(c, TUNNELING) && d->tiles[new_r][new_c].hardness)) &&
           tries < 100);

  // TODO: this is a temporary fix
  if (tries >= 100) {
    return 0;
  }

  if (new_r == c->pos.r && new_c == c->pos.c) {
    // skip if no move
    return 0;
  }

  return (move_to(d, c, (point){new_r, new_c}));
}

int move_to(dungeon *d, character *c, point p) {
  // if moving into rock, check tunneling: if not, return
  if (d->tiles[p.r][p.c].hardness) {
    if (C_IS(c, TUNNELING)) {
      d->tiles[p.r][p.c].hardness -= 85;
    } else {
      return 0;
    }
  }

  // if hardness still greater than 0, early exit. otherwise move into rock,
  // create corridor
  if (d->tiles[p.r][p.c].hardness > 0) {
    return 0;
  }

  if (d->tiles[p.r][p.c].hardness <= 0) {
    d->tiles[p.r][p.c].hardness = 0;
    if (d->tiles[p.r][p.c].sprite == ' ') {
      d->tiles[p.r][p.c].sprite = '#';
    }
  }

  // check collision
  if (d->character_map[p.r][p.c]) {
    int end = 0;
    if (d->character_map[p.r][p.c]->traits == PLAYER_TRAIT)
      end = 1; // end game

    d->character_map[p.r][p.c]->alive = 0;
    d->character_map[p.r][p.c] = NULL;

    if (end)
      return end;
  }

  // if player, update dungeon
  if (c->traits == PLAYER_TRAIT) {
    d->player_pos = p;
    calc_dists(d, d->dists, d->player_pos, 0);
    calc_dists(d, d->tunnel_dists, d->player_pos, 1);
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

  if (abs(dest.c - source.c) > abs(dest.r - source.r)) {
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
