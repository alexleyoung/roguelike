#include "dungeon.h"
#include <game_loop.h>

// create additional maps within the game with correct IDs
// for eventual stair continuity (hopefully)
static int add_dungeon(game *g, int link_id, int stair_type) {
  dungeon *tmp = realloc(g->maps, sizeof(*g->maps) * ++g->num_maps);

  if (!tmp) {
    return -1;
  }
  g->maps = tmp;

  generate_linked_dungeon(&g->maps[g->num_maps - 1], DEFAULT_ROOM_COUNT,
                          DEFAULT_MOB_COUNT, link_id, stair_type);
  g->maps[g->num_maps - 1].id = g->num_maps - 1;

  return 0;
}

// init game struct
int init_game(game *g) {
  g->maps = malloc(sizeof(*g->maps));
  g->num_maps = 1;
  g->current_map = 0;

  generate_dungeon(&g->maps[g->current_map], DEFAULT_ROOM_COUNT,
                   DEFAULT_MOB_COUNT);

  return 0;
}

int start_game(game *g) {
  // init ncurses display and settings
  initscr();
  clear();
  cbreak();
  noecho();
  curs_set(0);
  keypad(stdscr, TRUE);

  event e;
  int input;
  while (!heap_is_empty(&g->maps[g->current_map].events)) {
    dungeon map = g->maps[g->current_map];
    heap_pop(&g->maps[g->current_map].events, &e);

    if (!e.character->alive) {
      if (e.character->traits == PLAYER_TRAIT) {
        printf("game over. pc died!\n");
        return 0;
      }

      free(e.character);
      continue;
    }

    // print on player turn
    if (e.character->traits ==
        PLAYER_TRAIT) { // TODO: add check for ai player flag prob
      draw_dungeon(&g->maps[g->current_map]);

      input = getch();
      int res = move_player(&g->maps[g->current_map], e.character, input);

      if (res == PLAYER_MOVE_QUIT) { // quit game
        return 0;
      }

      if (res == PLAYER_MOVE_MENU) { // menud, redo turn
        heap_push(&g->maps[g->current_map].events, &e);
        continue;
      }

      if (res == PLAYER_MOVE_STAIR) {
        // find stair
        stair *s;
        for (int i = 0; i < g->maps[g->current_map].num_stairs; i++) {
          if (e.character->pos.r == g->maps[g->current_map].stairs[i].p.r &&
              e.character->pos.c == g->maps[g->current_map].stairs[i].p.c) {
            s = &g->maps[g->current_map].stairs[i];
          }
        }

        if (s->d >= 0) { // stair has linked room
          e.turn_time += 1000 / e.character->speed;
          heap_push(&g->maps[g->current_map].events, &e);
          g->current_map = s->d;
          continue;
        } else { // generate new room
          add_dungeon(g, g->current_map,
                      s->type == UP_STAIR ? DOWN_STAIR : UP_STAIR);
          e.turn_time += 1000 / e.character->speed;
          heap_push(&g->maps[g->current_map].events, &e);
          g->current_map = g->num_maps - 1;
          s->d = g->current_map; // link original stair to new map
          continue;
        }
      }

    }

    else {
      // move npc character according to their traits
      move_character(&g->maps[g->current_map], e.character);
    }

    // add character back to event queue
    e.turn_time += 1000 / e.character->speed;
    heap_push(&g->maps[g->current_map].events, &e);
  }

  // close ncurses win
  endwin();

  return 0;
}
