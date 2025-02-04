#include <stdlib.h>
#include <stdio.h>

#include "gen/gen.h"
#include "dsa/corridor_heap.h"

void print_dungeon(dungeon *dungeon);
void print_hardness(dungeon *dungeon);
void test_heap();

int main(int argc, char **argv) {
    // create a dungeon
    dungeon *dungeon = malloc(sizeof (*dungeon));

    // at least 6 rooms
    int num_rooms = 6 + (rand() % 6);

    generate_dungeon(dungeon, num_rooms);

    print_dungeon(dungeon);
    // print_hardness(dungeon);

    return 0;
}

void print_dungeon(dungeon *dungeon) {
    int r, c;
    for (r = 0; r < DUNGEON_HEIGHT; r++) {
        for (c = 0; c < DUNGEON_WIDTH; c++) {
            if (!dungeon->tiles[r][c].sprite) {
                printf(" ");
                continue;
            }
            printf("%c", dungeon->tiles[r][c].sprite);
        }
        printf("\n");
    }
}

void print_hardness(dungeon *dungeon) {
    int r, c;
    for (r = 0; r < DUNGEON_HEIGHT; r++) {
        for (c = 0; c < DUNGEON_WIDTH; c++) {
            if (r == 0 || r == DUNGEON_HEIGHT-1 || c == 0 || c == DUNGEON_WIDTH-1) {
                printf("%c", dungeon->tiles[r][c].sprite);
            } else if (dungeon->tiles[r][c].hardness != DEFAULT_HARDNESS) {
                printf("%3d", dungeon->tiles[r][c].hardness);
            } else {
                printf("   ");
            }
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