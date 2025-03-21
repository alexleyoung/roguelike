#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <game_loop.h>
#include <gen.h>
#include <heap.h>
#include <movement.h>

#define DEFAULT_MOB_COUNT 10

void print_dungeon(dungeon *dungeon);

// comparator for event struct
/*static int compare_events(const void *v1, const void *v2) {*/
/*    event event1 = *(event *)v1;*/
/*    event event2 = *(event *)v2;*/
/**/
/*    int diff;*/
/**/
/*    diff = event1.turn_time - event2.turn_time;*/
/*    if (diff) {*/
/*        return diff;*/
/*    } else {*/
/*        return event1.character->id - event2.character->id;*/
/*    }*/
/*}*/
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

// create additional maps within the game with correct IDs
// for eventual stair continuity (hopefully)
static int add_dungeon(game *g) {
  g->num_maps++;
  dungeon *tmp = realloc(g->maps, sizeof(*g->maps) * g->num_maps);

  if (!tmp) {
    return -1;
  }
  g->maps = tmp;

  generate_dungeon(&g->maps[g->num_maps - 1], (rand() % 6) + 3,
                   DEFAULT_MOB_COUNT);
  g->maps[g->num_maps - 1].id = g->num_maps - 1;

  return 0;
}

// init game struct
int init_game(game *g) {
  g->maps = malloc(sizeof(*g->maps));
  heap_init(&g->events, sizeof(event), compare_events);
  /*j_heap_init(&g->events, compare_events, NULL);*/
  g->num_maps = 1;
  g->current_map = 0;

  generate_dungeon(&g->maps[g->current_map], 6, DEFAULT_MOB_COUNT);

  return 0;
}

void print_dungeon(dungeon *dungeon) {
  int r, c;
  for (r = 0; r < DUNGEON_HEIGHT; r++) {
    for (c = 0; c < DUNGEON_WIDTH; c++) {
      if (dungeon->character_map[r][c]) {
        printf("%c", dungeon->character_map[r][c]->sprite);
      } else {
        printf("%c", dungeon->tiles[r][c].sprite);
      }
    }
    printf("\n");
  }
}

int start_game(game *g) {
  // init event queue
  for (int r = 0; r < DUNGEON_HEIGHT; r++) {
    for (int c = 0; c < DUNGEON_WIDTH; c++) {
      if (g->maps[g->current_map].character_map[r][c]) {
        event e;
        e.character = g->maps[g->current_map].character_map[r][c];
        e.turn_time = 0;

        heap_push(&g->events, &e);
        /*heap_insert(&g->events, &e);*/
      }
    }
  }

  while (!heap_is_empty(&g->events)) {
    event e;
    heap_pop(&g->events, &e);

    if (!e.character->alive) {
      g->maps[g->current_map]
          .character_map[e.character->pos.r][e.character->pos.c] = NULL;
      free(e.character);
      continue;
    }

    // print on character turn
    if (e.character->id == 0) {
      print_dungeon(&g->maps[g->current_map]);
    }

    // move character according to their traits
    int res = move(&g->maps[g->current_map], e.character);
    if (res == 1) {
      print_dungeon(&g->maps[g->current_map]);
      printf("game over. pc died!\n");
      return 0;
    } else if (res == 2) {
      // dont add to queue if monster is killed
      print_dungeon(&g->maps[g->current_map]);
      continue;
    };

    // add character back to event queue
    e.turn_time += 1000 / e.character->speed;
    heap_push(&g->events, &e);
    usleep(250000);
  }

  return 0;
}
