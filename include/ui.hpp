#ifndef UI_HPP
#define UI_HPP

#include <stdarg.h>
#include <stdlib.h>

#include <character.hpp>
#include <dungeon.hpp>
#include <ncurses.h>
#include <types.hpp>
#include <utils.hpp>

#define KEY_ESC 27

#define SCREEN_HEIGHT 24
#define SCREEN_WIDTH 80
#define STATUS_LINE 0
#define DUNGEON_TOP 1
#define DUNGEON_BOTTOM 21
#define DUNGEON_HEIGHT 21
#define INFO_START 22
#define INFO_END 23

#define MONSTERS_PER_PAGE 20 // max num displayed monsters

void ui_init();
void draw_dungeon(Dungeon *d);
void draw_player_dungeon(Dungeon *d, Player *p);
void draw_message(const char *fmt, ...);
void draw_monster_list(Dungeon *d, Character *c);
void draw_player_teleport(Dungeon *d, Player *pl, Point *p);
void draw_look_cursor(Dungeon *d, Player *p, Point *target);
void draw_inventory(Player *p);
void draw_equipment(Player *p);
void draw_object_info(Object *);
void draw_player_info(Player *);
void draw_monster_info(Monster *);

#endif
