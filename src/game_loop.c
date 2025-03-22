#include <game_loop.h>

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
  g->num_maps = 1;
  g->current_map = 0;

  generate_dungeon(&g->maps[g->current_map], DEFAULT_ROOM_COUNT,
                   DEFAULT_MOB_COUNT);
  heap_init(&g->maps[g->current_map].events, sizeof(event), compare_events);

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

  // init event queue
  for (int r = 0; r < DUNGEON_HEIGHT; r++) {
    for (int c = 0; c < DUNGEON_WIDTH; c++) {
      if (g->maps[g->current_map].character_map[r][c]) {
        event e;
        e.character = g->maps[g->current_map].character_map[r][c];
        e.turn_time = 0;

        heap_push(&g->maps[g->current_map].events, &e);
      }
    }
  }

  event e;
  int input;
  while (!heap_is_empty(&g->maps[g->current_map].events)) {
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
    } else {
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
