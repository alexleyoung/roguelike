#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "gen/gen.h"
#include "dsa/corridor_heap.h"
#include "saves/saves.h"

void print_dungeon(dungeon *dungeon);
void print_hardness(dungeon *dungeon);
void test_heap();
int compare_dungeons(dungeon *d1, dungeon *d2);
void test_save_load();

int main(int argc, char **argv) {
    dungeon *dungeon = malloc(sizeof (*dungeon));

    switch (argc) {
        case 2: // load dungeon from file
            // check arg
            if (strcmp(argv[1], "--save") == 0) {
                generate_dungeon(dungeon, 6 + (rand() % 6));
                save_dungeon(dungeon, "dungeon"); // default name for assignment
            } else if (strcmp(argv[1], "--load") == 0) {
                load_dungeon(dungeon, "dungeon");
            }
            break;
        case 3: // load and save dungeon to file
            if ((strcmp(argv[1], "--load") == 0 && strcmp(argv[2], "--save") == 0) ||
                (strcmp(argv[1], "--save") == 0 && strcmp(argv[2], "--load") == 0)) {
                load_dungeon(dungeon, "dungeon");
                save_dungeon(dungeon, "dungeon");
            } else if (strcmp(argv[1], "--load") == 0) {
                // load custom save
                load_dungeon(dungeon, argv[2]);
            }
            break;
        default: // no args (1 arg, case not strictly necessary)
            generate_dungeon(dungeon, 6 + (rand() % 6));
            break;
    }

    print_dungeon(dungeon);
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
    for (r = 0; r < DUNGEON_HEIGHT; r++) {
        for (c = 0; c < DUNGEON_WIDTH; c++) {
            printf("%3d", dungeon->tiles[r][c].hardness);
        }
        printf("\n");
    }
}

void test_heap() {
    heap h;
    heap_init(&h);
    heap_push(&h, (point){0, 0}, -3);
    heap_push(&h, (point){1, 1}, -2);
    heap_push(&h, (point){2, 2}, -1);
    heap_push(&h, (point){3, 3}, 0);

    point p;
    int w;
    while (!heap_is_empty(&h)) {
        heap_pop(&h, &p, &w);
        printf("%d\n", w);
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