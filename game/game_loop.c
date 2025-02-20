#include <stdlib.h>

#include "game_loop.h"
#include "../gen/gen.h"

// comparator for event struct
static int compare_events(const void *v1, const void *v2) {
    event event1 = *(event *)v1;
    event event2 = *(event *)v2;

    return event1.turn_time - event2.turn_time;
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

    generate_dungeon(&g->maps[0], 6);

    return 0;
}
