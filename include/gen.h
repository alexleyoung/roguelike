#ifndef GEN_H
#define GEN_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <corridor_heap.h>
#include <dungeon.h>
#include <queue.h>
#include <spawn.h>
#include <utils.h>

#define DEFAULT_HARDNESS 255

#define ROOM_MIN_WIDTH 4
#define ROOM_MIN_HEIGHT 3
#define ROOM_MAX_WIDTH 13
#define ROOM_MAX_HEIGHT 7

typedef struct seed {
  point p;
  int hardness;
} seed;

int init_dungeon(dungeon *dungeon);
int generate_dungeon(dungeon *dungeon, int num_rooms, int num_monsters);
int generate_linked_dungeon(dungeon *d, int num_rooms, int num_monster,
                            int link_id, int stair_type);

#endif
