#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <corridor_heap.hpp>
#include <descriptions.hpp>
#include <dungeon.hpp>
#include <game.hpp>
#include <game_loop.hpp>
#include <gen.hpp>
#include <heap.hpp>
#include <ncurses.h>
#include <saves.hpp>
#include <ui.hpp>

void print_hardness(Dungeon *dungeon);
void print_dists(Dungeon *dungeon, int dists[DUNGEON_HEIGHT][DUNGEON_WIDTH]);
void test_corridor_heap();
void test_heap();
int compare_dungeons(Dungeon *d1, Dungeon *d2);
void test_save_load();

int main(int argc, char **argv) {
  game g;
  init_game(&g);
  /*int err = 0;*/
  /**/
  /*switch (argc) {*/
  /*case 2: // load dungeon from file*/
  /*  // check arg*/
  /*  if (strcmp(argv[1], "--save") == 0) {*/
  /*    if ((err = save_dungeon(&g.maps[g.current_map], "dungeon"))) {*/
  /*      printf("Error: Failed to save dungeon\n");*/
  /*    }; // default name for assignment*/
  /*    g.maps[0].id = 0;*/
  /*  } else if (strcmp(argv[1], "--load") == 0) {*/
  /*    if ((err = load_dungeon(&g.maps[g.current_map], "dungeon"))) {*/
  /*      printf("Error: Failed to load dungeon\n");*/
  /*    }*/
  /*  }*/
  /*  break;*/
  /*case 3: // load and save dungeon to file*/
  /*  if ((strcmp(argv[1], "--load") == 0 && strcmp(argv[2], "--save") == 0)
   * ||*/
  /*      (strcmp(argv[1], "--save") == 0 && strcmp(argv[2], "--load") == 0))
   * {*/
  /*    if ((err = load_dungeon(&g.maps[0], "dungeon"))) {*/
  /*      printf("Error: Failed to load dungeon\n");*/
  /*    };*/
  /*    g.maps[0].id = 0;*/
  /*    if ((err = save_dungeon(&g.maps[0], "dungeon"))) {*/
  /*      printf("Error: Failed to save dungeon\n");*/
  /*    };*/
  /*  } else if (strcmp(argv[1], "--load") == 0) {*/
  /*    // load custom save*/
  /*    if ((err = load_dungeon(&g.maps[0], argv[2]))) {*/
  /*      printf("Error: Failed to load dungeon\n");*/
  /*    };*/
  /*  } else if (strcmp(argv[1], "--nummon") == 0) {*/
  /*    generate_dungeon(&g.maps[g.current_map], 6, atoi(argv[2]));*/
  /*  }*/
  /*  break;*/
  /*}*/
  /**/
  /*if (err) {*/
  /*  return err;*/
  /*}*/

  // test pathfinding outputs
  /*dungeon *dungeon = &g.maps[g.current_map];*/
  /*printf("Dungeon:\n");*/
  /*print_dungeon(dungeon);*/
  /**/
  /*printf("Distances:\n");*/
  /*calc_dists(dungeon, dungeon->dists, dungeon->player_pos, 0);*/
  /*print_dists(dungeon, dungeon->dists);*/
  /**/
  /*printf("Distances with Tunnels:\n");*/
  /*calc_dists(dungeon, dungeon->tunnel_dists, dungeon->player_pos, 1);*/
  /*print_dists(dungeon, dungeon->tunnel_dists);*/
  /**/
  /*printf("hardness:\n");*/
  /*print_hardness(dungeon);*/

  start_game(&g);

  /*std::vector<Monster_Description> vec =*/
  /*    load_monster_descriptions("monster_desc.txt");*/
  /*for (Monster_Description md : vec) {*/
  /*  md.print_info();*/
  /*  std::cout << std::endl;*/
  /*}*/
  /*load_object_descriptions("object_desc.txt");*/

  return 0;
}

/*void print_dungeon(dungeon *dungeon) {*/
/*    int r, c;*/
/*    for (r = 0; r < DUNGEON_HEIGHT; r++) {*/
/*        for (c = 0; c < DUNGEON_WIDTH; c++) {*/
/*            if (dungeon->character_map[r][c]) {*/
/*                printf("%c", dungeon->character_map[r][c]->sprite);*/
/*            } else {*/
/*                printf("%c", dungeon->tiles[r][c].sprite);*/
/*            }*/
/*        }*/
/*        printf("\n");*/
/*    }*/
/*}*/

void print_hardness(Dungeon *dungeon) {
  int r, c;
  for (r = 0; r < DUNGEON_HEIGHT + 1; r++) {
    for (c = 0; c < DUNGEON_WIDTH + 1; c++) {
      if (r == 0 && c == 0) {
        printf("   ");
      } else if (r == 0) {
        printf("%2d", c - 1);
      } else if (c == 0) {
        printf("%2d ", r - 1);
      } else {
        printf("%2x", dungeon->tiles[r - 1][c - 1].hardness);
      }
    }
    printf("\n");
  }
}

void print_dists(Dungeon *dungeon, int dists[DUNGEON_HEIGHT][DUNGEON_WIDTH]) {
  for (int r = 0; r < DUNGEON_HEIGHT; r++) {
    for (int c = 0; c < DUNGEON_WIDTH; c++) {
      if (dists[r][c] > 255) {
        printf(" ");
        continue;
      } else if (r == dungeon->player_pos.r && c == dungeon->player_pos.c) {
        printf("@");
      } else {
        printf("%d", dists[r][c] % 10);
      }
    }
    printf("\n");
  }
}

void test_corridor_heap() {
  corridor_heap h;
  corridor_heap_init(&h);
  corridor_heap_push(&h, (point){0, 0}, -3);
  corridor_heap_push(&h, (point){1, 1}, -2);
  corridor_heap_push(&h, (point){2, 2}, -1);
  corridor_heap_push(&h, (point){3, 3}, 0);

  point p;
  int w;
  while (!corridor_heap_is_empty(&h)) {
    corridor_heap_pop(&h, &p, &w);
    printf("%d\n", w);
  }
}

int compare_int(const void *v1, const void *v2) {
  return *((int *)v1) - *((int *)v2);
}

void test_heap() {
  int v;
  heap h;

  heap_init(&h, sizeof(int), compare_int);
  for (int i = 10; i >= 0; i--) {
    v = i;
    heap_push(&h, &v);
  }

  while (!heap_is_empty(&h)) {
    heap_pop(&h, &v);
    printf("%d\n", v);
  }
}

int compare_dungeons(Dungeon *d1, Dungeon *d2) {
  if (d1->num_rooms != d2->num_rooms)
    return 0;
  for (int i = 0; i < DUNGEON_HEIGHT; i++) {
    for (int j = 0; j < DUNGEON_WIDTH; j++) {
      if (d1->tiles[i][j].hardness != d2->tiles[i][j].hardness) {
        printf("%d ", d1->tiles[i][j].hardness);
        printf("%d ", d2->tiles[i][j].hardness);
        return 1;
      };
    }
  }
  return 0;
}

void test_save_load() {
  Dungeon dungeon1, dungeon2;
  generate_dungeon(&dungeon1, 6, 10, 10);
  draw_dungeon(&dungeon1);

  const char *test_file = "dungeon";
  save_dungeon(&dungeon1, test_file);
  int load_success = load_dungeon(&dungeon2, test_file);
  assert(load_success == 0);
  assert(compare_dungeons(&dungeon1, &dungeon2) == 0);

  draw_dungeon(&dungeon2);
  printf("Test passed: Dungeon saved and loaded successfully!\n");
}
