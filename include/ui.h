#ifndef UI_H
#define UI_H

#include <stdarg.h>

#include <dungeon.h>
#include <ncurses.h>
#include <types.h>

void draw_dungeon(dungeon *d);
void draw_status_message(char *fmt, ...);

#endif
