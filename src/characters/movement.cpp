#include "character.hpp"
#include "types.hpp"
#include "ui.hpp"
#include <cstring>
#include <movement.hpp>

#define ATTRIBUTE_INTELLIGENT 0x1
#define ATTRIBUTE_TELEPATHIC 0x2
#define ATTRIBUTE_TUNNELING 0x4
#define ATTRIBUTE_ERRATIC 0x8

int move_random(Dungeon *d, Character *c);
int move_to(Dungeon *d, Character *c, point p);
int check_los(Dungeon *d, Character *c);
int check_horizontal(Dungeon *d, Character *c);
int check_vertical(Dungeon *d, Character *c);

bool teleport = false;

int move_player(Dungeon *d, Character *c, int move) {
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
      draw_message("Can't move out of bounds!");
      return PLAYER_MOVE_INVALID;
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
      draw_message("Can't move out of bounds!");
      return PLAYER_MOVE_INVALID;
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
      draw_message("Can't move out of bounds!");
      return PLAYER_MOVE_INVALID;
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
      draw_message("Can't move out of bounds!");
      return PLAYER_MOVE_INVALID;
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
      draw_message("Can't move out of bounds!");
      return PLAYER_MOVE_INVALID;
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
      draw_message("Can't move out of bounds!");
      return PLAYER_MOVE_INVALID;
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
      draw_message("Can't move out of bounds!");
      return PLAYER_MOVE_INVALID;
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
      draw_message("Can't move out of bounds!");
      return PLAYER_MOVE_INVALID;
    } else {
      return move_to(d, c, p);
    }
  // rest
  case KEY_B2:
  case ' ':
  case '.':
  case '5':
    return PLAYER_MOVE;
  // stairs
  case '>':
    if (d->tiles[c->pos.r][c->pos.c].sprite != '>') {
      draw_message("Can't move down non-downward stairs!");
      return PLAYER_MOVE_INVALID;
    }
    return PLAYER_MOVE_STAIR;
  case '<':
    if (d->tiles[c->pos.r][c->pos.c].sprite != '<') {
      draw_message("Can't move down non-upward stairs!");
      return PLAYER_MOVE_INVALID;
    }
    return PLAYER_MOVE_STAIR;

  //// actions
  // drop item
  case 'd':
    draw_message("drop item");
    return PLAYER_MOVE_ACTION;
  // take off item
  case 't':
    draw_message("take off item");
    return PLAYER_MOVE_ACTION;
  // wear item
  case 'w':
    draw_message("wear item");
    return PLAYER_MOVE_ACTION;
  // expunge item
  case 'x':
    draw_message("expunge item");
    return PLAYER_MOVE_ACTION;
  // display equipment
  case 'e':
    draw_message("display equipment");
    return PLAYER_MOVE_MENU;
  // display inv
  case 'i':
    draw_message("display inventory");
    return PLAYER_MOVE_MENU;
  // inspect item
  case 'E':
    draw_message("inspect item");
    return PLAYER_MOVE_MENU;
  // Player info
  case 'c':
    draw_message("Character info");
    return PLAYER_MOVE_MENU;
  // Monster list
  case 'm':
    draw_monster_list(d, c);
    return PLAYER_MOVE_MENU;

  //// debug
  // toggle fow
  case 'f':
    return PLAYER_TOGGLE_FOG;
    break;
  // tp (goto)
  case 'g':
    draw_message("teleport");
    teleport = true;
    draw_player_teleport(d, (Player *)c, &p);
    move_to(d, c, p);
    teleport = false;
    return PLAYER_MOVE_ACTION;
  // default terrain map
  case 's':
    draw_message("terrain map");
    return PLAYER_MOVE_MENU;
  // hardness map
  case 'H':
    draw_message("hardness map");
    return PLAYER_MOVE_MENU;
  // non-tunneling dist map
  case 'D':
    draw_message("non-tunneling dist map");
    return PLAYER_MOVE_MENU;
  // tunneling dist map
  case 'T':
    draw_message("tunneling dist map");
    return PLAYER_MOVE_MENU;
  // look Monster
  case 'L':
    draw_message("look Monster");
    return PLAYER_MOVE_MENU;

  //// quit game
  case 'Q':
    draw_message("Quitting game...");
    refresh();
    return PLAYER_MOVE_QUIT;

  default:
    draw_message("Invalid key press: %c", move);
    return PLAYER_MOVE_INVALID;
  };

  return 0;
};

int move_character(Dungeon *d, Character *c) {
  // only random Player movement for now
  if (c->id == 0) {
    return move_random(d, c);
  }

  Monster *m = c->type == MONSTER ? static_cast<Monster *>(c) : NULL;

  // update Character dist_to_player if telepathic or has los
  int los = check_los(d, c);
  if (C_IS(m, TELEPATHIC) || los) {
    if (C_IS(m, TUNNELING)) {
      COPY_2D_ARRAY(m->dist_to_player, d->tunnel_dists, DUNGEON_HEIGHT,
                    DUNGEON_WIDTH);
    } else {
      COPY_2D_ARRAY(m->dist_to_player, d->dists, DUNGEON_HEIGHT, DUNGEON_WIDTH);
    }
  }

  // random move if no LOS and not telepathic, or hasn't seen Player, or is
  // erratic
  if ((!C_IS(m, TELEPATHIC) && !los) || (C_IS(m, ERRATIC) && rand() % 2)) {
    return move_random(d, c);
  }

  point p;

  // if intelligent, take shortest path
  if (C_IS(m, INTELLIGENT)) {
    int shortest = 99999;
    // find neighboring tile closest to Player
    for (uint8_t i = c->pos.r - 1; i <= c->pos.r + 1; i++) {
      for (uint8_t j = c->pos.c - 1; j <= c->pos.c + 1; j++) {
        if (!IN_BOUNDS(i, j)) {
          continue;
        }
        if (m->dist_to_player[i][j] < shortest) {
          p = (point){i, j};
          shortest = m->dist_to_player[i][j];
        }
      }
    }
  } else { // move in straight line path toward Player
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

int move_random(Dungeon *d, Character *c) {
  uint8_t new_r;
  uint8_t new_c;

  /*printf("TUNNELING: %d\n", C_IS(c, TUNNELING));*/
  /*printf("id: %d, sprite: %c\n", c->id, c->sprite);*/
  /*printf("r: %d, c: %d\n", c->pos.r, c->pos.c);*/
  int tries = 0;

  Player *p = c->type == PLAYER ? static_cast<Player *>(c) : NULL;
  Monster *m = c->type == MONSTER ? static_cast<Monster *>(c) : NULL;

  do {
    new_r = c->pos.r + rand() % 3 - 1;
    new_c = c->pos.c + rand() % 3 - 1;
    tries++;
  } while ((!IN_BOUNDS(new_r, new_c) ||
            (m && !C_IS(m, TUNNELING) && d->tiles[new_r][new_c].hardness)) &&
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

int move_to(Dungeon *d, Character *c, point p) {
  Player *pl = c->type == PLAYER ? static_cast<Player *>(c) : NULL;
  Monster *m = c->type == MONSTER ? static_cast<Monster *>(c) : NULL;

  // if moving into rock, check tunneling: if not, return
  if (d->tiles[p.r][p.c].hardness && !teleport) {
    if (m && C_IS(m, TUNNELING)) {
      d->tiles[p.r][p.c].hardness -= 85;
    } else if (pl) {
      return PLAYER_MOVE_INVALID;
    } else {
      return 0;
    }
  }

  // if hardness still greater than 0, early exit. otherwise move into rock,
  // create corridor
  if (d->tiles[p.r][p.c].hardness > 0 && !pl && !teleport) {
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
    // if Player killed, end game
    if (d->character_map[p.r][p.c]->type == PLAYER)
      return 1;

    d->character_map[p.r][p.c]->alive = 0;
    d->character_map[p.r][p.c] = NULL;
  }

  // if Player, update Dungeon and vision
  if (pl) {
    d->player_pos = p;
    calc_dists(d, d->dists, d->player_pos, 0);
    calc_dists(d, d->tunnel_dists, d->player_pos, 1);

    update_player_vision(d, pl);
  }

  d->character_map[c->pos.r][c->pos.c] = NULL;
  d->character_map[p.r][p.c] = c;
  c->pos = p;

  return 0;
}

int update_player_vision(Dungeon *d, Player *p) {
  /*memset(p->characters, 0,*/
  /*       sizeof(*p->characters) * DUNGEON_HEIGHT * DUNGEON_WIDTH);*/

  for (int r = p->pos.r - 2; r <= p->pos.r + 2; r++) {
    for (int c = p->pos.c - 2; c <= p->pos.c + 2; c++) {
      if (!IN_BOUNDS(r, c) || (r == p->pos.r && c == p->pos.c))
        continue;

      /*p->characters[r][c] = d->character_map[r][c];*/
      p->terrain[r][c] = d->tiles[r][c].sprite;
      draw_player_dungeon(d, p);
    }
  }

  return 0;
}

// bresenham's line drawing alg
//
// returns 0 if no LOS, 1 otherwise
int check_los(Dungeon *d, Character *c) {
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

int check_horizontal(Dungeon *d, Character *c) {
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

int check_vertical(Dungeon *d, Character *c) {
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
