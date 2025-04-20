#ifndef UTILS_HPP
#define UTILS_HPP

#define INF 2147483647

#define ATTRIBUTE_INTELLIGENT 0x1
#define ATTRIBUTE_TELEPATHIC 0x2
#define ATTRIBUTE_TUNNELING 0x4
#define ATTRIBUTE_ERRATIC 0x8
#define ATTRIBUTE_PASS 16
#define ATTRIBUTE_PICKUP 32
#define ATTRIBUTE_DESTROY 64
#define ATTRIBUTE_UNIQ 128
#define ATTRIBUTE_BOSS 256

#define NUM_COLORS 8
enum COLOR {
  BLACK,
  RED,
  GREEN,
  YELLOW,
  BLUE,
  MAGENTA,
  CYAN,
  WHITE,
  INVALID_COLOR
};

#define min(x, y) (x < y ? x : y)
#define CEIL(x, y) (((x) + (y) - 1) / (y))
#define C_IS(c, attr) (c->traits & ATTRIBUTE_##attr)
#define abs(x) (x < 0 ? x * -1 : x)
#define rand_range(min, max) ((min) + rand() % ((max) - (min) + 1))
#define COPY_2D_ARRAY(dest, src, rows, cols)                                   \
  do {                                                                         \
    for (int i = 0; i < (rows); i++)                                           \
      for (int j = 0; j < (cols); j++)                                         \
        (dest)[i][j] = (src)[i][j];                                            \
  } while (0)

char *get_save_path(const char *name);

#endif
