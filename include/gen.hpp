#ifndef GEN_HPP
#define GEN_HPP

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <corridor_heap.hpp>
#include <dungeon.hpp>
#include <movement.hpp>
#include <queue.hpp>
#include <types.hpp>
#include <utils.hpp>

#define DEFAULT_HARDNESS 255

#define ROOM_MIN_WIDTH 4
#define ROOM_MIN_HEIGHT 3
#define ROOM_MAX_WIDTH 13
#define ROOM_MAX_HEIGHT 7

class seed {
public:
  point p;
  int hardness;
};

int init_dungeon(Dungeon *dungeon);
int generate_dungeon(Dungeon *dungeon, int num_rooms, int num_monsters,
                     int num_objects);
int generate_linked_dungeon(Dungeon *d, int num_rooms, int num_monster,
                            int num_objects, int link_id, int stair_type);

#endif
