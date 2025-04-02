#include "movement.hpp"
#include <gen.hpp>

#define MAX_ROOM_TRIES 5000

int init_dungeon(dungeon *dungeon);
int generate_rooms(dungeon *dungeon);
int place_room(dungeon *dungeon, room *room);
int generate_hardness(dungeon *dungeon);
void propagate_hardness(dungeon *dungeon,
                        int propagated[DUNGEON_HEIGHT][DUNGEON_WIDTH], queue *q,
                        seed *s);
void smooth_hardness(dungeon *dungeon);
int generate_corridors(dungeon *dungeon, room *rooms);
int find_path(dungeon *dungeon, point source, point target, int longest);
int place_stairs(dungeon *dungeon, int num_stairs);
int spawn_player(dungeon *dungeon);
int spawn_monsters(dungeon *dungeon, int n);
int init_heap(dungeon *dungeon);

/*
Generate a dungeon with num_rooms number of rooms and at least 2 stairs.

Randomly places rectangular rooms, then creates a smooth hardness gradient
for corridor placement.

Returns 0 on success, non-zero on failure.
*/
int generate_dungeon(dungeon *dungeon, int num_rooms, int num_monsters) {
  dungeon->num_rooms = num_rooms;

  int err = 0;

  err = init_dungeon(dungeon);
  err = generate_hardness(dungeon);
  err = generate_rooms(dungeon);
  err = generate_corridors(dungeon, dungeon->rooms);
  err = place_stairs(dungeon, 2);
  err = spawn_player(dungeon);
  err = spawn_monsters(dungeon, num_monsters);
  err = init_heap(dungeon);

  return err;
}

/*
Generate a dungeon with a linked staircase

Params:
dungeon *d: dungeon to generate
int num_rooms: number of rooms to generate
int num_monsters: number of monsters to spawn
int link_id: dungeon id to link connector staircase to
int stair_type: connector stair type

Returns 0 on success, non-zero on failure.
*/
int generate_linked_dungeon(dungeon *d, int num_rooms, int num_monsters,
                            int link_id, int stair_type) {
  int err;

  // generate new dungeon
  err = generate_dungeon(d, num_rooms, num_monsters);

  // resize stair arr
  stair *tmp;
  if (!(tmp = (stair *)(realloc(d->stairs,
                                ++d->num_stairs * sizeof(*d->stairs))))) {
    return 1;
  }
  d->stairs = tmp;

  // make new connected stair
  stair s = {.p = d->player_pos, .type = stair_type, .d = link_id};
  /*stair s;*/
  /*s.p = d->player_pos;*/
  /*s.type = stair_type;*/
  /*s.d = link_id;*/
  d->stairs[d->num_stairs - 1] = s;
  d->tiles[s.p.r][s.p.c].sprite = stair_type == UP_STAIR ? '<' : '>';

  return err;
}

// event comparator for dungeon game loop heap
static int compare_events(const void *v1, const void *v2) {
  event *event1 = (event *)v1;
  event *event2 = (event *)v2;

  int diff;

  diff = event1->turn_time - event2->turn_time;
  if (diff) {
    return diff;
  } else {
    return event1->character->id - event2->character->id;
  }
}

/*
initialize dungeon with rock and default hardness and draw border

returns 0 on success, non-zero on failure
*/
int init_dungeon(dungeon *dungeon) {
  srand(time(NULL)); // seed RNG

  int r, c;

  // fill map with rock and default hardness and characters with null
  for (r = 0; r < DUNGEON_HEIGHT; r++) {
    for (c = 0; c < DUNGEON_WIDTH; c++) {
      dungeon->tiles[r][c].sprite = ' ';
      dungeon->tiles[r][c].hardness = DEFAULT_HARDNESS;
      dungeon->character_map[r][c] = NULL;
    }
  }

  // draw border
  for (r = 0; r < DUNGEON_HEIGHT; r++) {
    dungeon->tiles[r][0].sprite = '|';
    dungeon->tiles[r][DUNGEON_WIDTH - 1].sprite = '|';
  }
  for (c = 0; c < DUNGEON_WIDTH; c++) {
    dungeon->tiles[0][c].sprite = '-';
    dungeon->tiles[DUNGEON_HEIGHT - 1][c].sprite = '-';
  }

  return 0;
}

/*
Generates a smooth hardness map over the dungeon

Returns 0 on success, non-zero otherwise
*/
int generate_hardness(dungeon *dungeon) {
  int i;
  queue q;
  queue_init(&q);
  int num_seeds;
  seed *s;
  int propagated[DUNGEON_HEIGHT][DUNGEON_WIDTH];

  num_seeds = (rand() % 5) + 1;

  // place hardness seeds, add to queue for bfs
  for (i = 0; i < num_seeds; i++) {
    s = (seed *)(malloc(sizeof(*s)));

    // pick a random empty tile
    do {
      s->p.r = rand() % DUNGEON_HEIGHT;
      s->p.c = rand() % DUNGEON_WIDTH;
    } while (s->p.r == 0 || s->p.r == DUNGEON_HEIGHT - 1 || s->p.c == 0 ||
             s->p.c == DUNGEON_WIDTH - 1);

    s->hardness = ((rand() % 20) + 1) * 11;

    queue_enqueue(&q, s);
  }

  // propagate hardness outward
  while (!queue_is_empty(&q)) {
    void *data;
    if (queue_dequeue(&q, &data) == 0) {
      s = (seed *)data;
    }
    dungeon->tiles[s->p.r][s->p.c].hardness = s->hardness;
    propagate_hardness(dungeon, propagated, &q, s);
    free(s);
  }

  // set missed spots to average of sum of neighbors
  for (int r = 1; r < DUNGEON_HEIGHT - 1; r++) {
    for (int c = 1; c < DUNGEON_WIDTH - 1; c++) {
      if (dungeon->tiles[r][c].hardness == DEFAULT_HARDNESS) {
        int sum = 0;
        int dirs = 8;
        int dr[] = {-1, 1, 0, 0, -1, 1, 0, 0};
        int dc[] = {0, 0, -1, 1, 0, 0, -1, 1};
        for (int i = 0; i < dirs; i++) {
          int nr = r + dr[i];
          int nc = c + dc[i];
          if (nr < 1 || nr > DUNGEON_HEIGHT - 2 || nc < 1 ||
              nc > DUNGEON_WIDTH - 2) {
            continue;
          }
          sum += dungeon->tiles[nr][nc].hardness;
        }
        dungeon->tiles[r][c].hardness = sum / dirs;
        if (dungeon->tiles[r][c].hardness == 0) {
          dungeon->tiles[r][c].hardness = 1;
        }
      }
    }
  }

  queue_destroy(&q);

  smooth_hardness(dungeon);

  return 0;
}

/*
BFS helper to propagate hardness from seeds
*/
void propagate_hardness(dungeon *dungeon,
                        int propagated[DUNGEON_HEIGHT][DUNGEON_WIDTH], queue *q,
                        seed *s) {
  int i, r, c;
  seed *child;

  // add neighbors to queue
  // 4 directions for better randomness, issue is that it's possible miss a spot
  int dirs = 4;
  int dr[] = {-1, 1, 0, 0};
  int dc[] = {0, 0, -1, 1};
  for (i = 0; i < dirs; i++) {
    r = s->p.r + dr[i];
    c = s->p.c + dc[i];
    // check bounds
    if (r < 1 || r > DUNGEON_HEIGHT - 2 || c < 1 || c > DUNGEON_WIDTH - 2) {
      continue;
    }
    // check if neighbor is already visited
    if (propagated[r][c] == 1) {
      continue;
    }

    // update hardness
    dungeon->tiles[s->p.r][s->p.c].hardness = s->hardness;

    // add to queue
    child = (seed *)(malloc(sizeof(*child)));
    child->p.r = r;
    child->p.c = c;
    child->hardness = s->hardness;
    queue_enqueue(q, child);

    // mark as visited
    propagated[r][c] = 1;
  }
}

/*
Apply gaussian blur to hardness map for smoothing
*/
void smooth_hardness(dungeon *dungeon) {
  int r, c, i, j;

  // kernel for guassian blur
  // int kernel[5][5] = {
  //  {1, 2, 3, 2, 1},
  //  {2, 4, 6, 4, 2},
  //  {3, 6, 9, 6, 3},
  //  {2, 4, 6, 4, 2},
  //  {1, 2, 3, 2, 1}
  // };
  // int kernel_size = 5;
  int kernel[3][3] = {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}};
  int kernel_size = 3;
  int kernal_offset = kernel_size / 2;
  int kernel_sum = 0;
  for (int i = 0; i < kernel_size; i++) {
    for (int j = 0; j < kernel_size; j++) {
      kernel_sum += kernel[i][j];
    }
  }

  // hardness map copy
  int blurred_hardness[DUNGEON_HEIGHT][DUNGEON_WIDTH];

  for (r = 1; r < DUNGEON_HEIGHT - 1; r++) {
    for (c = 1; c < DUNGEON_WIDTH - 1; c++) {
      int new_hardness = 0;
      for (i = 0; i < kernel_size; i++) {
        for (int j = 0; j < kernel_size; j++) {
          int nr = r + i - kernal_offset; // neighbor row
          int nc = c + j - kernal_offset; // neighbor column
          // careful not to include border
          if (nr < 1 || nr > DUNGEON_HEIGHT - 2 || nc < 1 ||
              nc > DUNGEON_WIDTH - 2) {
            continue;
          }
          new_hardness += dungeon->tiles[nr][nc].hardness * kernel[i][j];
        }
      }
      // average with kernel sum
      blurred_hardness[r][c] = new_hardness / kernel_sum;
      if (blurred_hardness[r][c] == 0) {
        blurred_hardness[r][c] = 1;
      }
    }
  }

  // transfer calculated hardness back to the dungeon tiles
  for (int r = 1; r < DUNGEON_HEIGHT - 1; r++) {
    for (int c = 1; c < DUNGEON_WIDTH - 1; c++) {
      dungeon->tiles[r][c].hardness = blurred_hardness[r][c];
    }
  }
}

/*
Randomly generates num_rooms rectangular rooms in the dungeon to be placed

Returns 0 on success, non-zero on failure.
*/
int generate_rooms(dungeon *dungeon) {
  dungeon->rooms =
      (room *)(malloc(sizeof(*dungeon->rooms) * dungeon->num_rooms));

  int i, r, c, err;

  for (i = 0; i < dungeon->num_rooms; i++) {
    room *room = &dungeon->rooms[i];
    point corner, size; // for room validation

    int invalid = 0;
    int tries = 1;

    while (!invalid && tries < 5000) {
      // pick room corner and size
      corner.r = (rand() % DUNGEON_HEIGHT - 1) + 1;
      corner.c = (rand() % DUNGEON_WIDTH - 1) + 1;
      size.r = rand() % (ROOM_MAX_HEIGHT - ROOM_MIN_HEIGHT) + ROOM_MIN_HEIGHT;
      size.c = rand() % (ROOM_MAX_WIDTH - ROOM_MIN_WIDTH) + ROOM_MIN_WIDTH;

      // check if room is valid
      for (r = 0; r < size.r; r++) {
        for (c = 0; c < size.c; c++) {
          // if invalid, try a different spot by breaking out of both loops
          if (dungeon->tiles[corner.r + r][corner.c + c].sprite != ' ') {
            invalid = 1;
            break;
          }
        }
        if (invalid) {
          break;
        }
      }

      // retry logic
      if (invalid) {
        tries++;
        invalid = 0;
        continue;
      } else {
        break;
      }
    }

    // tried and failed to place all the rooms
    if (tries >= 5000) {
      printf("failed to place room. too many tries\n");
      return -1;
    }

    room->corner = corner;
    room->size = size;

    if ((err = place_room(dungeon, room))) {
      printf("failed to place room\n");
      return err;
    }
  }

  return 0;
}

/*
Places a rectangular room in the dungeon
*/
int place_room(dungeon *dungeon, room *room) {
  int r, c;
  int err; // shouldn't ever error, here just in case

  // place room
  for (r = 0; r < room->size.r; r++) {
    for (c = 0; c < room->size.c; c++) {
      dungeon->tiles[room->corner.r + r][room->corner.c + c].sprite = '.';
      dungeon->tiles[room->corner.r + r][room->corner.c + c].hardness = 0;
    }
  }

  return 0;
}

/*
Generates corridors between rooms

Function uses dijkstra to find the shortest path between rooms, with slight
randomization to add randomness to hallways.

Returns 0 on success, non-zero on failure.
*/
int generate_corridors(dungeon *dungeon, room *rooms) {
  int i, err;

  // find centers of rooms for pathfinding - could potentially change to random
  // points
  point *centers = (point *)(malloc(sizeof(*centers) * dungeon->num_rooms));
  for (i = 0; i < dungeon->num_rooms; i++) {
    centers[i] = (point){(uint8_t)(rooms[i].corner.r + rooms[i].size.r / 2),
                         (uint8_t)(rooms[i].corner.c + rooms[i].size.c / 2)};
  }

  // dijkstra to connect each room
  for (i = 0; i < dungeon->num_rooms - 1; i++) {
    find_path(dungeon, centers[i], centers[(i + 1)], 0);
  }

  // create cycles
  int r1, r2, cycles;
  cycles = rand() % 3 + 1; // completely arbitrary - mostly for fun
  for (i = 0; i < cycles; i++) {
    do {
      r1 = rand() % dungeon->num_rooms;
      r2 = rand() % dungeon->num_rooms;
    } while (r1 == r2);
    find_path(dungeon, centers[r1], centers[r2], 1);
  }

  free(centers);
  return 0;
}

/*
Helper method to find the shortest path between two rooms.
longest is flag to invert dijkstra for longer path.

Returns 0 on success, non-zero on failure.
*/
int find_path(dungeon *dungeon, point source, point target, int longest) {
  int r, c, err;
  int distances[DUNGEON_HEIGHT][DUNGEON_WIDTH];
  point predecessors[DUNGEON_HEIGHT][DUNGEON_WIDTH];

  // init dijkstra
  for (r = 0; r < DUNGEON_HEIGHT; r++) {
    for (c = 0; c < DUNGEON_WIDTH; c++) {
      distances[r][c] = INF;
      predecessors[r][c] = (point){255, 255};
    }
  }

  corridor_heap h;
  corridor_heap_init(&h);
  corridor_heap_push(&h, source, 0);
  distances[source.r][source.c] = 0;

  while (!corridor_heap_is_empty(&h)) {
    point u;
    int w;
    corridor_heap_pop(&h, &u, &w);

    // process neighbors (up, down, left, right)
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    for (int dir = 0; dir < 4; dir++) {
      uint8_t nr = u.r + dr[dir];
      uint8_t nc = u.c + dc[dir];

      // check bounds
      if (nr < 1 || nr > DUNGEON_HEIGHT - 2 || nc < 1 ||
          nc > DUNGEON_WIDTH - 2) {
        continue;
      }

      // calculate new distance
      int new_dist;

      // 270 kind of magic number; should technically be max hardness to invert
      // the values i.e previous hardest now becomes the lightest if we want
      // longer path randomness on weights for more variety in paths
      if (longest) {
        new_dist = w + 270 - dungeon->tiles[nr][nc].hardness + (rand() % 15);
      } else {
        new_dist = w + dungeon->tiles[nr][nc].hardness + (rand() % 15);
      }

      // update if new distance is "better"
      if (new_dist < distances[nr][nc]) {
        distances[nr][nc] = new_dist;
        predecessors[nr][nc] = u;
        corridor_heap_push(&h, (point){nr, nc}, new_dist);
      }
    }
  }

  // draw corridor by backtracking through predecessors
  point current = target;
  while ((current.r != source.r || current.c != source.c)) {
    if (dungeon->tiles[current.r][current.c].sprite == ' ') {
      dungeon->tiles[current.r][current.c].sprite = '#';
      // add randomness to hallway path variety
      // dungeon->tiles[current.r][current.c].hardness = (rand() % 21) + 4;
      dungeon->tiles[current.r][current.c].hardness = 0; // for loader
    }
    current = predecessors[current.r][current.c];
  }

  corridor_heap_destroy(&h);
  return 0;
}

/*
Places at least 2 stairs in any room or corridor tile

Returns 0 on success, non-zero otherwise.
*/
int place_stairs(dungeon *dungeon, int num_stairs) {
  int i;
  uint8_t r, c;

  if (num_stairs < 2) {
    return 1;
  }

  dungeon->num_stairs = num_stairs;
  dungeon->stairs = (stair *)(malloc(sizeof(*dungeon->stairs) * num_stairs));

  char stairs[2] = {'<', '>'};

  // place at least one of each stair
  for (i = 0; i < 2; i++) {
    do {
      r = rand() % DUNGEON_HEIGHT;
      c = rand() % DUNGEON_WIDTH;
    } while (dungeon->tiles[r][c].sprite != '.' &&
             dungeon->tiles[r][c].sprite != '#');

    dungeon->tiles[r][c].sprite = stairs[i];
    stair s = {.p = {r, c}, .type = i, .d = UNLINKED};
    dungeon->stairs[i] = s;
  }

  // place additional stairs
  for (; i < num_stairs; i++) {
    int type = rand() % 2;

    // pick random room or corridor tile
    do {
      r = rand() % DUNGEON_HEIGHT;
      c = rand() % DUNGEON_WIDTH;
    } while (dungeon->tiles[r][c].sprite != '.' &&
             dungeon->tiles[r][c].sprite != '#');

    stair s = {.p = {r, c}, .type = type, .d = UNLINKED};
    dungeon->stairs[i] = s;
    dungeon->tiles[r][c].sprite = stairs[type];
  }

  return 0;
}

int spawn_player(dungeon *dungeon) {
  uint8_t r, c;
  player *p = (player *)(malloc(sizeof(player)));

  // pick random room
  do {
    r = rand() % DUNGEON_HEIGHT;
    c = rand() % DUNGEON_WIDTH;
  } while (dungeon->tiles[r][c].sprite != '.');

  dungeon->player_pos.r = r;
  dungeon->player_pos.c = c;

  create_player(p, (point){r, c});

  dungeon->character_map[r][c] = p;

  // init vision
  update_player_vision(dungeon, p);

  return 0;
}

int spawn_monsters(dungeon *dungeon, int n) {
  int i;
  point p;
  for (i = 1; i <= n; i++) {
    monster *mob = (monster *)(malloc(sizeof(monster)));

    // generate random traits
    create_monster(mob, i);

    // pick random spot
    int wall_spawn = C_IS(mob, TUNNELING) && C_IS(mob, TELEPATHIC) ? 1 : 0;
    do {
      p.r = rand() % DUNGEON_HEIGHT;
      p.c = rand() % DUNGEON_WIDTH;
    } while (!IN_BOUNDS(p.r, p.c) || dungeon->tiles[p.r][p.c].sprite != '.' &&
                                         dungeon->character_map[p.r][p.c]);

    mob->pos = p;

    dungeon->character_map[p.r][p.c] = mob;
  }

  return 0;
}

int init_heap(dungeon *d) {
  // init event queue
  heap_init(&d->events, sizeof(event), compare_events);
  for (int r = 0; r < DUNGEON_HEIGHT; r++) {
    for (int c = 0; c < DUNGEON_WIDTH; c++) {
      if (d->character_map[r][c]) {
        event *e = (event *)(malloc(sizeof(event)));
        e->character = d->character_map[r][c];
        e->turn_time = 0;

        heap_push(&d->events, e);
      }
    }
  }

  return 0;
}
