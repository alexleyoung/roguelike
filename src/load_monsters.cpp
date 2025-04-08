#include <fstream>
#include <iostream>

#include "load_monsters.hpp"
#include "saves.hpp"

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

  return 0;
}
