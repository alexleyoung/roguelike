#include "types.hpp"
#include <ui.hpp>

// helpers
void draw_formatted_monster_line(int offset, character *p, character *m);
void draw_monster_page(character **monsters, int num_monsters, int page,
                       int pages, character *p);

/*
 * Draw dungeon
 *
 * Params:
 * d: current dungeon
 */
void draw_dungeon(dungeon *d) {
  /*clear();*/

  int r, c;
  for (r = 0; r < DUNGEON_HEIGHT; r++) {
    for (c = 0; c < DUNGEON_WIDTH; c++) {
      if (d->character_map[r][c]) {
        mvaddch(r + 1, c, d->character_map[r][c]->sprite);
      } else {
        mvaddch(r + 1, c, d->tiles[r][c].sprite);
      }
    }
  }
}

/*
 * Draw dungeon
 *
 * Params:
 * d: current dungeon
 */
void draw_player_dungeon(dungeon *d, player *p) {
  /*clear();*/

  int r, c;
  for (r = 0; r < DUNGEON_HEIGHT; r++) {
    for (c = 0; c < DUNGEON_WIDTH; c++) {
      if (!p->terrain[r][c]) {
        mvaddch(r + 1, c, ' ');
        continue;
      }

      if (d->character_map[r][c] && r <= p->pos.r + 2 && p->pos.r - 2 <= r &&
          c <= p->pos.c + 2 && p->pos.c - 2 <= c) {
        mvaddch(r + 1, c, d->character_map[r][c]->sprite);
      } else {
        mvaddch(r + 1, c, d->tiles[r][c].sprite);
      }
    }
  }
}

/*
 * Printw wrapper which prints top line message
 *
 * Params:
 * fmt: format string
 */
void draw_message(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  move(0, 0);
  clrtoeol();
  vw_printw(stdscr, fmt, args);

  va_end(args);
}

/*
 * Opens monster list menu
 *
 * Params:
 * d: current dungeon
 * *c: player
 */
void draw_monster_list(dungeon *d, character *c) {
  // count monsters and make array
  int num_monsters = 0;
  for (int row = 0; row < DUNGEON_HEIGHT; row++) {
    for (int col = 0; col < DUNGEON_WIDTH; col++) {
      character *m = d->character_map[row][col];
      if (m && m != c) {
        num_monsters++;
      }
    }
  }
  character **monsters =
      (character **)(malloc(num_monsters * sizeof(*monsters)));
  int idx = 0;
  for (int row = 0; row < DUNGEON_HEIGHT; row++) {
    for (int col = 0; col < DUNGEON_WIDTH; col++) {
      character *m = d->character_map[row][col];
      if (m && m != c) {
        monsters[idx++] = m;
      }
    }
  }

  // window of monsters to look at
  int pages = CEIL(num_monsters, MONSTERS_PER_PAGE);
  int page = 1;
  draw_monster_page(monsters, num_monsters, page, pages, c);

  int inp;
  do {
    if (inp == KEY_DOWN && page < pages) {
      draw_monster_page(monsters, num_monsters, ++page, pages, c);
    }
    if (inp == KEY_UP && page > 1) {
      draw_monster_page(monsters, num_monsters, --page, pages, c);
    }
  } while ((inp = getch()) != 27); // 27 = esc

  free(monsters);
}

void draw_player_teleport(dungeon *d, player *p, point *target) {
  point old = p->pos;
  *target = p->pos;

  while (true) {
    switch (getch()) {
    //// movement
    // up left
    case KEY_HOME:
    case '7':
    case 'y':
      old = *target;
      target->r = target->r - 1;
      target->c = target->c - 1;
      if (!IN_BOUNDS(target->r, target->c)) {
        draw_message("Can't move out of bounds!");
        *target = old;
      }
      break;
    // up
    case KEY_UP:
    case '8':
    case 'k':
      old = *target;
      target->r = target->r - 1;
      if (!IN_BOUNDS(target->r, target->c)) {
        draw_message("Can't move out of bounds!");
        *target = old;
      }
      break;
    // up right
    case KEY_PPAGE:
    case '9':
    case 'u':
      old = *target;
      target->r = target->r - 1;
      target->c = target->c + 1;
      if (!IN_BOUNDS(target->r, target->c)) {
        draw_message("Can't move out of bounds!");
        *target = old;
      }
      break;
    // right
    case KEY_RIGHT:
    case '6':
    case 'l':
      old = *target;
      target->c = target->c + 1;
      if (!IN_BOUNDS(target->r, target->c)) {
        draw_message("Can't move out of bounds!");
        *target = old;
      }
      break;
    // down right
    case KEY_NPAGE:
    case '3':
    case 'n':
      old = *target;
      target->r = target->r + 1;
      target->c = target->c + 1;
      if (!IN_BOUNDS(target->r, target->c)) {
        draw_message("Can't move out of bounds!");
        *target = old;
      }
      break;
    // down
    case KEY_DOWN:
    case '2':
    case 'j':
      old = *target;
      target->r = target->r + 1;
      if (!IN_BOUNDS(target->r, target->c)) {
        draw_message("Can't move out of bounds!");
        *target = old;
      }
      break;
    // down left
    case KEY_END:
    case '1':
    case 'b':
      old = *target;
      target->r = target->r + 1;
      target->c = target->c - 1;
      if (!IN_BOUNDS(target->r, target->c)) {
        draw_message("Can't move out of bounds!");
        *target = old;
      }
      break;
    // left
    case KEY_LEFT:
    case '4':
    case 'h':
      old = *target;
      target->c = target->c - 1;
      if (!IN_BOUNDS(target->r, target->c)) {
        draw_message("Can't move out of bounds!");
        *target = old;
      }
      break;
    case 'g':
      return;
    case 'r':
      do {
        target->r = rand() % 21;
        target->c = rand() % 80;
      } while (!IN_BOUNDS(target->r, target->c));
      return;
    }
    char old_sprite = d->character_map[old.r][old.c]
                          ? d->character_map[old.r][old.c]->sprite
                          : d->tiles[old.r][old.c].sprite;
    mvprintw(old.r + 1, old.c, "%c", old_sprite);
    mvprintw(target->r + 1, target->c, "%c", '*');
  }
}

// helper to draw single formatted monster line
void draw_formatted_monster_line(int line, character *p, character *m) {
  // print formatted info: sprite   d:rr d:cc
  mvprintw(line, 0, "%c", m->sprite);

  // get relative dists
  int v_dist = p->pos.r - m->pos.r;
  char v_cardinal = v_dist >= 0 ? 'n' : 's';
  v_dist = v_dist > 0 ? v_dist : v_dist * -1;
  mvprintw(line, 4, "%c:", v_cardinal);
  mvprintw(line, 6, "%d", v_dist);

  int h_dist = p->pos.c - m->pos.c;
  char h_cardinal = h_dist >= 0 ? 'w' : 'e';
  h_dist = h_dist > 0 ? h_dist : h_dist * -1;
  mvprintw(line, 9, "%c:", h_cardinal);
  mvprintw(line, 11, "%d", h_dist);
}

// draw slice of monster list
void draw_monster_page(character **monsters, int num_monsters, int page,
                       int pages, character *p) {
  clear();
  mvprintw(0, 0, "Monster List:");
  mvprintw(1, 0, "-------------");

  int line, left = (page - 1) * MONSTERS_PER_PAGE;
  for (line = 2; left < num_monsters && line < MONSTERS_PER_PAGE + 2; line++) {
    draw_formatted_monster_line(line, p, monsters[left++]);
  }

  mvprintw(23, 0, "Page %d/%d", page, pages);
}
