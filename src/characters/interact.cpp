#include <ncurses.h>
#include <sstream>

#include <character.hpp>
#include <descriptions.hpp>
#include <dungeon.hpp>
#include <movement.hpp>
#include <object.hpp>
#include <ui.hpp>

#define SELECT_FAIL -1
static int select_item(Player *p);
static int select_equipment(Player *p);

static PLAYER_ACTION pickup_item(Dungeon *d, Player *p);
static PLAYER_ACTION drop_item(Dungeon *d, Player *p);
static PLAYER_ACTION take_off_item(Player *p);
static PLAYER_ACTION wear_item(Player *p);
static PLAYER_ACTION expunge_item(Player *p);
static PLAYER_ACTION inspect_item(Player *p);

int interact(Dungeon *d, Player *p, int move) {
  //// actions
  // drop item
  switch (move) {
  case ',':
    return pickup_item(d, p);
  case 'd':
    return drop_item(d, p);
  // take off item
  case 't':
    return take_off_item(p);
  // wear item
  case 'w':
    return wear_item(p);
  // expunge item
  case 'x':
    return expunge_item(p);
  // display equipment
  case 'e':
    draw_equipment(p);
    getch();
    return PLAYER_MOVE_MENU;
  // display inv
  case 'i':
    draw_inventory(p);
    getch();
    return PLAYER_MOVE_MENU;
  // inspect item
  case 'I':
    return inspect_item(p);
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

int select_item(Player *p) {
  // prompt player which item
  draw_inventory(p);
  draw_message("Press (0-9) to choose item");
  int selected = getch();
  if (selected < '0' || selected > '9')
    return SELECT_FAIL;
  return selected - '0';
}

int select_equipment(Player *p) {
  // prompt player which equipment
  draw_equipment(p);
  draw_message("Press (a-l) to take off item");
  int selected = getch();
  if (selected < 'a' || selected > 'l')
    return SELECT_FAIL;
  return selected - 'a';
}

PLAYER_ACTION pickup_item(Dungeon *d, Player *p) {
  if (!d->object_map[p->pos.r][p->pos.c]) {
    draw_message("No item to pickup!");
    return PLAYER_MOVE_INVALID;
  }

  // try to find open carry slot
  int i;
  for (i = 0; p->inventory[i]; i++) {
  }

  // Check if carry inventory full
  if (i == NUM_INVENTORY_SLOTS) {
    draw_message("Can't carry anymore items!");
    return PLAYER_MOVE_INVALID;
  }

  // Put item in carry slot and remove from map
  p->inventory[i] = d->object_map[p->pos.r][p->pos.c];
  draw_message("Picked up %s", d->object_map[p->pos.r][p->pos.c]->name.c_str());
  d->object_map[p->pos.r][p->pos.c] = NULL;
  return PLAYER_MOVE_ACTION;
}

PLAYER_ACTION drop_item(Dungeon *d, Player *p) {
  Point ppos = p->pos;
  if (d->object_map[ppos.r][ppos.c]) {
    // TODO: add stacks in the future
    draw_message("Can't drop item on another item.");
    return PLAYER_MOVE_INVALID;
  }

  // prompt player which item
  int selected = select_item(p);
  if (selected == SELECT_FAIL)
    return PLAYER_MOVE_INVALID;

  // dropping no item
  if (!p->inventory[selected]) {
    draw_message("Dropped nothing. (slot: %d)", selected);
    return PLAYER_MOVE_INVALID;
  }

  // put item on map, remove from inventory
  draw_message("Dropped %s.", p->inventory[selected]->name.c_str());
  d->object_map[p->pos.r][p->pos.c] = p->inventory[selected];
  p->inventory[selected] = NULL;

  return PLAYER_MOVE_ACTION;
}

PLAYER_ACTION take_off_item(Player *p) {
  int selected = select_equipment(p);
  if (selected == SELECT_FAIL)
    return PLAYER_MOVE_INVALID;

  // unequip nothing
  if (!p->equipment[selected]) {
    draw_message("Took off nothing.");
    return PLAYER_MOVE_INVALID;
  }

  // find, if any, inv slot to put item
  int inv_slot;
  for (inv_slot = 0; p->inventory[inv_slot]; inv_slot++) {
  }
  if (inv_slot == NUM_INVENTORY_SLOTS) {
    draw_message("Not enough room in inventory.");
    return PLAYER_MOVE_INVALID;
  }

  // take off item and put in inventory
  draw_message("Took off %s", p->equipment[selected]->name.c_str());
  p->inventory[inv_slot] = p->equipment[selected];
  p->equipment[selected] = NULL;

  return PLAYER_MOVE_ACTION;
}

PLAYER_ACTION wear_item(Player *p) {
  int selected = select_item(p);
  if (selected == SELECT_FAIL)
    return PLAYER_MOVE_INVALID;

  if (!p->inventory[selected]) {
    draw_message("Equipped nothing.");
    return PLAYER_MOVE_INVALID;
  }

  OBJECT_TYPE obj_type = p->inventory[selected]->type;
  int equipment_slot;
  switch (obj_type) {
  case WEAPON:
    equipment_slot = WEAPON_SLOT;
    break;
  case OFFHAND:
    equipment_slot = OFFHAND;
    break;
  case RANGED:
    equipment_slot = RANGED_SLOT;
    break;
  case ARMOR:
    equipment_slot = ARMOR_SLOT;
    break;
  case HELMET:
    equipment_slot = HELMET_SLOT;
    break;
  case CLOAK:
    equipment_slot = CLOAK_SLOT;
    break;
  case GLOVES:
    equipment_slot = GLOVES_SLOT;
    break;
  case BOOTS:
    equipment_slot = BOOTS_SLOT;
    break;
  case AMULET:
    equipment_slot = AMULET_SLOT;
    break;
  case LIGHT:
    equipment_slot = LIGHT_SLOT;
    break;
  case RING:
    if (p->equipment[RING1_SLOT] && p->equipment[RING2_SLOT] ||
        !p->equipment[RING1_SLOT])
      equipment_slot = RING1_SLOT;
    else
      equipment_slot = RING2_SLOT;
    break;
  default:
    return PLAYER_MOVE_INVALID;
  }

  // SWAP items
  if (p->equipment[equipment_slot]) {
    Object *tmp = p->equipment[equipment_slot];
    p->equipment[equipment_slot] = p->inventory[selected];
    p->inventory[selected] = tmp;
  } else /* equip normally */ {
    p->equipment[equipment_slot] = p->inventory[selected];
    p->inventory[selected] = NULL;
  }
  draw_message("Equipped %s.", p->equipment[equipment_slot]);

  return PLAYER_MOVE_ACTION;
}

PLAYER_ACTION expunge_item(Player *p) {
  int selected = select_item(p);
  if (selected == SELECT_FAIL)
    return PLAYER_MOVE_INVALID;

  if (!p->inventory[selected]) {
    draw_message("Expunged nothing.");
    return PLAYER_MOVE_INVALID;
  }

  draw_message("Expunged %s.", p->inventory[selected]->name.c_str());
  delete (p->inventory[selected]);
  p->inventory[selected] = NULL;

  return PLAYER_MOVE_ACTION;
}

PLAYER_ACTION inspect_item(Player *p) {
  int selected = select_item(p);
  if (selected == SELECT_FAIL || !p->inventory[selected])
    return PLAYER_MOVE_INVALID;

  clear();
  refresh();
  Object *item = p->inventory[selected];

  int i = 0;
  mvprintw(i++, 0, ("name: " + item->name).c_str());
  std::stringstream ss(item->desc);
  std::string line;
  for (; std::getline(ss, line); i++) {
    if (line == "")
      continue;
    if (i == 1)
      mvprintw(i, 0, ("desc: " + line).c_str());
    else
      mvprintw(i, 0, line.c_str());
  }
  mvprintw(i++, 0, ("type: " + std::string(to_string(item->type))).c_str());
  mvprintw(i++, 0, ("dam: " + item->dam.to_string()).c_str());
  mvprintw(i++, 0, ("dodge: " + std::to_string(item->dodge)).c_str());
  mvprintw(i++, 0, ("def: " + std::to_string(item->def)).c_str());
  mvprintw(i++, 0, ("weight: " + std::to_string(item->weight)).c_str());
  mvprintw(i++, 0, ("speed: " + std::to_string(item->speed)).c_str());
  mvprintw(i++, 0, ("val: " + std::to_string(item->val)).c_str());
  item->art ? line = "false" : line = "true";
  mvprintw(i++, 0, ("art: " + line).c_str());
  mvprintw(i++, 0, ("rrty: " + std::to_string(item->rrty)).c_str());

  int action = getch();

  return PLAYER_MOVE_MENU;
}
