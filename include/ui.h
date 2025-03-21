#ifndef UI_H
#define UI_H

#include <stdarg.h>
#include <stdlib.h>

#include <character.h>
#include <dungeon.h>
#include <ncurses.h>
#include <types.h>
#include <utils.h>

#define MONSTERS_PER_PAGE 20 // max num displayed monsters

void draw_dungeon(dungeon *d);
void draw_message(char *fmt, ...);
void draw_monster_list(dungeon *d, character *c);

#endif
