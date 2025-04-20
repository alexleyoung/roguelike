
#include <cstdlib>
#include <string>

#include "dice.hpp"
#include "utils.hpp"

int32_t Dice::roll(void) const {
  int32_t total;
  uint32_t i;

  total = base;

  if (sides) {
    for (i = 0; i < number; i++) {
      total += rand_range(1, sides);
    }
  }

  return total;
}

std::string Dice::to_string() {
  return std::to_string(base) + "+" + std::to_string(number) + "d" +
         std::to_string(sides);
}

std::ostream &Dice::print(std::ostream &o) {
  return o << base << '+' << number << 'd' << sides;
}

std::ostream &operator<<(std::ostream &o, Dice &d) { return d.print(o); }
