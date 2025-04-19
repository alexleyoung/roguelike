#include <character.hpp>
#include <dungeon.hpp>
#include <movement.hpp>
#include <ui.hpp>

int interact(Dungeon *d, Player *p, int move) {
  //// actions
  // drop item
  switch (move) {
  case ',':
    if (!d->object_map[p->pos.r][p->pos.c]) {
      draw_message("No item to pickup!");
      return PLAYER_MOVE_INVALID;
    }

    // try to find open carry slot
    int i;
    for (i = 0; p->carry[i]; i++) {
    }

    // Check if carry inventory full
    if (i == NUM_CARRY_SLOTS) {
      draw_message("Can't carry anymore items!");
      return PLAYER_MOVE_INVALID;
    }

    // Put item in carry slot and remove from map
    draw_message("Picked up %s",
                 d->object_map[p->pos.r][p->pos.c]->name.c_str());
    p->carry[i] = d->object_map[p->pos.r][p->pos.c];
    d->object_map[p->pos.r][p->pos.c] = NULL;

    return PLAYER_MOVE_ACTION;
  case 'd':
    draw_message("drop item");
    return PLAYER_MOVE_ACTION;
  // take off item
  case 't':
    draw_message("take off item");
    return PLAYER_MOVE_ACTION;
  // wear item
  case 'w':
    draw_message("wear item");
    return PLAYER_MOVE_ACTION;
  // expunge item
  case 'x':
    draw_message("expunge item");
    return PLAYER_MOVE_ACTION;
  // display equipment
  case 'e':
    draw_message("display equipment");
    return PLAYER_MOVE_MENU;
  // display inv
  case 'i':
    draw_message("display inventory");
    return PLAYER_MOVE_MENU;
  // inspect item
  case 'E':
    draw_message("inspect item");
    return PLAYER_MOVE_MENU;
  // Player info
  case 'c':
    draw_message("Character info");
    return PLAYER_MOVE_MENU;
  // Monster list
  case 'm':
    draw_monster_list(d, p);
    return PLAYER_MOVE_MENU;

  //// debug
  // toggle fow
  case 'f':
    return PLAYER_TOGGLE_FOG;
    break;
  // default terrain map
  case 's':
    draw_message("terrain map");
    return PLAYER_MOVE_MENU;
  // hardness map
  case 'H':
    draw_message("hardness map");
    return PLAYER_MOVE_MENU;
  // non-tunneling dist map
  case 'D':
    draw_message("non-tunneling dist map");
    return PLAYER_MOVE_MENU;
  // tunneling dist map
  case 'T':
    draw_message("tunneling dist map");
    return PLAYER_MOVE_MENU;
  // look Monster
  case 'L':
    draw_message("look Monster");
    return PLAYER_MOVE_MENU;

  //// quit game
  case 'Q':
    draw_message("Quitting game...");
    refresh();
    return PLAYER_MOVE_QUIT;

  default:
    draw_message("Invalid key press: %c", move);
    return PLAYER_MOVE_INVALID;
  }

  return PLAYER_MOVE_INVALID;
}
