#ifndef GAME_LOOP_H
#define GAME_LOOP_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <character.h>
#include <dungeon.h>
#include <game.h>
#include <gen.h>
#include <heap.h>
#include <movement.h>
#include <ncurses.h>
#include <ui.h>

int init_game(game *g);
int start_game(game *g);

#endif
