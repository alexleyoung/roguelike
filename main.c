#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "dsa/heap.h"
#include "dsa/corridor_heap.h"
#include "types/dungeon.h"
#include "gen/gen.h"
#include "saves/saves.h"
#include "monsters/pathfinding.h"

void print_dungeon(dungeon *dungeon);
void print_hardness(dungeon *dungeon);
void print_dists(dungeon *dungeon, int dists[DUNGEON_HEIGHT][DUNGEON_WIDTH]);
void test_corridor_heap();
void test_heap();
int compare_dungeons(dungeon *d1, dungeon *d2);
void test_save_load();

int main(int argc, char **argv) {
    dungeon *dungeon = malloc(sizeof (*dungeon));
    int err = 0;

    switch (argc) {
        case 2: // load dungeon from file
            // check arg
            if (strcmp(argv[1], "--save") == 0) {
                if ((err = generate_dungeon(dungeon, 6 + (rand() % 6)))) {
                    printf("Error: Failed to generate dungeon\n");
                }; 
                if ((err = save_dungeon(dungeon, "dungeon"))) {
                    printf("Error: Failed to save dungeon\n");
                }; // default name for assignment 
            } else if (strcmp(argv[1], "--load") == 0) {
                if ((err = load_dungeon(dungeon, "dungeon"))) {
                    printf("Error: Failed to load dungeon\n");
                }
            }
            break;
        case 3: // load and save dungeon to file
            if ((strcmp(argv[1], "--load") == 0 && strcmp(argv[2], "--save") == 0) ||
                (strcmp(argv[1], "--save") == 0 && strcmp(argv[2], "--load") == 0)) {
                if ((err = load_dungeon(dungeon, "dungeon"))) {
                    printf("Error: Failed to load dungeon\n");
                };
                if ((err = save_dungeon(dungeon, "dungeon"))) {
                    printf("Error: Failed to save dungeon\n");
                }; 
            } else if (strcmp(argv[1], "--load") == 0) {
                // load custom save
                if ((err = load_dungeon(dungeon, argv[2]))) {
                    printf("Error: Failed to load dungeon\n");
                };
            }
            break;
        default: // no args (1 arg, case not strictly necessary)
            if ((err = generate_dungeon(dungeon, 6 + (rand() % 6)))) {
                printf("Error: Failed to generate dungeon\n");
            }; 
            break;
    }

    if (err) { return err; }
    print_dungeon(dungeon);

    printf("Distances:\n");
    calc_dists(dungeon, dungeon->dists, dungeon->player, 0);
    print_dists(dungeon, dungeon->dists);

    printf("Distances with Tunnels:\n");
    calc_dists(dungeon, dungeon->tunnel_dists, dungeon->player, 1);
    print_dists(dungeon, dungeon->tunnel_dists);

    printf("hardness:\n");
    print_hardness(dungeon);

    return 0;
}

void print_dungeon(dungeon *dungeon) {
    int r, c;
    for (r = 0; r < DUNGEON_HEIGHT; r++) {
        for (c = 0; c < DUNGEON_WIDTH; c++) {
            if (!dungeon->tiles[r][c].sprite) {
                printf(" ");
                continue; }
            printf("%c", dungeon->tiles[r][c].sprite);
        }
        printf("\n");
    }
}

void print_hardness(dungeon *dungeon) {
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

void print_dists(dungeon *dungeon, int dists[DUNGEON_HEIGHT][DUNGEON_WIDTH]) {
    for (int r = 0; r < DUNGEON_HEIGHT; r++) {
        for (int c = 0; c < DUNGEON_WIDTH; c++) {
            if (dists[r][c] > 255) {
                printf(" ");
                continue;
            } else if (r == dungeon->player.r && c == dungeon->player.c) {
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

    heap_init(&h, sizeof (int), compare_int);
    v = 5;
    if (heap_push(&h, &v)) printf("error");
    v = 4;
    heap_push(&h, &v);
    v = 3;
    heap_push(&h, &v);
    v = 2;
    heap_push(&h, &v);
    v = 1;
    heap_push(&h, &v);

    while (!heap_is_empty(&h)) {
        heap_pop(&h, &v);
        printf("%d\n", v);
    }
}

int compare_dungeons(dungeon *d1, dungeon *d2) {
    if (d1->num_rooms != d2->num_rooms) return 0;
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
    dungeon dungeon1, dungeon2;
    generate_dungeon(&dungeon1, 6);
    print_dungeon(&dungeon1);
    
    const char *test_file = "dungeon";
    save_dungeon(&dungeon1, test_file);
    int load_success = load_dungeon(&dungeon2, test_file);
    assert(load_success == 0);
    assert(compare_dungeons(&dungeon1, &dungeon2) == 0);
    
    print_dungeon(&dungeon2);
    printf("Test passed: Dungeon saved and loaded successfully!\n");
}
