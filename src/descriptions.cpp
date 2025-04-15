#include <fstream>
#include <iostream>
#include <ostream>
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

  Object_Description od;

  bool create = false;
  bool name_set = false;
  bool desc_set = false;
  bool type_set = false;
  bool color_set = false;
  bool hit_set = false;
  bool dam_set = false;
  bool dodge_set = false;
  bool def_set = false;
  bool weight_set = false;
  bool speed_set = false;
  bool attr_set = false;
  bool val_set = false;
  bool art_set = false;
  bool rrty_set = false;

  std::string keyword;
  std::string data;
  while (std::getline(file, line)) {
    std::stringstream ss(line);
    std::getline(ss, keyword, ' ');

    if (keyword == "NAME") {
      if (name_set)
        return;
      std::getline(ss, data);
      if (!od.set_name(data))
        return;
      name_set = true;

    } else if (keyword == "DESC") {
      if (desc_set)
        return;
      std::string desc = "";
      while (std::getline(file, line) && line != ".") {
        desc += line + "\n";
      }
      if (!od.set_desc(desc))
        return;
      desc_set = true;

    } else if (keyword == "TYPE") {
      if (type_set)
        return;
      std::getline(ss, data);
      if (!od.set_type(data))
        return;
      type_set = true;

    } else if (keyword == "COLOR") {
      if (color_set)
        return;
      std::getline(ss, data);
      if (!od.set_color(data))
        return;
      color_set = true;

    } else if (keyword == "HIT") {
      if (hit_set)
        return;
      std::getline(ss, data);
      if (!od.set_hit(data))
        return;
      hit_set = true;

    } else if (keyword == "DAM") {
      if (dam_set)
        return;
      std::getline(ss, data);
      if (!od.set_dam(data))
        return;
      dam_set = true;

    } else if (keyword == "DODGE") {
      if (dodge_set)
        return;
      std::getline(ss, data);
      if (!od.set_dodge(data))
        return;
      dodge_set = true;

    } else if (keyword == "DEF") {
      if (def_set)
        return;
      std::getline(ss, data);
      if (!od.set_def(data))
        return;
      def_set = true;

    } else if (keyword == "WEIGHT") {
      if (weight_set)
        return;
      std::getline(ss, data);
      if (!od.set_weight(data))
        return;
      weight_set = true;

    } else if (keyword == "SPEED") {
      if (speed_set)
        return;
      std::getline(ss, data);
      if (!od.set_speed(data))
        return;
      speed_set = true;

    } else if (keyword == "ATTR") {
      if (attr_set)
        return;
      std::getline(ss, data);
      if (!od.set_attr(data))
        return;
      attr_set = true;

    } else if (keyword == "VAL") {
      if (val_set)
        return;
      std::getline(ss, data);
      if (!od.set_val(data))
        return;
      val_set = true;

    } else if (keyword == "ART") {
      if (art_set)
        return;
      std::getline(ss, data);
      if (!od.set_art(data))
        return;
      art_set = true;

    } else if (keyword == "RRTY") {
      if (rrty_set)
        return;
      std::getline(ss, data);
      if (!od.set_rrty(data))
        return;
      rrty_set = true;

    } else if (keyword == "END") {
      if (!(name_set && desc_set && type_set && color_set && hit_set &&
            dam_set && dodge_set && def_set && weight_set && speed_set &&
            attr_set && val_set && art_set && rrty_set)) {
        std::cout << "Missing fields before END: " << keyword << std::endl;
        std::cout << "Skipping object." << std::endl;
        return;
      }

      // Successfully parsed the object
      od.print_info();
      std::cout << std::endl;
      break;

    } else {
      std::cout << "Invalid keyword: " << keyword << std::endl;
      std::cout << "Skipping object." << std::endl;
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
