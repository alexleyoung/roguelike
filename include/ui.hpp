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

void draw_dungeon(dungeon *d);
void draw_message(char *fmt, ...);
void draw_monster_list(dungeon *d, character *c);

#endif
