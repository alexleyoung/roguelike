#ifndef UTILS_H
#define UTILS_H

#define ATTRIBUTE_INTELLIGENT 0x1
#define ATTRIBUTE_TELEPATHIC 0x2
#define ATTRIBUTE_TUNNELING 0x4
#define ATTRIBUTE_ERRATIC 0x8

#define min(x, y) (x < y ? x : y)
#define C_IS(c, attr) (c->traits & ATTRIBUTE_ ## attr)
#define abs(x) (x < 0 ? x * -1 : x)

#endif
