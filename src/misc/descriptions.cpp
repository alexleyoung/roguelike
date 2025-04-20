#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <vector>

#include <character.hpp>
#include <descriptions.hpp>

/*
 * UTIL METHODS
 */
bool parse_dice(const std::string &str, Dice &target) {
  try {
    std::stringstream ss(str);
    std::string val;

    std::getline(ss, val, '+');
    target.set_base(std::stoi(val));

    std::getline(ss, val, 'd');
    target.set_number(std::stoi(val));

    std::getline(ss, val);
    target.set_sides(std::stoi(val));
  } catch (...) {
    return false;
  }
  return true;
}

const char *to_string(COLOR c) {
  switch (c) {
  case RED:
    return "RED";
  case GREEN:
    return "GREEN";
  case BLUE:
    return "BLUE";
  case CYAN:
    return "CYAN";
  case YELLOW:
    return "YELLOW";
  case MAGENTA:
    return "MAGENTA";
  case WHITE:
    return "WHITE";
  case BLACK:
    return "BLACK";
  default:
    return "UNKNOWN_COLOR";
  }
}

const char *to_string(ABILITY a) {
  switch (a) {
  case SMART:
    return "SMART";
  case TELE:
    return "TELE";
  case TUNNEL:
    return "TUNNEL";
  case ERRATIC:
    return "ERRATIC";
  case PASS:
    return "PASS";
  case PICKUP:
    return "PICKUP";
  case DESTROY:
    return "DESTROY";
  case UNIQ:
    return "UNIQ";
  case BOSS:
    return "BOSS";
  default:
    return "";
  }
}

const char *to_string(OBJECT_TYPE t) {
  switch (t) {
  case WEAPON:
    return "WEAPON";
  case OFFHAND:
    return "OFFHAND";
  case RANGED:
    return "RANGED";
  case ARMOR:
    return "ARMOR";
  case HELMET:
    return "HELMET";
  case CLOAK:
    return "CLOAK";
  case GLOVES:
    return "GLOVES";
  case BOOTS:
    return "BOOTS";
  case RING:
    return "RING";
  case AMULET:
    return "AMULET";
  case LIGHT:
    return "LIGHT";
  case SCROLL:
    return "SCROLL";
  case BOOK:
    return "BOOK";
  case FLASK:
    return "FLASK";
  case GOLD:
    return "GOLD";
  case AMMUNITION:
    return "AMMUNITION";
  case FOOD:
    return "FOOD";
  case WAND:
    return "WAND";
  case CONTAINER:
    return "CONTAINER";
  default:
    return "";
  }
}

/*
 * MONSTER_DESCRIPTION CLASS IMPLEMENTATIONS
 */
bool Monster_Description::set_name(const std::string &name) {
  this->name = name;
  return true;
}

bool Monster_Description::set_desc(const std::string &desc) {
  this->desc = desc;
  return true;
}

bool Monster_Description::set_color(const std::string &color_str) {
  int count = 0;
  std::stringstream ss(color_str);
  std::string token;
  while (count < NUM_COLORS && std::getline(ss, token, ' ')) {
    if (token == "RED")
      color[count++] = RED;
    else if (token == "GREEN")
      color[count++] = GREEN;
    else if (token == "BLUE")
      color[count++] = BLUE;
    else if (token == "CYAN")
      color[count++] = CYAN;
    else if (token == "YELLOW")
      color[count++] = YELLOW;
    else if (token == "MAGENTA")
      color[count++] = MAGENTA;
    else if (token == "WHITE")
      color[count++] = WHITE;
    else if (token == "BLACK")
      color[count++] = BLACK;
  }
  return count > 0;
}

bool Monster_Description::set_speed(const std::string &s) {
  return parse_dice(s, speed);
}
bool Monster_Description::set_hp(const std::string &s) {
  return parse_dice(s, hp);
}
bool Monster_Description::set_dam(const std::string &s) {
  return parse_dice(s, dam);
}

bool Monster_Description::set_abil(const std::string &s) {
  int count = 0;
  std::stringstream ss(s);
  std::string token;
  while (count < NUM_ABILITIES && std::getline(ss, token, ' ')) {
    if (token == "SMART")
      abil[count++] = SMART;
    else if (token == "TELE")
      abil[count++] = TELE;
    else if (token == "TUNNEL")
      abil[count++] = TUNNEL;
    else if (token == "ERRATIC")
      abil[count++] = ERRATIC;
    else if (token == "PASS")
      abil[count++] = PASS;
    else if (token == "PICKUP")
      abil[count++] = PICKUP;
    else if (token == "DESTROY")
      abil[count++] = DESTROY;
    else if (token == "UNIQ")
      abil[count++] = UNIQ;
    else if (token == "BOSS")
      abil[count++] = BOSS;
  }
  return count > 0;
}

bool Monster_Description::set_symb(const std::string &s) {
  if (s.length() != 1)
    return false;
  symb = s[0];
  return true;
}

bool Monster_Description::set_rrty(const std::string &s) {
  try {
    rrty = std::stoi(s);
  } catch (...) {
    return false;
  }
  return true;
}

bool Monster_Description::is_unique() {
  for (ABILITY a : abil) {
    if (a == UNIQ)
      return true;
  }

  return false;
}

Monster *Monster_Description::generate(int id) {
  int abilities = 0;

  for (int i = 0; i < NUM_ABILITIES; i++) {
    switch (abil[i]) {
    case SMART:
      abilities = abilities | ATTRIBUTE_INTELLIGENT;
      break;
    case TELE:
      abilities = abilities | ATTRIBUTE_TELEPATHIC;
      break;
    case TUNNEL:
      abilities = abilities | ATTRIBUTE_TUNNELING;
      break;
    case ERRATIC:
      abilities = abilities | ATTRIBUTE_ERRATIC;
      break;
    case PASS:
      abilities = abilities | ATTRIBUTE_PASS;
      break;
    case PICKUP:
      abilities = abilities | ATTRIBUTE_PICKUP;
      break;
    case DESTROY:
      abilities = abilities | ATTRIBUTE_DESTROY;
      break;
    case UNIQ:
      abilities = abilities | ATTRIBUTE_UNIQ;
      break;
    case BOSS:
      abilities = abilities | ATTRIBUTE_BOSS;
      break;
    case INVALID_ABILITY:
      break;
    }
  }
  return new Monster(id, name, desc, color[0], speed.roll(), abilities,
                     hp.roll(), dam, symb, rrty);
}

void Monster_Description::print_info() {
  std::cout << "Name: " << name << "\n";
  std::cout << "Description:\n" << desc;
  std::cout << "Symbol: " << symb << "\n";

  std::cout << "Color(s): ";
  for (int i = 0; i < NUM_COLORS; ++i) {
    if (color[i] != INVALID_COLOR)
      std::cout << to_string(color[i]) << " ";
  }
  std::cout << "\n";

  std::cout << "Speed: " << speed << "\n";

  std::cout << "Abilities: ";
  for (int i = 0; i < NUM_ABILITIES; ++i) {
    if (abil[i] != INVALID_ABILITY)
      std::cout << to_string(abil[i]) << " ";
  }
  std::cout << "\n";

  std::cout << "HP: " << hp << "\n";
  std::cout << "Damage: " << dam << "\n";
  std::cout << "Rarity: " << rrty << "\n";
}

/*
 * OBJECT_DESCRIPTION CLASS IMPLEMENTATIONS
 */
bool Object_Description::set_name(const std::string &name) {
  this->name = name;
  return true;
}

bool Object_Description::set_desc(const std::string &desc) {
  this->desc = desc;
  return true;
}

bool Object_Description::set_type(const std::string &type_str) {
  if (type_str == "WEAPON")
    type = WEAPON;
  else if (type_str == "OFFHAND")
    type = OFFHAND;
  else if (type_str == "RANGED")
    type = RANGED;
  else if (type_str == "ARMOR")
    type = ARMOR;
  else if (type_str == "HELMET")
    type = HELMET;
  else if (type_str == "CLOAK")
    type = CLOAK;
  else if (type_str == "GLOVES")
    type = GLOVES;
  else if (type_str == "BOOTS")
    type = BOOTS;
  else if (type_str == "RING")
    type = RING;
  else if (type_str == "AMULET")
    type = AMULET;
  else if (type_str == "LIGHT")
    type = LIGHT;
  else if (type_str == "SCROLL")
    type = SCROLL;
  else if (type_str == "BOOK")
    type = BOOK;
  else if (type_str == "FLASK")
    type = FLASK;
  else if (type_str == "GOLD")
    type = GOLD;
  else if (type_str == "AMMUNITION")
    type = AMMUNITION;
  else if (type_str == "FOOD")
    type = FOOD;
  else if (type_str == "WAND")
    type = WAND;
  else if (type_str == "CONTAINER")
    type = CONTAINER;
  else
    return false;
  return true;
}

bool Object_Description::set_color(const std::string &color_str) {
  int count = 0;
  std::stringstream ss(color_str);
  std::string token;
  while (count < NUM_COLORS && std::getline(ss, token, ' ')) {
    if (token == "RED")
      color[count++] = RED;
    else if (token == "GREEN")
      color[count++] = GREEN;
    else if (token == "BLUE")
      color[count++] = BLUE;
    else if (token == "CYAN")
      color[count++] = CYAN;
    else if (token == "YELLOW")
      color[count++] = YELLOW;
    else if (token == "MAGENTA")
      color[count++] = MAGENTA;
    else if (token == "WHITE")
      color[count++] = WHITE;
    else if (token == "BLACK")
      color[count++] = BLACK;
  }
  return count > 0;
}

bool Object_Description::set_hit(const std::string &s) {
  return parse_dice(s, hit);
}
bool Object_Description::set_dam(const std::string &s) {
  return parse_dice(s, dam);
}
bool Object_Description::set_dodge(const std::string &s) {
  return parse_dice(s, dodge);
}
bool Object_Description::set_def(const std::string &s) {
  return parse_dice(s, def);
}
bool Object_Description::set_weight(const std::string &s) {
  return parse_dice(s, weight);
}
bool Object_Description::set_speed(const std::string &s) {
  return parse_dice(s, speed);
}
bool Object_Description::set_attr(const std::string &s) {
  return parse_dice(s, attr);
}
bool Object_Description::set_val(const std::string &s) {
  return parse_dice(s, val);
}

bool Object_Description::set_art(const std::string &art_str) {
  if (art_str == "TRUE")
    art = true;
  else if (art_str == "FALSE")
    art = false;
  else
    return false;
  return true;
}

bool Object_Description::set_rrty(const std::string &rrty_str) {
  try {
    rrty = std::stoi(rrty_str);
  } catch (...) {
    return false;
  }
  return true;
}

Object *Object_Description::generate() {
  return new Object(name, desc, type, color[0], hit.roll(), dam, dodge.roll(),
                    def.roll(), weight.roll(), speed.roll(), attr.roll(),
                    val.roll(), art, rrty);
}

void Object_Description::print_info() {
  std::cout << "Name: " << name << "\n";
  std::cout << "Description:\n" << desc;
  std::cout << "Type: " << type << "\n";

  std::cout << "Color(s): ";
  for (int i = 0; i < NUM_COLORS; ++i) {
    if (color[i] != INVALID_COLOR)
      std::cout << to_string(color[i]) << " ";
  }
  std::cout << "\n";

  std::cout << "Hit: " << hit << "\n";
  std::cout << "Damage: " << dam << "\n";
  std::cout << "Dodge: " << dodge << "\n";
  std::cout << "Defense: " << def << "\n";
  std::cout << "Weight: " << weight << "\n";
  std::cout << "Speed: " << speed << "\n";
  std::cout << "Attribute: " << attr << "\n";
  std::cout << "Value: " << val << "\n";
  std::cout << "Artifact: " << (art ? "Yes" : "No") << "\n";
  std::cout << "Rarity: " << rrty << "\n";
}

/*
 * FUNCTIONS
 */
void parse_monster(std::ifstream &file, std::string &line,
                   std::vector<Monster_Description> &vec) {
  while (std::getline(file, line) && line != "BEGIN MONSTER") {
  }

  Monster_Description md;

  bool name_set = false;
  bool desc_set = false;
  bool color_set = false;
  bool speed_set = false;
  bool abil_set = false;
  bool hp_set = false;
  bool dam_set = false;
  bool symb_set = false;
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
      if (!md.set_name(data))
        return;
      name_set = true;

    } else if (keyword == "DESC") {
      if (desc_set)
        return;
      std::string desc;
      while (std::getline(file, line) && line != ".") {
        desc += line + "\n";
      }
      if (!md.set_desc(desc))
        return;
      desc_set = true;

    } else if (keyword == "COLOR") {
      if (color_set)
        return;
      std::getline(ss, data);
      if (!md.set_color(data))
        return;
      color_set = true;

    } else if (keyword == "SPEED") {
      if (speed_set)
        return;
      std::getline(ss, data);
      if (!md.set_speed(data))
        return;
      speed_set = true;

    } else if (keyword == "ABIL") {
      if (abil_set)
        return;
      std::getline(ss, data);
      if (!md.set_abil(data))
        return;
      abil_set = true;

    } else if (keyword == "HP") {
      if (hp_set)
        return;
      std::getline(ss, data);
      if (!md.set_hp(data))
        return;
      hp_set = true;

    } else if (keyword == "DAM") {
      if (dam_set)
        return;
      std::getline(ss, data);
      if (!md.set_dam(data))
        return;
      dam_set = true;

    } else if (keyword == "SYMB") {
      if (symb_set)
        return;
      std::getline(ss, data);
      if (!md.set_symb(data))
        return;
      symb_set = true;

    } else if (keyword == "RRTY") {
      if (rrty_set)
        return;
      std::getline(ss, data);
      if (!md.set_rrty(data))
        return;
      rrty_set = true;

    } else if (keyword == "END") {
      if (!(name_set && desc_set && color_set && speed_set && abil_set &&
            hp_set && dam_set && symb_set && rrty_set)) {
        std::cout << "Missing fields before END.\n";
        std::cout << "Skipping monster.\n";
        return;
      }

      std::cout << std::endl;
      vec.push_back(md);
      break;

    } else {
      std::cout << "Invalid keyword: " << keyword << std::endl;
      std::cout << "Skipping monster.\n";
      return;
    }
  }
}

void parse_object(std::ifstream &file, std::string &line,
                  std::vector<Object_Description> &vec) {
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
        std::cout << "Missing fields before END for item: " << od.get_name()
                  << std::endl;
        std::cout << "Skipping object." << std::endl;
        return;
      }

      // Successfully parsed the object
      std::cout << std::endl;
      vec.push_back(od);
      break;

    } else {
      std::cout << "Invalid keyword: " << keyword << std::endl;
      std::cout << "Skipping object." << std::endl;
      return;
    }
  }
}

std::vector<Monster_Description>
load_monster_descriptions(const char *fileName) {
  std::string path = get_save_path(fileName);
  std::vector<Monster_Description> vec;

  std::ifstream file(path);
  if (!file.is_open()) {
    std::cout << "Couldn't open file: " << path << std::endl;
    return vec;
  }

  std::string line;

  // check metadata
  std::getline(file, line);
  if (line != "RLG327 MONSTER DESCRIPTION 1") {
    std::cout << "Not a proper Monster Description File!" << std::endl;
    return vec;
  }

  // until no more lines / eof
  while (std::getline(file, line)) {
    parse_monster(file, line, vec);
  }

  return vec;
}

std::vector<Object_Description> load_object_descriptions(const char *fileName) {
  std::string path = get_save_path(fileName);
  std::vector<Object_Description> vec;

  std::ifstream file(path);
  if (!file.is_open()) {
    std::cout << "Couldn't open file: " << path << std::endl;
    return vec;
  }

  std::string line;

  // check metadata
  std::getline(file, line);
  if (line != "RLG327 OBJECT DESCRIPTION 1") {
    std::cout << "Not a proper Monster Description File!" << std::endl;
    return vec;
  }

  // until no more lines / eof
  while (std::getline(file, line)) {
    parse_object(file, line, vec);
  }

  return vec;
}
