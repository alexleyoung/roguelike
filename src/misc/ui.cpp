#include "character.hpp"
#include <ncurses.h>
#include <ui.hpp>

// helpers
void draw_formatted_monster_line(int offset, Character *p, Character *m);
void draw_monster_page(Character **monsters, int num_monsters, int page,
                       int pages, Character *p);

void ui_init() {
  // init ncurses and clear
  initscr();
  clear();

  cbreak();
  // disable cursor and input visibility
  noecho();
  curs_set(0);
  // enable keypad
  keypad(stdscr, TRUE);

  // init colors
  start_color();
  init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
  init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
  init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
  init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
  init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
  init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
  init_pair(COLOR_BLACK, COLOR_WHITE, COLOR_BLACK);
}

/*
 * Draw dungeon
 *
 * Params:
 * d: current dungeon
 */
void draw_dungeon(Dungeon *d) {
  /*clear();*/

  int r, c;
  Character *character;
  Object *object;
  for (r = 0; r < DUNGEON_HEIGHT; r++) {
    for (c = 0; c < DUNGEON_WIDTH; c++) {

      if ((character = d->character_map[r][c])) {
        attron(COLOR_PAIR(character->color));
        mvaddch(r + 1, c, character->sprite);
        attroff(COLOR_PAIR(character->color));
      } else if ((object = d->object_map[r][c])) {
        attron(COLOR_PAIR(object->color));
        mvaddch(r + 1, c, object->sprite);
        attroff(COLOR_PAIR(object->color));
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
void draw_player_dungeon(Dungeon *d, Player *p) {
  /*clear();*/

  int r, c;
  for (r = 0; r < DUNGEON_HEIGHT; r++) {
    for (c = 0; c < DUNGEON_WIDTH; c++) {
      if (!p->terrain[r][c]) {
        mvaddch(r + 1, c, ' ');
        continue;
      }

      Character *character;
      Object *object;
      if ((character = d->character_map[r][c]) && r <= p->pos.r + 2 &&
          p->pos.r - 2 <= r && c <= p->pos.c + 2 && p->pos.c - 2 <= c) {
        attron(COLOR_PAIR(character->color));
        mvaddch(r + 1, c, character->sprite);
        attroff(COLOR_PAIR(character->color));
      } else if ((object = d->object_map[r][c]) && r <= p->pos.r + 2 &&
                 p->pos.r - 2 <= r && c <= p->pos.c + 2 && p->pos.c - 2 <= c) {
        attron(COLOR_PAIR(object->color));
        mvaddch(r + 1, c, object->sprite);
        attroff(COLOR_PAIR(object->color));
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

  move(STATUS_LINE, 0);
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
void draw_monster_list(Dungeon *d, Character *c) {
  // count monsters and make array
  int num_monsters = 0;
  for (int row = 0; row < DUNGEON_HEIGHT; row++) {
    for (int col = 0; col < DUNGEON_WIDTH; col++) {
      Character *m = d->character_map[row][col];
      if (m && m != c) {
        num_monsters++;
      }
    }
  }
  Character **monsters =
      (Character **)(malloc(num_monsters * sizeof(*monsters)));
  int idx = 0;
  for (int row = 0; row < DUNGEON_HEIGHT; row++) {
    for (int col = 0; col < DUNGEON_WIDTH; col++) {
      Character *m = d->character_map[row][col];
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
  } while ((inp = getch()) != KEY_ESC); // 27 = esc

  free(monsters);
}

void draw_player_teleport(Dungeon *d, Player *p, Point *target) {
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
void draw_formatted_monster_line(int line, Character *p, Character *m) {
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
void draw_monster_page(Character **monsters, int num_monsters, int page,
                       int pages, Character *p) {
  clear();
  mvprintw(0, 0, "Monster List:");
  mvprintw(1, 0, "-------------");

  int line, left = (page - 1) * MONSTERS_PER_PAGE;
  for (line = 2; left < num_monsters && line < MONSTERS_PER_PAGE + 2; line++) {
    draw_formatted_monster_line(line, p, monsters[left++]);
  }

  mvprintw(23, 0, "Page %d/%d", page, pages);
}

void draw_inventory(Player *p) {
  int menu_width = 30; // 50 arbitrary menu width
  char horizontal_border_char = ' ';
  char vertical_border_char = ' ';

  int start_line = (DUNGEON_HEIGHT - NUM_INVENTORY_SLOTS) / 2 + DUNGEON_TOP;
  int start_col = (SCREEN_WIDTH - menu_width) / 2;

  std::string spaces(menu_width, ' ');
  std::string horizontal_border(menu_width, horizontal_border_char);

  int i;
  for (i = 0; i < NUM_INVENTORY_SLOTS; i++) {
    // draw vertical borders
    mvaddch(start_line + i, start_col - 1, vertical_border_char);
    mvaddch(start_line + i, start_col + menu_width, vertical_border_char);

    // get item info and print slot
    const char *item_name;
    p->inventory[i] ? item_name = p->inventory[i]->name.c_str()
                    : item_name = "";
    mvprintw(start_line + i, start_col, spaces.c_str());
    mvprintw(start_line + i, start_col, "slot %d: %s", i, item_name);
  }

  // draw horizontal border
  mvprintw(start_line - 1, start_col, "%s", horizontal_border.c_str());
  mvprintw(start_line + NUM_INVENTORY_SLOTS, start_col, "%s",
           horizontal_border.c_str());
  // fill corners
  mvaddch(start_line - 1, start_col - 1, horizontal_border_char);
  mvaddch(start_line - 1, start_col + menu_width, horizontal_border_char);
  mvaddch(start_line + NUM_INVENTORY_SLOTS, start_col - 1,
          horizontal_border_char);
  mvaddch(start_line + NUM_INVENTORY_SLOTS, start_col + menu_width,
          horizontal_border_char);
}

void draw_equipment(Player *p) {
  int menu_width = 30; // 50 arbitrary menu width
  char horizontal_border_char = ' ';
  char vertical_border_char = ' ';

  int start_line = (DUNGEON_HEIGHT - NUM_EQUIPMENT_SLOTS) / 2 + DUNGEON_TOP;
  int start_col = (SCREEN_WIDTH - menu_width) / 2;

  std::string spaces(menu_width, ' ');
  std::string horizontal_border(menu_width, horizontal_border_char);

  int i;
  for (i = 0; i < NUM_EQUIPMENT_SLOTS; i++) {
    // draw vertical borders
    mvaddch(start_line + i, start_col - 1, vertical_border_char);
    mvaddch(start_line + i, start_col + menu_width, vertical_border_char);

    // get slot and item info
    const char *item_name;
    const char *slot_name;
    p->equipment[i] ? item_name = p->equipment[i]->name.c_str()
                    : item_name = "";
    switch (i) {
    case WEAPON_SLOT:
      slot_name = "weapon (a)";
      break;
    case OFFHAND_SLOT:
      slot_name = "offhand (b)";
      break;
    case RANGED_SLOT:
      slot_name = "ranged (c)";
      break;
    case ARMOR_SLOT:
      slot_name = "armor (d)";
      break;
    case HELMET_SLOT:
      slot_name = "helmet (e)";
      break;
    case CLOAK_SLOT:
      slot_name = "cloak (f)";
      break;
    case GLOVES_SLOT:
      slot_name = "gloves (g)";
      break;
    case BOOTS_SLOT:
      slot_name = "boots (h)";
      break;
    case AMULET_SLOT:
      slot_name = "amulet (i)";
      break;
    case LIGHT_SLOT:
      slot_name = "light (j)";
      break;
    case RING1_SLOT:
      slot_name = "ring1 (k)";
      break;
    case RING2_SLOT:
      slot_name = "ring2 (l)";
      break;
    }

    // print slot and item
    mvprintw(start_line + i, start_col, spaces.c_str());
    mvprintw(start_line + i, start_col, "%s: %s", slot_name, item_name);
  }

  // draw horizontal border
  mvprintw(start_line - 1, start_col, "%s", horizontal_border.c_str());
  mvprintw(start_line + NUM_EQUIPMENT_SLOTS, start_col, "%s",
           horizontal_border.c_str());
  // fill corners
  mvaddch(start_line - 1, start_col - 1, horizontal_border_char);
  mvaddch(start_line - 1, start_col + menu_width, horizontal_border_char);
  mvaddch(start_line + NUM_EQUIPMENT_SLOTS, start_col - 1,
          horizontal_border_char);
  mvaddch(start_line + NUM_EQUIPMENT_SLOTS, start_col + menu_width,
          horizontal_border_char);
}
