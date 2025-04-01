#ifndef GAME_LOOP_HPP
#define GAME_LOOP_HPP

#include <stdio.h>
#include <stdlib.h>

#include <character.hpp>
#include <dungeon.hpp>
#include <game.hpp>
#include <gen.hpp>
#include <heap.hpp>
#include <movement.hpp>
#include <ncurses.h>
#include <ui.hpp>

int init_game(game *g);
int start_game(game *g);

#endif
