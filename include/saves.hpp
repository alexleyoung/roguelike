#ifndef SAVES_HPP
#define SAVES_HPP

#include "dungeon.hpp"

char *get_save_path(const char *name);
int save_dungeon(Dungeon *dungeon, const char *name);
int load_dungeon(Dungeon *dungeon, const char *name);

#endif
