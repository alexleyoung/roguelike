#include <ncurses.h>
#include <ui.h>

void draw_dungeon(dungeon *d) {
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

void draw_status_message(char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  move(0, 0);
  clrtoeol();
  vw_printw(stdscr, fmt, args);

  va_end(args);
}
