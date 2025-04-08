#include <fstream>
#include <iostream>
#include <sstream>

#include "character.hpp"
#include "load_monsters.hpp"
#include "saves.hpp"

void parse_monster(std::ifstream &file, std::string &line) {
  while (std::getline(file, line) && line != "BEGIN MONSTER") {
  }

  // fields
  std::string name;
  std::string description;
  std::string color;
  std::string speed;
  std::string abilities;
  std::string hp;
  std::string ad;
  std::string symbol;
  std::string rarity;

  bool create = false;
  std::string keyword;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::getline(ss, keyword, ' ');

    if (keyword == "NAME") {
      if (name.length())
        return;
      std::getline(ss, name);

    } else if (keyword == "DESC") {
      if (description.length())
        return;
      while (std::getline(file, line) && line != ".") {
        description += line;
      }

    } else if (keyword == "COLOR") {
      if (color.length())
        return;
      std::getline(ss, color);

    } else if (keyword == "SPEED") {
      if (speed.length())
        return;
      std::getline(ss, speed);

    } else if (keyword == "ABIL") {
      if (abilities.length())
        return;
      std::getline(ss, abilities);

    } else if (keyword == "HP") {
      if (hp.length())
        return;
      std::getline(ss, hp);

    } else if (keyword == "DAM") {
      if (ad.length())
        return;
      std::getline(ss, ad);

    } else if (keyword == "SYMB") {
      if (symbol.length())
        return;
      std::getline(ss, symbol);

    } else if (keyword == "RRTY") {
      if (rarity.length())
        return;
      std::getline(ss, rarity);

    } else if (keyword == "END") {
      // if missing field, exit
      if (!name.length() || !description.length() || !color.length() ||
          !speed.length() || !abilities.length() || !hp.length() ||
          !ad.length() || !symbol.length() || !rarity.length())
        return;

      std::cout << std::endl;
      Monster_Description md(name, description, color, speed, abilities, hp, ad,
                             symbol, rarity);
      md.print_info();
      break;

    } else {
      std::cout << "invalid keyword: " << keyword << std::endl;
      std::cout << "Skipping monster. " << keyword << std::endl;
      return;
    }
  }
}

int load_monster_descriptions(const char *fileName) {
  std::string path = get_save_path(fileName);

  std::ifstream file(path);
  if (!file.is_open()) {
    std::cout << "Couldn't open file: " << path << std::endl;
    return 1;
  }

  std::string line;

  // check metadata
  std::getline(file, line);
  if (line != "RLG327 MONSTER DESCRIPTION 1") {
    std::cout << "Not a proper Monster Description File!" << std::endl;
    return 1;
  }

  // until no more lines / eof
  while (std::getline(file, line)) {
    parse_monster(file, line);
  }

  return 0;
}
