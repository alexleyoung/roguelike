#include <fstream>
#include <iostream>
#include <sstream>

#include "descriptions.hpp"
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
        description += line + "\n";
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
          !ad.length() || !symbol.length() || !rarity.length()) {
        std::cout << "missing fields: " << keyword << std::endl;
        std::cout << "Skipping monster. " << keyword << std::endl;
        return;
      }

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

void parse_object(std::ifstream &file, std::string &line) {
  while (std::getline(file, line) && line != "BEGIN OBJECT") {
  }

  // fields
  std::string name;
  std::string desc;
  std::string type;
  std::string color;
  std::string hit;
  std::string dam;
  std::string dodge;
  std::string def;
  std::string weight;
  std::string speed;
  std::string attr;
  std::string val;
  std::string art;
  std::string rrty;

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
      if (desc.length())
        return;
      while (std::getline(file, line) && line != ".") {
        desc += line + "\n";
      }

    } else if (keyword == "TYPE") {
      if (type.length())
        return;
      std::getline(ss, type);

    } else if (keyword == "COLOR") {
      if (color.length())
        return;
      std::getline(ss, color);

    } else if (keyword == "HIT") {
      if (hit.length())
        return;
      std::getline(ss, hit);

    } else if (keyword == "DAM") {
      if (dam.length())
        return;
      std::getline(ss, dam);
    } else if (keyword == "DODGE") {
      if (dodge.length())
        return;
      std::getline(ss, dodge);
    } else if (keyword == "DEF") {
      if (def.length())
        return;
      std::getline(ss, def);
    } else if (keyword == "WEIGHT") {
      if (weight.length())
        return;
      std::getline(ss, weight);
    } else if (keyword == "SPEED") {
      if (speed.length())
        return;
      std::getline(ss, speed);
    } else if (keyword == "ATTR") {
      if (attr.length())
        return;
      std::getline(ss, attr);
    } else if (keyword == "VAL") {
      if (val.length())
        return;
      std::getline(ss, val);
    } else if (keyword == "ART") {
      if (art.length())
        return;
      std::getline(ss, art);
    } else if (keyword == "RRTY") {
      if (rrty.length())
        return;
      std::getline(ss, rrty);
    } else if (keyword == "END") {
      // if missing field, exit
      if (!name.length() || !desc.length() || !type.length() ||
          !color.length() || !hit.length() || !dam.length() ||
          !dodge.length() || !def.length() || !weight.length() ||
          !speed.length() || !attr.length() || !val.length() || !art.length() ||
          !rrty.length()) {
        std::cout << "missing fields: " << keyword << std::endl;
        std::cout << "Skipping monster. " << keyword << std::endl;
        return;
      }

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

int load_object_descriptions(const char *fileName) {
  std::string path = get_save_path(fileName);

  std::ifstream file(path);
  if (!file.is_open()) {
    std::cout << "Couldn't open file: " << path << std::endl;
    return 1;
  }

  std::string line;

  // check metadata
  std::getline(file, line);
  if (line != "RLG327 OBJECT DESCRIPTION 1") {
    std::cout << "Not a proper Monster Description File!" << std::endl;
    return 1;
  }

  // until no more lines / eof
  while (std::getline(file, line)) {
    parse_object(file, line);
  }

  return 0;
}
