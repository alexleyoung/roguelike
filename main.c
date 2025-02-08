#include <stdlib.h>
#include <stdio.h>
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
    // // create a dungeon
    dungeon *dungeon = malloc(sizeof (*dungeon));

    // int num_rooms = 6 + (rand() % 6);
    // generate_dungeon(dungeon, num_rooms);

    // print_dungeon(dungeon);
    // print_hardness(dungeon);

    test_save_load();

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
            if (d1->tiles[i][j].hardness != d2->tiles[i][j].hardness) return 1;
        }
    }
    return 0;
}

void test_save_load() {
    dungeon dungeon1, dungeon2;
    generate_dungeon(&dungeon1, 6);
    print_hardness(&dungeon1);
    
    const char *test_file = "test_dungeon.rlg";
    save_dungeon(&dungeon1, test_file);
    
    int load_success = load_dungeon(&dungeon2, test_file);
    assert(load_success == 0);
    
    assert(compare_dungeons(&dungeon1, &dungeon2));
    printf("Test passed: Dungeon saved and loaded successfully!\n");
}