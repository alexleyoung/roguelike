#include "character.hpp"
#include "movement.hpp"
#include "ui.hpp"
#include <game_loop.hpp>
#include <ncurses.h>

// create additional maps within the game with correct IDs
// for eventual stair continuity (hopefully)
static int add_dungeon(game *g, Player *p, int link_id, int stair_type) {
  Dungeon *tmp =
      (Dungeon *)(realloc(g->maps, sizeof(*g->maps) * ++g->num_maps));

  if (!tmp) {
    return -1;
  }
  g->maps = tmp;

  generate_linked_dungeon(
      &g->maps[g->num_maps - 1], p, DEFAULT_ROOM_COUNT + rand() % 4,
      DEFAULT_MOB_COUNT + rand() % 11, DEFAULT_OBJECT_COUNT + rand() % 5,
      link_id, stair_type);
  g->maps[g->num_maps - 1].id = g->num_maps - 1;

  return 0;
}

// init game struct
int init_game(game *g) {
  g->maps = (Dungeon *)(malloc(sizeof(*g->maps)));
  g->num_maps = 1;
  g->current_map = 0;

  generate_dungeon(&g->maps[g->current_map], DEFAULT_ROOM_COUNT + rand() % 4,
                   DEFAULT_MOB_COUNT + rand() % 11,
                   DEFAULT_OBJECT_COUNT + rand() % 5);
  g->maps[g->current_map].id = 0;

  return 0;
}

int start_game(game *g) {
  // init ncurses display and settings
  ui_init();

  Event e;
  int input;
  bool fog = true;
  while (!heap_is_empty(&g->maps[g->current_map].events)) {
    Dungeon map = g->maps[g->current_map];
    heap_pop(&g->maps[g->current_map].events, &e);

    Player *p =
        e.character->type == PLAYER ? static_cast<Player *>(e.character) : NULL;
    Monster *m = e.character->type == MONSTER
                     ? static_cast<Monster *>(e.character)
                     : NULL;

    if (!e.character->alive) {
      if (p) {
        clear();
        refresh();
        printf("game over. pc died!\n");
        delete p;
        return 0;
      }

      if (C_IS(m, BOSS)) {
        clear();
        refresh();
        printf("you win!\n");
        delete m;
        return 0;
      }

      delete m;
      continue;
    }

    // print on player turn
    if (p) { // TODO: add check for ai player flag prob
      if (fog) {
        draw_player_dungeon(&g->maps[g->current_map], p);
      } else {
        draw_dungeon(&g->maps[g->current_map]);
      }

      input = getch();
      int res = move_player(&g->maps[g->current_map], p, input);

      if (res == PLAYER_MOVE_QUIT) { // quit game
        return 0;
      }

      if (res == PLAYER_MOVE_ACTION) {
        heap_push(&g->maps[g->current_map].events, &e);
        continue;
      }

      if (res == PLAYER_MOVE_MENU) { // menud, redo turn
        heap_push(&g->maps[g->current_map].events, &e);
        continue;
      }

      if (res == PLAYER_MOVE_STAIR) {
        // find stair
        Stair *s;
        for (int i = 0; i < g->maps[g->current_map].num_stairs; i++) {
          if (e.character->pos.r == g->maps[g->current_map].stairs[i].p.r &&
              e.character->pos.c == g->maps[g->current_map].stairs[i].p.c) {
            s = &g->maps[g->current_map].stairs[i];
          }
        }

        // re-add player turn to this Dungeon's Event queue
        e.turn_time += 1000 / e.character->speed;
        heap_push(&g->maps[g->current_map].events, &e);

        // go to new Dungeon or make one if necessary
        if (s->d != UNLINKED) {
          g->current_map = s->d;
          p->pos = g->maps[g->current_map].player_pos;
        } else {
          // create Dungeon linked to the stair
          add_dungeon(g, p, g->current_map,
                      s->type == UP_STAIR ? DOWN_STAIR : UP_STAIR);
          g->current_map = g->num_maps - 1;
          s->d = g->current_map; // link original stair to new map
        }
        clear();
        draw_message("Going to floor: %d", s->d);
        continue;
      }

      if (res == PLAYER_TOGGLE_FOG) {
        fog = !fog;
        draw_message("Toggled Fog of War");
        heap_push(&g->maps[g->current_map].events, &e);
        continue;
      }

      if (res == PLAYER_MOVE_INVALID) {
        heap_push(&g->maps[g->current_map].events, &e);
        continue;
      }

      draw_message("");
    }

    else {
      // move npc character according to their traits
      move_character(&g->maps[g->current_map], e.character);
    }

    // add character back to Event queue
    e.turn_time += 1000 / e.character->speed;
    heap_push(&g->maps[g->current_map].events, &e);
  }

  // close ncurses win
  endwin();

  return 0;
}
