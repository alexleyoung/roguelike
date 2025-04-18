#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <gen.hpp>
#include <saves.hpp>
#include <utils.hpp>

#if defined(__APPLE__)
#include "../include/portable_endian.h"
#else
#include <endian.h>
#endif

/*
Save current dungeon to big endian binary file

Returns 0 on success, non-zero on failure.
*/
int save_dungeon(Dungeon *dungeon, const char *name) {
  // create path to file in case we want different save files eventually
  char *dir = get_save_path(name);
  FILE *f = fopen(dir, "wb");
  if (f == NULL) {
    printf("Error: Could not save file %s\n", dir);
    return 1;
  }

  const char *marker = "RLG327-S2025"; // 12 bytes
  for (int i = 0; i < 12; i += 1) {
    uint8_t letter = marker[i];
    fwrite(&letter, sizeof(letter), 1, f);
  }

  // write version marker (4B)
  uint32_t version = 0;
  version = htobe32(version);
  fwrite(&version, 4, 1, f);

  // write size of file (4B)
  uint32_t size = 0;
  // calcualte size of file
  size += 12;                             // semantic marker 12 char
  size += 4;                              // version 1 uint32
  size += 4;                              // size of file 1 uint32
  size += 2;                              // player location, 2 uint8
  size += DUNGEON_HEIGHT * DUNGEON_WIDTH; // tile each uint8 hardness
  size += 2;                              // num rooms 1 uint16
  for (int i = 0; i < dungeon->num_rooms; i++) {
    size += 2; // room corner
    size += 2; // room size
  }
  size += 2; // num up stairs 1 uint16
  size += 2; // num down stairs 1 uint16
  for (int i = 0; i < dungeon->num_stairs; i++) {
    size += 2; // stairs 2 uint8
  }
  size = htobe32(size);
  fwrite(&size, sizeof(size), 1, f);

  // write location of player (2B)
  fwrite(&dungeon->player_pos.c, sizeof(dungeon->player_pos.c), 1, f);
  fwrite(&dungeon->player_pos.r, sizeof(dungeon->player_pos.r), 1, f);

  // write tiles array
  for (int r = 0; r < DUNGEON_HEIGHT; r++) {
    for (int c = 0; c < DUNGEON_WIDTH; c++) {
      // write tile
      uint8_t hardness = dungeon->tiles[r][c].hardness;
      fwrite(&hardness, sizeof(hardness), 1, f);
    }
  }

  // write num_rooms (2B)
  uint16_t r = dungeon->num_rooms;
  r = htobe16(r);
  fwrite(&r, sizeof(r), 1, f);
  r = be16toh(r);

  // write positions of rooms (r * 4B)
  for (int i = 0; i < r; i++) {
    uint8_t corner_c = dungeon->rooms[i].corner.c;
    uint8_t corner_r = dungeon->rooms[i].corner.r;
    fwrite(&corner_c, sizeof(corner_c), 1, f);
    fwrite(&corner_r, sizeof(corner_r), 1, f);

    uint8_t size_c = dungeon->rooms[i].size.c;
    uint8_t size_r = dungeon->rooms[i].size.r;
    fwrite(&size_c, sizeof(size_c), 1, f);
    fwrite(&size_r, sizeof(size_r), 1, f);
  }

  // count stairs
  uint16_t u = 0;
  uint16_t d = 0;
  for (int i = 0; i < dungeon->num_stairs; i++) {
    if (dungeon->stairs[i].type == 0) {
      u++;
    } else {
      d++;
    }
  }

  // write # up stairs (2B)
  u = htobe16(u);
  fwrite(&u, sizeof(u), 1, f);
  u = be16toh(u); // undo to make sure loop is correct

  // write positions of up stairs (u * 2B)
  for (int i = 0; i < u + d; i++) {
    if (dungeon->stairs[i].type == 0) {
      uint8_t c = dungeon->stairs[i].p.c;
      uint8_t r = dungeon->stairs[i].p.r;
      fwrite(&c, sizeof(c), 1, f);
      fwrite(&r, sizeof(r), 1, f);
    }
  }

  // write # down stairs (2B)
  d = htobe16(d);
  fwrite(&d, sizeof(d), 1, f);
  d = be16toh(d); // undo for loop

  // write positions of down stairs (d * 2B)
  for (int i = 0; i < u + d; i++) {
    if (dungeon->stairs[i].type == 1) {
      uint8_t c = dungeon->stairs[i].p.c;
      uint8_t r = dungeon->stairs[i].p.r;
      fwrite(&c, sizeof(c), 1, f);
      fwrite(&r, sizeof(r), 1, f);
    }
  }

  fclose(f);
  return 0;
}

/*
Loads dungeon from binary file with format from assignment specification.
Creates dungeon struct (according to types.h), fills it with data from file,
then draws the appropriate sprites.

Returns 0 on success, non-zero on failure.
*/
int load_dungeon(Dungeon *dungeon, const char *name) {
  init_dungeon(dungeon);

  char marker[13];
  uint32_t version, file_size;
  uint8_t player_x, player_y, hardness;
  uint16_t r, u, d;

  // create path to file in case we want different save files eventually
  char *dir = get_save_path(name);
  FILE *f = fopen(dir, "rb");
  if (f == NULL) {
    printf("Error: Could not open file to load: %s\n", dir);
    return 1;
  }

  // get size of file
  fseek(f, 0, SEEK_END);
  file_size = ftell(f);
  fseek(f, 0, SEEK_SET);

  // read semantic marker byte by byte converting to h endinaness
  for (int i = 0; i < 12; i++) {
    uint8_t letter;
    fread(&letter, sizeof(letter), 1, f);
    marker[i] = letter;
  }
  marker[12] = '\0';
  // check if file is valid
  if (strcmp(marker, "RLG327-S2025") != 0) {
    printf("Error: Invalid marker %s\n", marker);
    return 1;
  }

  // read version marker
  fread(&version, sizeof(version), 1, f);

  // read size of file
  fread(&file_size, sizeof(file_size), 1, f);
  file_size = be32toh(file_size);

  // create player
  fread(&dungeon->player_pos.c, sizeof(dungeon->player_pos.c), 1, f);
  fread(&dungeon->player_pos.r, sizeof(dungeon->player_pos.r), 1, f);

  /*Player *pl = (Player *)(malloc(sizeof(Player)));*/
  /*create_player(pl, dungeon->player_pos);*/
  Player *pl = new Player(dungeon->player_pos);
  dungeon->character_map[pl->pos.r][pl->pos.c] = pl;

  // read tiles array
  for (int r = 0; r < DUNGEON_HEIGHT; r++) {
    for (int c = 0; c < DUNGEON_WIDTH; c++) {
      fread(&hardness, sizeof(hardness), 1, f);
      dungeon->tiles[r][c].hardness = hardness;
      if (hardness == 0) {
        dungeon->tiles[r][c].sprite = '#';
      } else {
        dungeon->tiles[r][c].sprite = ' ';
      }

      // check if border
      if (c == 0 || c == DUNGEON_WIDTH - 1) {
        dungeon->tiles[r][c].sprite = '|';
      }
      if (r == 0 || r == DUNGEON_HEIGHT - 1) {
        dungeon->tiles[r][c].sprite = '-';
      }
    }
  }

  // read num_rooms
  fread(&r, sizeof(r), 1, f);
  r = be16toh(r);
  dungeon->num_rooms = r;
  dungeon->rooms = (Room *)(malloc(sizeof(*dungeon->rooms) * r));

  for (int i = 0; i < r; i++) {
    fread(&dungeon->rooms[i].corner.c, sizeof(dungeon->rooms[i].corner.c), 1,
          f);
    fread(&dungeon->rooms[i].corner.r, sizeof(dungeon->rooms[i].corner.r), 1,
          f);
    fread(&dungeon->rooms[i].size.c, sizeof(dungeon->rooms[i].size.c), 1, f);
    fread(&dungeon->rooms[i].size.r, sizeof(dungeon->rooms[i].size.r), 1, f);
  }

  // read up Stair count
  fread(&u, sizeof(u), 1, f);
  u = be16toh(u);
  dungeon->stairs = (Stair *)(malloc(sizeof(*dungeon->stairs) * u));

  for (int i = 0; i < u; i++) {
    fread(&dungeon->stairs[i].p.c, sizeof(dungeon->stairs[i].p.c), 1, f);
    fread(&dungeon->stairs[i].p.r, sizeof(dungeon->stairs[i].p.r), 1, f);
    dungeon->stairs[i].type = 0;
  }

  // read down Stair count
  fread(&d, sizeof(d), 1, f);
  d = be16toh(d);
  dungeon->stairs =
      (Stair *)(realloc(dungeon->stairs, sizeof(*dungeon->stairs) * (u + d)));

  for (int i = 0; i < d; i++) {
    fread(&dungeon->stairs[u + i].p.c, sizeof(dungeon->stairs[u + i].p.c), 1,
          f);
    fread(&dungeon->stairs[u + i].p.r, sizeof(dungeon->stairs[u + i].p.r), 1,
          f);
    dungeon->stairs[u + i].type = 1;
  }

  // place room sprites
  for (int i = 0; i < r; i++) {
    for (int j = 0; j < dungeon->rooms[i].size.r; j++) {
      for (int k = 0; k < dungeon->rooms[i].size.c; k++) {
        dungeon
            ->tiles[dungeon->rooms[i].corner.r + j]
                   [dungeon->rooms[i].corner.c + k]
            .sprite = '.';
      }
    }
  }
  // place Stair sprites
  int stair_count = 0;
  for (int i = 0; i < u; i++) {
    dungeon->tiles[dungeon->stairs[i].p.r][dungeon->stairs[i].p.c].sprite = '<';
  }
  for (int i = 0; i < d; i++) {
    dungeon->tiles[dungeon->stairs[u + i].p.r][dungeon->stairs[u + i].p.c]
        .sprite = '>';
  }
  // place player sprite
  dungeon->tiles[dungeon->player_pos.r][dungeon->player_pos.c].sprite = '@';

  fclose(f);
  return 0;
}
