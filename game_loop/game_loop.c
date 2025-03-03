#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "game_loop.h"
#include "../gen/gen.h"
#include "../dsa/heap.h"
#include "../characters/movement.h"

void print_dungeon(dungeon *dungeon);

// comparator for event struct
static int compare_events(const void *v1, const void *v2) {
    event event1 = *(event *)v1;
    event event2 = *(event *)v2;

    int diff;

    diff = event1.turn_time - event2.turn_time;
    if (diff) {
        return diff;
    } else {
        return event1.character->id - event2.character->id;
    }
}

// create additional maps within the game with correct IDs
// for eventual stair continuity (hopefully)
static int add_dungeon(game *g) {
    g->num_maps++;
    dungeon *tmp = realloc(g->maps, sizeof (*g->maps) * g->num_maps);

    if (!tmp) {
        return -1;
    }
    g->maps = tmp;

    generate_dungeon(&g->maps[g->num_maps - 1], (rand() % 6) + 3);
    g->maps[g->num_maps - 1].id = g->num_maps - 1;

    return 0;
}

// init game struct
int init_game(game *g) {
    g->maps = malloc(sizeof (*g->maps));
    heap_init(&g->events, sizeof (event), compare_events);
    g->num_maps = 1;
    g->current_map = 0;

    generate_dungeon(&g->maps[0], 6);

    // init event queue
    for (int r = 0; r < DUNGEON_HEIGHT; r++) {
        for (int c = 0; c < DUNGEON_WIDTH; c++) {
            if (g->maps[g->current_map].character_map[r][c]) {
                event e;
                e.character = g->maps[g->current_map].character_map[r][c];
                e.turn_time = 0;

                heap_push(&g->events, &e);
            }
        }
    }

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
    while (!heap_is_empty(&g->events)) {
        event e;
        heap_pop(&g->events, &e);

        // if character is killed,
        if (!e.character) {
            continue;
        }

        // print on character turn
        if (e.character->id == 0) {
            /*print_dungeon(&g->maps[g->current_map]);*/
        }
            print_dungeon(&g->maps[g->current_map]);

        // move character according to their traits
        if (move(&g->maps[g->current_map], e.character)) {
            printf("pc died!\n");
            return 0;
        };

        // add character back to event queue
        e.turn_time += e.character->speed;
        heap_push(&g->events, &e);
        usleep(250000);
    }

    return 0;
}
