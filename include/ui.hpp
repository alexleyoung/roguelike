#ifndef UI_HPP
#define UI_HPP

#include <stdarg.h>
#include <stdlib.h>

#include <character.hpp>
#include <dungeon.hpp>
#include <ncurses.h>
#include <types.hpp>
#include <utils.hpp>

#define MONSTERS_PER_PAGE 20 // max num displayed monsters

void draw_dungeon(Dungeon *d);
void draw_player_dungeon(Dungeon *d, Player *p);
void draw_message(const char *fmt, ...);
void draw_monster_list(Dungeon *d, Character *c);
void draw_player_teleport(Dungeon *d, Player *pl, Point *p);

#endif
