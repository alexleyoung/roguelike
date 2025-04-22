#ifndef DUNGEON_HPP
#define DUNGEON_HPP

#include <stdint.h>

#include <character.hpp>
#include <heap.hpp>
#include <object.hpp>
#include <types.hpp>

enum { UP_STAIR, DOWN_STAIR };

#define UNLINKED -99

class Tile {
public:
  char sprite;
  int hardness;
};

class Room {
public:
  Point corner;
  Point size;
};

class Event {
public:
  int turn_time;
  Character *character;
};

class Stair {
public:
  point p;
  int type;
  int d; // dungeon id
};

class Dungeon {
public:
  int id;
  heap events;

  Tile tiles[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {};

  Room *rooms;
  uint16_t num_rooms;
  Stair *stairs;
  uint16_t num_stairs;

  int dists[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {};
  int tunnel_dists[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {};
  char player_map[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {};

  Point player_pos;

  Character *character_map[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {};
  Object *object_map[DUNGEON_HEIGHT][DUNGEON_WIDTH] = {};
};

#endif
